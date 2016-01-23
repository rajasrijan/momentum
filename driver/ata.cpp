#include "ata.h"
#include "../arch/x86/pci.h"
#include "../DDI/pci_driver.h"
#include "../DDI/block_driver.h"

enum ATA_STATUS
{
    ERR = 1,
    DRQ = 8,
    SRV = 16,
    DF = 32,
    RDY = 64,
    BSY = 128
};

enum ATA_CONTROL
{
    nIEN = 2,
    SRST = 4,
    HOB = 0x80
};

uint16_t control_register = 0x3F6;

int ataReadSectors(size_t offset, size_t count, void* data);
int ataSoftReset();
int ata_probe(pci_device_t *dev, pci_device_id table);
void ata_remove(pci_device_t *dev);
int ata_suspend(pci_device_t *dev, uint32_t state);
int ata_resume(pci_device_t *dev);

class ataBlock : public blockInterface
{
public:

    ataBlock(const char* s)
    {
        name = s;
        ataSoftReset();

    }

    int read(size_t offset, size_t count, void* data)
    {
        return ataReadSectors(offset, count, data);
    }

    int write(size_t offset, size_t count, void* data)
    {
        return 1;
    }
};

pci_driver_t ata_pci_driver_interface = {"IDE PCI Driver.",
    {(uint16_t) PCI_ANY_ID, (uint16_t) PCI_ANY_ID, (uint8_t) PCI_ANY_ID, (uint8_t) PCI_ANY_ID, PCI_BASE_CLASS_STORAGE},
    ata_probe,
    ata_remove,
    ata_suspend,
    ata_resume};

void ata_init()
{
    printf("\nata_init");
    pci_register_driver(&ata_pci_driver_interface);
    const vector_list_t* device_list = pci_getDevices();
    pci_device_t* pci_device = (pci_device_t*) device_list->data;
    uint32_t no_devices = device_list->size(device_list);

    for (int i = 0; i < no_devices; i++)
    {
        pci_device_id devID;
        pci_getDeviceId(&pci_device[i], &devID);
        if ((devID.BaseClass == PCI_BASE_CLASS_STORAGE)&&(devID.SubClass == PCI_CLASS_STORAGE_SATA))
        {
            uint32_t standard_bars[] = {0x1F0, 0x3F6, 0x170, 0x376};
            printf("\nMatch found.%x", devID.SubClass);
            uint32_t BAR[6];
            for (uint32_t j = 0; j < 6; j++)
            {
                BAR[j] = pci_resource_start(&pci_device[i], j);
                if (j < 4)
                {
                    //  Replace BAR[j] with standard bars if it is 0x0 or 0x1
                    BAR[j] = ((BAR[j] == 0) || (BAR[j] == 1)) ? standard_bars[j] : BAR[j];
                }
                printf("\nBAR[%d]=0x%x", j, BAR[j]);
            }
        }
    }
}

int ata_probe(pci_device_t *dev, pci_device_id table)
{
    printf("\nDevice loaded.");
    ataBlock *myBlock = new ataBlock("test_device");
    register_blkdev(myBlock);
    return 0;
}

void ata_remove(pci_device_t *dev)
{

}

int ata_suspend(pci_device_t *dev, uint32_t state)
{
    return 0;
}

int ata_resume(pci_device_t *dev)
{
    return 0;
}

int ataReadSectors(size_t offset, size_t count, void* data)
{
    //LOGHEX(offset)
    offset &= 0x0FFFFFFF;
    uint32_t status = 0;
    uint16_t porta = 0x01F6; //Where bit 24 to 27 are sent.
    uint16_t portb = 0x01F2; //Where no of sectors is sent.
    uint16_t portc = 0x01F3; //Where nit 0 to 7 are sent.
    uint16_t portd = 0x01F4; //Where nit 8 to 15 are sent.
    uint16_t porte = 0x01F5; //Where nit 16 to 23 are sent.
    uint16_t command_port = 0x1F7;
    uint16_t data_port = 0x1F0;

    uint8_t bits_0_to_7 = (uint8_t) (offset & 0xFF);
    uint8_t bits_8_to_15 = (uint8_t) ((offset >> 8) & 0xFF);
    uint8_t bits_16_to_23 = (uint8_t) ((offset >> 16) & 0xFF);
    uint8_t bits_24_to_27 = (uint8_t) ((offset >> 24) | 0xE0);

    /*LOGHEX(count)
    LOGHEX(bits_0_to_7)
    LOGHEX(bits_8_to_15)
    LOGHEX(bits_16_to_23)
    LOGHEX(bits_24_to_27)*/
    //  Send no of sectors to get.
    outb(portb, (uint8_t) count);
    //  Send bit 0 to 7.
    outb(portc, bits_0_to_7);
    //  Send bit 8 to 15.
    outb(portd, bits_8_to_15);
    //  Send bit 0 to 7.
    outb(porte, bits_16_to_23);
    //  Send bit 24 to 27.
    outb(porta, bits_24_to_27);
    //  read with retry
    outb(command_port, 0x20);

    //while (((status = inb(command_port)) & BSY) != BSY);
    while (true)
    {
        status = inb(command_port);
        //LOGHEX(status)
        //printf("\nStatus [%x]", status);
        assert(status & (ERR | DF))
        if (status & DRQ)
            break;

    }
    //printf("\nout of loop.YAY");
    for (size_t c = 0; c < count; c++)
        for (int i = 0; i < 256; i++)
            ((uint16_t*) data)[i] = inw(data_port);
}

int ataSoftReset()
{
    outb(control_register, SRST);
    outb(control_register, 0);
}