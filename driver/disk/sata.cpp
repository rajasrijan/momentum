/*
 * Copyright 2009-2019 Srijan Kumar Sharma
 *
 * This file is part of Momentum.
 *
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <DDI/driver.h>
#include <DDI/pci_driver.h>
#include <arch/x86_64/paging.h>
#include <arch/x86_64/timer.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "ide.h"
#include "sata.h"
#include "sata3.0.h"


static int disk_no = 0;

int sata_probe(pci_device_t *dev, pci_device_id table);
void sata_remove(pci_device_t *dev);
int sata_suspend(pci_device_t *dev, uint32_t state);
int sata_resume(pci_device_t *dev);
int sata_interrupt(pci_device_t *dev);

static pci_device_id supportedDevices[] = {
    {(uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID,
     (uint16_t)PCI_ANY_ID, PCI_BASE_CLASS_STORAGE, PCI_CLASS_STORAGE_SATA,
     (uint8_t)PCI_ANY_ID},
};

pci_driver_t sata_pci_driver_interface = {"Generic Sata Driver",
                                          supportedDevices,
                                          sizeof(supportedDevices) /
                                              sizeof(supportedDevices[0]),
                                          sata_probe,
                                          sata_remove,
                                          sata_suspend,
                                          sata_resume,
                                          sata_interrupt};

void sata_init()
{
    printf("sata_init\n");
    pci_register_driver(&sata_pci_driver_interface);
}

class ahci_controller
{
private:
    HBA_MEM volatile *abar;
    sata_blk_vnode *sata_blk_root_node[32];

public:
    ahci_controller() : abar(nullptr), sata_blk_root_node{}
    {
    }
    ~ahci_controller() {}
    int probe(pci_device_t *dev)
    {
        int ret = 0;
        uint64_t bar5 = (uint64_t)pci_resource_start(dev, 5);
        abar = (HBA_MEM *)bar5;
        // ahci_reset_host_contoller(abar);

        // enable interrupt
        abar->ghc |= HBA_GHC::IE;
        uint32_t pi = abar->pi;
        for (size_t i = 0; i < 32; i++)
        {
            if (pi & (1 << i))
            {
                auto det = abar->ports[i].ssts.det;
                auto spd = abar->ports[i].ssts.spd;
                auto ipm = abar->ports[i].ssts.ipm;
                if (det > 0 || spd > 0 || ipm > 0)
                {
                    printf("SATA%d disk at port: %d\n", spd, i);
                    string disk_name = "sda";
                    disk_name.back() += disk_no++;
                    sata_blk_root_node[i] = new sata_blk_vnode(abar, i, disk_name);
                    ret = mknod(("/dev/" + disk_name).c_str(), sata_blk_root_node[i]);
                    if (ret)
                    {
                        printf("Failed to register block device\n");
                        // ret = 0; // non fatal.
                    }
                }
                else
                    printf("Port disconnected\n");
            }
        }
        if (ret)
            register_interrupt_handler(dev->irq_no, nullptr);
        return ret;
    }
    int handle_interrupt()
    {
        auto is = abar->is;
        //  check pending interrupts
        for (size_t i = 0; i < 32; i++)
        {
            if (is & (1 << i))
            {
                if (!sata_blk_root_node[i])
                    return -EFAULT;
                mtx_unlock(&(sata_blk_root_node[i]->cmd_notify));
            }
        }
        abar->is = is;
        return 0;
    }
};

int sata_probe(pci_device_t *dev, pci_device_id table)
{
    int ret = -ENXIO;
    ahci_controller *controller = new ahci_controller;
    dev->pDriver->user_ptr = controller;
    if ((ret = controller->probe(dev)) == 0)
    {
        printf("Using AHCI mode for SATA device.\n");
        return 0;
    }
    delete controller;
    return ret;
}
void sata_remove(pci_device_t *dev) { return; }
int sata_suspend(pci_device_t *dev, uint32_t state) { return -ENOSYS; }
int sata_resume(pci_device_t *dev) { return -ENOSYS; }
int sata_interrupt(pci_device_t *dev)
{
    int ret = 0;
    ahci_controller *controller = (ahci_controller *)dev->pDriver->user_ptr;
    ret = controller->handle_interrupt();
    return ret;
}

int ahci_reset_host_contoller(HBA_MEM volatile *ahci)
{
    //  HBA Reset HR
    ahci->ghc |= HBA_GHC::HR;
    //  wait for reset to complete
    while (ahci->ghc & HBA_GHC::HR)
    {
        asm("pause");
    }
    return 0;
}

sata_blk_vnode::sata_blk_vnode(HBA_MEM volatile *_abar, size_t _portId, const string &_name) : vnode(nullptr), abar(_abar), portId(_portId)
{
    mtx_init(&port_lock, 0);
    mtx_init(&cmd_notify, 0);
    setName(_name.c_str());
    v_type = VBLK;
    PageManager *pageManager = PageManager::getInstance();
    port = &(abar->ports[portId]);
    command_list = (HBA_CMD_HEADER *)aligned_malloc(sizeof(HBA_CMD_HEADER) * 32, 10);
    memset((void *)command_list, 0x0, sizeof(HBA_CMD_HEADER) * 32);
    fis_list = (HBA_FIS *)aligned_malloc(sizeof(HBA_FIS) * 32, 12);
    memset((void *)fis_list, 0, sizeof(HBA_FIS) * 32);
    uint64_t physical_address = 0;

    disableCommand();
    disableFISReceive();
    // Command list offset: 1K*portno
    // Command list entry size = 32
    // Command list entry maxim count = 32
    // Command list maxim size = 32*32 = 1K per port
    physical_address = pageManager->getPhysicalAddress((uint64_t)command_list);
    Save_64BitPtr(port->clb, physical_address);
    // FIS offset: 32K+256*portno
    // FIS entry size = 256 bytes per port
    physical_address = pageManager->getPhysicalAddress((uint64_t)fis_list);
    Save_64BitPtr(port->fb, physical_address);
    //
    enableFISReceive();
    if (enableCommand())
    {
        // enable failed try reset
        disableCommand(); // disable port
                          // port->sctl;
    }
    // clear error
    port->serr = 0xFFFFFFFF;
    // enable all interrupts
    port->ie = 0xFFFFFFFF;
}

sata_blk_vnode::~sata_blk_vnode()
{
    free((void *)command_list);
    command_list = nullptr;
    free((void *)fis_list);
    fis_list = nullptr;
}

int sata_blk_vnode::bread(ssize_t position, size_t size, char *data, int *bytesRead)
{
    mtx_lock(&cmd_notify);
    mtx_unlock(&cmd_notify);
    uint64_t physical_address = 0;

    int spin = 0; // Spin lock timeout counter
    int slot = getCommandSlot();
    if (slot == -1)
        return -EBUSY;
    sync lock(port_lock);
    HBA_CMD_HEADER volatile *cmdheader = &command_list[slot];
    cmdheader->cfl = sizeof(FIS_REG_H2D) / sizeof(uint32_t); // Command FIS size
    cmdheader->w = 0;                                        // Read from device
    cmdheader->prdtl = (uint16_t)((size - 1) >> 4) + 1;      // PRDT entries count

    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL *)aligned_malloc(sizeof(HBA_CMD_TBL), 10);
    memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) + (cmdheader->prdtl - 1) * sizeof(HBA_PRDT_ENTRY));
    physical_address = PageManager::getInstance()->getPhysicalAddress((uint64_t)cmdtbl);
    Save_64BitPtr(cmdheader->ctba, physical_address);

    for (size_t i = 0, total_size = (512 * size); i < cmdheader->prdtl;
         i++, total_size -= 0x800000)
    {
        physical_address = PageManager::getInstance()->getPhysicalAddress(
            (uint64_t)data + (i * 0x800000));
        Save_64BitPtr(cmdtbl->prdt_entry[i].dba, physical_address);
        cmdtbl->prdt_entry[i].dbc = min(0x800000ul, total_size) - 1;
        cmdtbl->prdt_entry[i].i = 1;
    }
    // Setup command
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D *)(&cmdtbl->cfis);

    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c = 1; // Command
    cmdfis->command = ATA_COMMAND::READ_DMA_EXT;

    cmdfis->lba0 = (uint8_t)position;
    cmdfis->lba1 = (uint8_t)(position >> 8);
    cmdfis->lba2 = (uint8_t)(position >> 16);
    cmdfis->device = 1 << 6; // LBA mode

    cmdfis->lba3 = (uint8_t)(position >> 24);
    cmdfis->lba4 = (uint8_t)position;
    cmdfis->lba5 = (uint8_t)(position >> 8);

    cmdfis->countl = size & 0xFF;
    cmdfis->counth = (size >> 8) & 0xFF;

    // The below loop waits until the port is no longer busy before issuing a new
    // command
    while ((port->tfd & (ATA_STATUS::BSY | ATA_STATUS::DRQ)) && spin < 1000000)
    {
        spin++;
    }
    if (spin == 1000000)
    {
        printf("Drive is busy\n");
        return -EAGAIN;
    }

    port->ci = 1 << slot; // Issue command
    // Wait for completion

    mtx_lock(&cmd_notify);
    mtx_unlock(&cmd_notify);
    mtx_unlock(&cmd_notify);

    // Check again
    if (port->is & HBA_PxIS::TFES)
    {
        printf("Disk read failed\n");
        return -EIO;
    }

    return 0;
}

int sata_blk_vnode::ioctl(uint32_t command, void *data, int fflag)
{
    switch (command)
    {
    case BLKGETSIZE64:
    {
        ata_identity identity;
        identify(identity);
        *((uint64_t *)data) = identity.MAX_LBA;
        return 0;
    }
    default:
        return -EINVAL;
    }
    return 0;
}

int sata_blk_vnode::readdir(vector<shared_ptr<vnode>> &vnodes)
{
    return -ENOSYS;
}

int sata_blk_vnode::open(uint32_t flags) { return -ENOSYS; }

int sata_blk_vnode::getCommandSlot()
{
    sync lock(port_lock);
    // If not set in SACT and CI, the slot is free
    uint32_t slots = (port->sact | port->ci);
    for (int i = 0; i < 32; i++)
    {
        if ((slots & 1) == 0)
            return i;
        slots >>= 1;
    }
    printf("Cannot find free command list entry\n");
    return -1;
}

int sata_blk_vnode::identify(ata_identity &ident)
{
    mtx_lock(&cmd_notify);
    mtx_unlock(&cmd_notify);
    ata_identity *identify_data = (ata_identity *)aligned_malloc(sizeof(ata_identity), 12);
    memset(identify_data, 0, sizeof(ata_identity));
    uint64_t physical_address = 0;
    size_t position = 0;

    int spin = 0; // Spin lock timeout counter
    int slot = getCommandSlot();
    if (slot == -1)
        return -EBUSY;

    HBA_CMD_HEADER volatile *cmdheader = &command_list[slot];
    memset((void *)cmdheader, 0, sizeof(HBA_CMD_HEADER));
    cmdheader->cfl = sizeof(FIS_REG_H2D) / sizeof(uint32_t); // Command FIS size
    cmdheader->w = 0;                                        // Read from device
    cmdheader->prdtl = 1;                                    // PRDT entries count
                                                             // 3FFFFF
    HBA_CMD_TBL volatile *cmdtbl = (HBA_CMD_TBL *)aligned_malloc(sizeof(HBA_CMD_TBL), 10);
    memset((void *)cmdtbl, 0, sizeof(HBA_CMD_TBL));

    // 8K bytes (16 sectors) per PRDT
    physical_address = PageManager::getInstance()->getPhysicalAddress((uint64_t)identify_data);
    Save_64BitPtr(cmdtbl->prdt_entry[0].dba, physical_address);
    cmdtbl->prdt_entry[0].dbc = 0x200 - 1; // 512 bytes per sector
    cmdtbl->prdt_entry[0].i = 1;

    physical_address = PageManager::getInstance()->getPhysicalAddress((uint64_t)cmdtbl);
    Save_64BitPtr(cmdheader->ctba, physical_address);

    // Setup command
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D *)(&cmdtbl->cfis);
    memset((void *)cmdfis, 0, sizeof(FIS_REG_H2D));

    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c = 1; // Command
    cmdfis->command = ATA_COMMAND::IDENTIFY;

    cmdfis->lba0 = (uint8_t)position;
    cmdfis->lba1 = (uint8_t)(position >> 8);
    cmdfis->lba2 = (uint8_t)(position >> 16);
    cmdfis->device = 1 << 6; // LBA mode

    cmdfis->lba3 = (uint8_t)(position >> 24);
    cmdfis->lba4 = (uint8_t)position;
    cmdfis->lba5 = (uint8_t)(position >> 8);

    cmdfis->countl = 1;
    cmdfis->counth = 0;

    // The below loop waits until the port is no longer busy before issuing a new
    // command
    while ((port->tfd & (ATA_STATUS::BSY | ATA_STATUS::DRQ)) && spin < 1000000)
    {
        spin++;
    }
    if (spin == 1000000)
    {
        printf("Drive is busy\n");
        asm("cli;hlt");

        return -EAGAIN;
    }
    port->ci = 1 << slot; // Issue command
    // Wait for completion
    mtx_lock(&cmd_notify);
    mtx_unlock(&cmd_notify);
    // Check again
    if (port->is & HBA_PxIS::TFES)
    {

        printf("Disk read failed\n");
        asm("cli;hlt");
        return -EIO;
    }
    // Check again
    if (port->tfd & 0xFF00)
    {

        printf("Disk read failed\n");
        asm("cli;hlt");
        return -EIO;
    }
    ident = *identify_data;
    free(identify_data);
    return 0;
}

int sata_blk_vnode::disableCommand()
{
    port->cmd.st = 0;
    while (port->cmd.cr == 1)
    {
        asm("pause");
    }
    return 0;
}

int sata_blk_vnode::disableFISReceive()
{
    // disable FIS receiver
    port->cmd.fre = 0;
    // wait for FIS receiver to stop
    while (port->cmd.fr == 1)
    {
        asm("pause");
    }
    return 0;
}

int sata_blk_vnode::enableCommand()
{
    port->cmd.st = 1;
    if (port->cmd.st == 0)
    {
        return -EFAULT;
    }
    return 0;
}

int sata_blk_vnode::enableFISReceive()
{
    // enable FIS receiver
    port->cmd.fre = 1;
    // wait for FIS receiver to start
    while (port->cmd.fr == 0)
    {
        asm("pause");
    }
    return 0;
}

MODULT_INIT(sata_init)