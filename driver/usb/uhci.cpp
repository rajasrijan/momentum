/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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
#include "uhci.h"
#include <DDI/driver.h>
#include <DDI/pci_driver.h>
#include <kernel/vfs.h>
#include <arch/x86_64/timer.h>
#include <errno.h>

#define USB_HC_NAME "/dev/usb-hca"
#define USB_HC_COUNTER_OFFSET 11
static int controller_count = 0;

// 	Usb Command
#define USBCMD(base) (base + 0x00)
// 	Usb Status
#define USBSTS(base) (base + 0x02)
// 	Usb Interrupt Enable
#define USBINTR(base) (base + 0x04)
//  Frame Number
#define FRNUM(base) (base + 0x06)
// 	Frame List Base Address
#define FRBASEADD(base) (base + 0x08)
// 	Start Of Frame Modify
#define SOFMOD(base) (base + 0x0C)
// 	Port 1 Status/Control
#define PORTSC1(base) (base + 0x10)
// 	Port 2 Status/Control
#define PORTSC2(base) (base + 0x12)

//  run schedule
#define CMD_HC_RUN (1 << 0)
//  reset host controller
#define CMD_HC_RST (1 << 1)
//  host controller configured
#define CMD_HC_CFG (1 << 6)

#pragma pack(push, 1)
struct uhci_command_reg
{

    uint8_t run : 1;
    uint8_t host_controller_reset : 1;
    uint8_t global_reset : 1;
    uint8_t global_suspend : 1;
    uint8_t global_resume : 1;
    uint8_t software_debug : 1;
    uint8_t config : 1;
    uint8_t max_packet : 1;
    uint8_t reserved;
};

struct uhci_status_reg
{
    union {
        uint16_t value;
        struct
        {
            uint8_t interrupt : 1;
            uint8_t error_interrupt : 1;
            uint8_t resume_detected : 1;
            uint8_t system_error : 1;
            uint8_t process_error : 1;
            uint8_t halted : 1;
            uint8_t resv0 : 2;
            uint8_t reserved;
        };
    };
};

#define INTR_TIMEOUTCRC (1 << 0)
#define INTR_RESUME (1 << 1)
#define INTR_COMPLETE (1 << 2)
#define INTR_SHORTPKT (1 << 3)

struct interrupt_enable_register
{
    uint8_t resv0;
    uint8_t resv1 : 4;
    uint8_t short_packet : 1;
    uint8_t complete : 1;
    uint8_t resume : 1;
    uint8_t timeout_crc : 1;
};

#define PORT_ENABLE (1 << 2)

struct port_status_control_registers
{
    union {
        uint16_t value;
        struct
        {
            uint8_t reserved0 : 3;
            uint8_t suspend : 1;
            uint8_t reserved1 : 2;
            uint8_t reset : 1;
            uint8_t low_speed : 1;
            uint8_t reserved2 : 1; //    (Must be 1)
            uint8_t resume_detected : 1;
            uint8_t line_status : 2;
            uint8_t port_enable_changed : 1;
            uint8_t port_enabled : 1;
            uint8_t connected_changed : 1;
            uint8_t connected : 1;
        };
    };
};

//  4K aligned
struct frame_list_pointer
{
    uint8_t is_empty : 1;
    uint8_t is_queue : 1;
    uint8_t process_full_queue : 1;
    uint8_t reserved : 1;
    uint32_t descriptor_address : 28;
};

struct transfer_descriptor_status
{
    uint16_t actual_length : 11;       //
    uint8_t reserved1 : 6;             //
    uint8_t bit_stuff_error : 1;       //Set by UHCI Controller
    uint8_t timeout_crc : 1;           //Set by UHCI Controller
    uint8_t non_acknowledged : 1;      //Set by UHCI Controller
    uint8_t babble_detected : 1;       //Set by UHCI Controller
    uint8_t data_buffer_error : 1;     //Set by UHCI Controller
    uint8_t stalled : 1;               //Set by UHCI Controller
    uint8_t active : 1;                //Set by UHCI Controller
    uint8_t interrupt_on_complete : 1; //
    uint8_t is_isochronous : 1;        //
    uint8_t low_speed : 1;             //
    uint8_t error_counter : 2;         //
    uint8_t short_packet_detect : 1;   //
    uint8_t reserved0 : 2;             //
};

#define PACKET_TYPE_IN 0x69
#define PACKET_TYPE_OUT 0xE1
#define PACKET_TYPE_SETUP 0x2D

struct transfer_descriptor_packet_header
{
    uint8_t packet_type;          // 	0x69 = IN, 0xE1 = OUT, 0x2D = SETUP
    uint8_t device : 7;           //
    uint8_t endpoint : 4;         //
    uint8_t data_toggle : 1;      //
    uint8_t reserved : 1;         //
    uint16_t maximum_length : 11; // 	(Length - 1)
};

// 16 bytes aligned
struct transfer_descriptor
{
    frame_list_pointer link_pointer;
    transfer_descriptor_status status;
    transfer_descriptor_packet_header packet_header;
    uint32_t buffer_address;
};

struct control_transfer
{
    union{
        uint8_t bmRequestType;
        struct {
            
        };
    };
};

#pragma pack(pop)

#define FRAME_POINTER_EMPTY (1 << 0)
#define FRAME_TRANSFER_DESC (0)
#define FRAME_QUEUE_DESC (1 << 1)

#define SOF_DEFAULT (64)

class uhci_char_interface : public vnode
{
  private:
    uint32_t USBBASEPort;
    //  pointer to a 'Frame List Pointer'
    frame_list_pointer *frame_list;
    transfer_descriptor *td;
    //  frame lists should be aligned on 4k boundry
    uint32_t frame_list_base_address;

  public:
    uhci_char_interface(uint32_t USBBASE, const string &_name) : vnode(nullptr), USBBASEPort(USBBASE)
    {
        setName(_name.c_str());
        printf("creating interface for [%s]\n", getName().c_str());
        //  reset coltroller
        outw(USBCMD(USBBASEPort), CMD_HC_RST);
        sleep(50);
        if (inw(USBCMD(USBBASEPort)) & CMD_HC_RST)
            printf("CMD_HC_RST not completed yet!\n");

        //  disable interrupt
        outw(USBINTR(USBBASEPort), 0);
        outw(USBCMD(USBBASEPort), 0);
        frame_list = (frame_list_pointer *)aligned_malloc(1024 * sizeof(frame_list_pointer), 12);

        for (int i = 0; i < 1024; i++)
        {
            frame_list[i].is_empty = 1;
            frame_list[i].is_queue = 0;
            frame_list[i].process_full_queue = 0;
            frame_list[i].reserved = 0;
        }
        frame_list_base_address = *(uint32_t *)&frame_list;
        // set at 1ms
        outb(SOFMOD(USBBASEPort), SOF_DEFAULT);
        //  set DMA address
        outl(FRBASEADD(USBBASEPort), frame_list_base_address);
        // set frame no. to 0
        outl(FRNUM(USBBASEPort), 0);
        // //  enable all interrupts
        // outw(USBINTR(USBBASEPort), INTR_TIMEOUTCRC | INTR_RESUME | INTR_COMPLETE | INTR_SHORTPKT);
        //  run schedule
        outw(USBCMD(USBBASEPort), CMD_HC_RST | CMD_HC_CFG);
        enumerate_devices();
    }

    ~uhci_char_interface()
    {
    }

    void enumerate_devices()
    {
        transfer_descriptor td = {0};
        td.link_pointer.is_empty = 1;
        td.packet_header.packet_type = PACKET_TYPE_SETUP;
        td.packet_header.device;
        td.packet_header.endpoint;
        td.packet_header.data_toggle;
        td.packet_header.reserved;
        td.packet_header.maximum_length;
        td.buffer_address;
    }

    int read(size_t offset, size_t count, void *data)
    {
        printf("%s not implemented\n", __FUNCTION__);
        __asm__("cli;hlt");
        return ENOSYS;
    }

    int write(size_t offset, size_t count, void *data)
    {
        printf("%s not implemented\n", __FUNCTION__);
        __asm__("cli;hlt");
        return ENOSYS;
    }

    int readdir(vector<shared_ptr<vnode>> &vnodes)
    {
        return ENOTDIR;
    }
};

void uhci_init();
int uhci_probe(pci_device_t *dev, pci_device_id table);
void uhci_remove(pci_device_t *dev);
int uhci_suspend(pci_device_t *dev, uint32_t state);
int uhci_resume(pci_device_t *dev);

int uhci_probe(pci_device_t *dev, pci_device_id table)
{
    int ret = 0;
    printf("Device probed\n");
    pci_device_id devID;
    dev->getDeviceId(&devID);
    uint32_t USBBASE, portbase;
    USBBASE = pci_resource_start(dev, 4);
    USBBASE &= 0xFFE0;
    portbase = pci_resource_start(dev, 0);
    printf("USBBASE=0x%x\n", USBBASE);
    printf("portbase=0x%x\n", portbase);
    char controller_name[] = USB_HC_NAME;
    controller_name[USB_HC_COUNTER_OFFSET] += controller_count++;
    auto pUhciCharInterface = new uhci_char_interface(USBBASE, "USB-HC");
    ret = mknod(controller_name, pUhciCharInterface);
    if (ret)
    {
        printf("Failed to register block device\n");
        ret = 0; // non fatal.
    }
    return ret;
}

void uhci_remove(pci_device_t *dev)
{
}

int uhci_suspend(pci_device_t *dev, uint32_t state)
{
    return 0;
}

int uhci_resume(pci_device_t *dev)
{
    return 0;
}

static pci_device_id supportedDevices[] = {
    {(uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, PCI_BASE_CLASS_SERIAL, PCI_CLASS_SERIAL_USB, PCI_CLASS_SERIAL_USB_UHCI}};

pci_driver_t uhci_driver_interface = {
    "Universal Host Controller Driver",
    supportedDevices,
    1,
    uhci_probe,
    uhci_remove,
    uhci_suspend,
    uhci_resume};

void uhci_init()
{
    printf("init USB\n");
    pci_register_driver(&uhci_driver_interface);
}

//  add to module list
MODULT_INIT(uhci_init)