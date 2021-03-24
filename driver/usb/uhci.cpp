/*
 * Copyright 2009-2021 Srijan Kumar Sharma
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

#include <stdint.h>
#include <vector>
#include "uhci.h"
#include <DDI/driver.h>
#include <DDI/pci_driver.h>
#include <kernel/vfs.h>
#include <kernel/vnode.h>
#include <arch/x86_64/timer.h>
#include <arch/x86_64/paging.h>
#include <errno.h>
#include <stdlib.h>

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
#define PORTSCn(base, n) (base + 0x10 + (n * 2))

//  run schedule
#define CMD_HC_RUN (1 << 0)
//  reset host controller
#define CMD_HC_RST (1 << 1)
//  host controller configured
#define CMD_HC_CFG (1 << 6)

#pragma pack(push, 1)
struct uhci_command_reg {

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

struct uhci_status_reg {
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

struct interrupt_enable_register {
    uint8_t resv0;
    uint8_t resv1 : 4;
    uint8_t short_packet : 1;
    uint8_t complete : 1;
    uint8_t resume : 1;
    uint8_t timeout_crc : 1;
};

#define PORT_ENABLE (1 << 2)

struct port_status_control_registers {
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
struct frame_list_pointer {
    uint8_t is_empty : 1;
    uint8_t is_queue : 1;
    uint8_t process_full_queue : 1;
    uint8_t reserved : 1;
    uint32_t descriptor_address : 28;
};

struct transfer_descriptor_status {
    uint16_t actual_length : 11;       //
    uint8_t reserved1 : 6;             //
    uint8_t bit_stuff_error : 1;       // Set by UHCI Controller
    uint8_t timeout_crc : 1;           // Set by UHCI Controller
    uint8_t non_acknowledged : 1;      // Set by UHCI Controller
    uint8_t babble_detected : 1;       // Set by UHCI Controller
    uint8_t data_buffer_error : 1;     // Set by UHCI Controller
    uint8_t stalled : 1;               // Set by UHCI Controller
    uint8_t active : 1;                // Set by UHCI Controller
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

struct transfer_descriptor_packet_header {
    uint8_t packet_type; // 	0x69 = IN, 0xE1 = OUT, 0x2D = SETUP
    uint8_t device : 7;
    uint8_t endpoint : 4;
    uint8_t data_toggle : 1;
    uint8_t reserved : 1;
    uint16_t maximum_length : 11; // 	(Length - 1)
};

// 16 bytes aligned
struct transfer_descriptor {
    frame_list_pointer link_pointer;
    transfer_descriptor_status status;
    transfer_descriptor_packet_header packet_header;
    uint32_t buffer_address;
};

#define GET_STATUS 0
#define CLEAR_FEATURE 1
#define SET_FEATURE 3
#define SET_ADDRESS 5
#define GET_DESCRIPTOR 6
#define SET_DESCRIPTOR 7
#define GET_CONFIGURATION 8
#define SET_CONFIGURATION 9
#define GET_INTERFACE 10
#define SET_INTERFACE 11
#define SYNC_FRAME 12

#define DESC_TYPE_DEVICE 1
#define DESC_TYPE_CONFIGURATION 2
#define DESC_TYPE_STRING 3
#define DESC_TYPE_INTERFACE 4
#define DESC_TYPE_ENDPOINT 5
#define DESC_TYPE_DEVICE_QUALIFIER 6
#define DESC_TYPE_OTHER_SPEED_CONFIGURATION 7
#define DESC_TYPE_INTERFACE_POWER 8

struct control_transfer {
    union {
        uint8_t bmRequestType;
        struct
        {
            uint8_t data_transfer_direction : 1;
            uint8_t type_of_request : 2;
            uint8_t Recipient : 4;
        };
    };
    uint8_t bRequest;
    uint16_t wValue, wIndex, wLength;
};

struct DEVICE_desc {
    uint8_t bLength, bDescriptorType;
    uint8_t bcdUSB[2];
    uint8_t bDeviceClass, bDeviceSubClass, bDeviceProtocol, bMaxPacketSize0;
    uint16_t idVendor, idProduct, bcdDevice;
    uint8_t iManufacturer, iProduct, iSerialNumber, bNumConfigurations;
    void print()
    {
        printf("USB ver. %d.%d\n\tDevice class [%x]\n\tDevice sub class [%x]\n\tDevice protocol "
               "[%x]\n\tNumConfigurations [%x]\n",
               bcdUSB[1], bcdUSB[0], bDeviceClass, bDeviceSubClass, bDeviceProtocol, bNumConfigurations);
    }
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
    uint64_t port_count;
    //  frame lists should be aligned on 4k boundry
    uint32_t frame_list_base_address;

  public:
    uhci_char_interface(const uhci_char_interface &) = delete;
    uhci_char_interface(uint32_t USBBASE, const string &_name)
        : vnode(nullptr), USBBASEPort(USBBASE), frame_list(nullptr), port_count(0), frame_list_base_address(0)
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
        memset(frame_list, 0, 1024 * sizeof(frame_list_pointer));
        for (int i = 0; i < 1024; i++) {
            frame_list[i].is_empty = 1;
            frame_list[i].is_queue = 0;
            frame_list[i].process_full_queue = 0;
            frame_list[i].reserved = 0;
        }
        uint64_t long_base_pointer = 0;
        if (PageManager::getPhysicalAddress((uint64_t)frame_list, long_base_pointer)) {
            printf("Failed to get physical address!");
            asm("cli;hlt");
        }
        frame_list_base_address = (uint32_t)long_base_pointer;
        //  stop transfers
        outw(USBCMD(USBBASEPort), 0);
        // set at 1ms
        outb(SOFMOD(USBBASEPort), SOF_DEFAULT);
        //  set DMA address
        outl(FRBASEADD(USBBASEPort), frame_list_base_address);
        // set frame no. to 0
        outl(FRNUM(USBBASEPort), 0);
        // //  enable all interrupts
        // outw(USBINTR(USBBASEPort), INTR_TIMEOUTCRC | INTR_RESUME | INTR_COMPLETE | INTR_SHORTPKT);
        // //  run schedule
        // outw(USBCMD(USBBASEPort), CMD_HC_RUN | CMD_HC_CFG);
        //  stop transfers
        outw(USBCMD(USBBASEPort), 0);
        sleep(50);
        for (port_count = 0; port_count < 128; port_count++) {
            auto tmp = inw(PORTSCn(USBBASEPort, port_count));
            if ((tmp & (1 << 7)) == 0)
                break;
            printf("USB Port %d %s\n", port_count, (tmp & 1) ? "connected" : "not connected");
            if ((tmp & 1) && !(tmp & PORT_ENABLE)) {
                printf("Enabling port %d\n", port_count);
                outw(PORTSCn(USBBASEPort, port_count), PORT_ENABLE);
                sleep(50);
                tmp = inw(PORTSCn(USBBASEPort, port_count));
                if (!(tmp & PORT_ENABLE)) {
                    printf("Failed to enable port\n");
                }
            }
        }
        //  stop transfers
        outw(USBCMD(USBBASEPort), 0);
        set_address(0);
        DEVICE_desc deviceDesc = {};
        get_descriptor(1, 0, deviceDesc);
        deviceDesc.print();
    }

    ~uhci_char_interface()
    {
    }
    uhci_char_interface &operator=(const uhci_char_interface &) = delete;
    int open(uint64_t flags)
    {
        return -ENOSYS;
    }
    int get_descriptor(uint8_t address, uint8_t endpoint, DEVICE_desc &deviceDesc)
    {
        printf("Get Descriptor\n");
        size_t transfer_size = 0;
        alignas(16) control_transfer get_desc_pld = {0x80, GET_DESCRIPTOR, DESC_TYPE_DEVICE << 8, 0, sizeof(DEVICE_desc)};
        int ret = transfer<PACKET_TYPE_SETUP>(address, endpoint, &get_desc_pld, sizeof(control_transfer), transfer_size, (uint8_t *)&deviceDesc);
        return ret;
    }

    int set_address(uint8_t endpoint)
    {
        printf("Set Address\n");
        alignas(16) control_transfer set_add_pld = {0, SET_ADDRESS, 1, 0, 0};
        size_t transfer_size = 0;
        return transfer<PACKET_TYPE_SETUP>(0, endpoint, &set_add_pld, sizeof(control_transfer), transfer_size, nullptr);
    }

    void link_td(transfer_descriptor *src, transfer_descriptor *dst)
    {
        uint64_t long_base_pointer = 0;
        if (PageManager::getPhysicalAddress((uint64_t)dst, long_base_pointer)) {
            printf("Failed to get physical address!");
            asm("cli;hlt");
        }
        uint32_t dst_phy_addr = (uint32_t)long_base_pointer;

        src->link_pointer.descriptor_address = (dst_phy_addr >> 4);
        src->link_pointer.is_empty = 0;
    }

    uint32_t virtual_addr_to_phy(void *ptr)
    {
        uint64_t long_base_pointer = 0;
        if (PageManager::getPhysicalAddress((uint64_t)ptr, long_base_pointer)) {
            printf("Failed to get physical address!");
            asm("cli;hlt");
        }
        return (uint32_t)long_base_pointer;
    }

    template <uint8_t packet_type>
    int transfer(uint8_t address, uint8_t endpoint, control_transfer *pPayload, size_t sz, size_t &actual_transfer, uint8_t *pData)
    {
        shared_ptr<transfer_descriptor> pSetupDesc((transfer_descriptor *)aligned_malloc(sizeof(transfer_descriptor), 4));
        shared_ptr<transfer_descriptor> pDataDesc((transfer_descriptor *)aligned_malloc(sizeof(transfer_descriptor), 4));
        shared_ptr<transfer_descriptor> pStatusDesc((transfer_descriptor *)aligned_malloc(sizeof(transfer_descriptor), 4));
        memset(pSetupDesc.get(), 0, sizeof(transfer_descriptor));
        memset(pDataDesc.get(), 0, sizeof(transfer_descriptor));
        memset(pStatusDesc.get(), 0, sizeof(transfer_descriptor));
        uint64_t long_base_pointer = 0;
        if (PageManager::getPhysicalAddress((uint64_t)pPayload, long_base_pointer)) {
            printf("Failed to get physical address!");
            asm("cli;hlt");
        }
        uint32_t payload_phy_addr = (uint32_t)long_base_pointer;
        long_base_pointer = 0;

        if (PageManager::getPhysicalAddress((uint64_t)pSetupDesc.get(), long_base_pointer)) {
            printf("Failed to get physical address!");
            asm("cli;hlt");
        }
        uint32_t setup_phy_addr = (uint32_t)long_base_pointer;

        pSetupDesc->link_pointer.is_empty = 1;
        pSetupDesc->status.active = 1;
        pSetupDesc->status.interrupt_on_complete = 0;
        pSetupDesc->packet_header.packet_type = packet_type;
        pSetupDesc->packet_header.device = address;
        pSetupDesc->packet_header.endpoint = endpoint;
        pSetupDesc->packet_header.data_toggle = 0;
        pSetupDesc->packet_header.maximum_length = sz - 1;
        pSetupDesc->buffer_address = payload_phy_addr;
        if (pPayload->wLength > 0) {
            pDataDesc->link_pointer.is_empty = 1;
            pDataDesc->status.active = 1;
            pDataDesc->status.interrupt_on_complete = 0;
            pDataDesc->packet_header.packet_type = PACKET_TYPE_IN;
            pDataDesc->packet_header.device = address;
            pDataDesc->packet_header.endpoint = endpoint;
            pDataDesc->packet_header.data_toggle = 0;
            pDataDesc->packet_header.maximum_length = pPayload->wLength - 1;
            pDataDesc->buffer_address = virtual_addr_to_phy(pData);
            link_td(pSetupDesc.get(), pDataDesc.get());
            link_td(pDataDesc.get(), pStatusDesc.get());
        } else {
            link_td(pSetupDesc.get(), pStatusDesc.get());
        }
        // if (packet_type == PACKET_TYPE_SETUP)
        {
            pStatusDesc->packet_header.packet_type = PACKET_TYPE_IN;
        }
        pStatusDesc->link_pointer.is_empty = 1;
        pStatusDesc->status.active = 1;
        pStatusDesc->status.interrupt_on_complete = 1;
        pStatusDesc->packet_header.device = address;
        pStatusDesc->packet_header.endpoint = endpoint;
        pStatusDesc->packet_header.data_toggle = 1;
        pStatusDesc->packet_header.maximum_length = 0x7FF;
        pStatusDesc->buffer_address = 0;

        frame_list[0].descriptor_address = (setup_phy_addr >> 4);
        frame_list[0].reserved = 0;
        frame_list[0].process_full_queue = 1;
        frame_list[0].is_queue = 0;
        frame_list[0].is_empty = 0;
        outw(FRNUM(USBBASEPort), 0);
        outw(USBCMD(USBBASEPort), CMD_HC_RUN);
        sleep(50);
        auto status = inw(USBSTS(USBBASEPort));
        for (int i = 0; i < 10 && status == 0; i++, status = inw(USBSTS(USBBASEPort))) {
            printf("try %d, status 0x%x\n", i + 1, status);
            sleep(50);
        }
        frame_list[0].is_empty = 1;
        actual_transfer = pSetupDesc->status.actual_length;
        printf("status 0x%x\n", status);
        sleep(50);
        //  clear status
        outw(USBSTS(USBBASEPort), 1);
        sleep(50);
        status = inw(USBSTS(USBBASEPort));
        printf("status 0x%x\n", status);
        //  stop transfers
        outw(USBCMD(USBBASEPort), 0);
        // print_descriptor(pSetupDesc.get());
        // print_descriptor(pStatusDesc.get());
        return 0;
    }

    void print_descriptor(transfer_descriptor *pDesc)
    {
        printf("USB Transfer status:\n\tactual_length: %d"
               "\tbit_stuff_error: %d\n\ttimeout_crc: %d"
               "\tnon_acknowledged: %d\n\tbabble_detected: %d"
               "\tdata_buffer_error: %d\n\tstalled: %d"
               "\tactive: %d\n\tinterrupt_on_complete: %d"
               "\tis_isochronous: %d\n\tlow_speed: %d"
               "\terror_counter: %d\n\tshort_packet_detect: %d\n",
               pDesc->status.actual_length, pDesc->status.bit_stuff_error, pDesc->status.timeout_crc, pDesc->status.non_acknowledged, pDesc->status.babble_detected, pDesc->status.data_buffer_error, pDesc->status.stalled,
               pDesc->status.active, pDesc->status.interrupt_on_complete, pDesc->status.is_isochronous, pDesc->status.low_speed, pDesc->status.error_counter, pDesc->status.short_packet_detect);
    }
    int bread(ssize_t position, size_t size, char *data, int *bytesRead)
    {
        return ENOSYS;
    }
    int bwrite(ssize_t position, size_t size, const char *data, int *bytesRead)
    {
        return ENOSYS;
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

class uhci_pci_driver : public pci_driver
{
  private:
    /* data */
  public:
    uhci_pci_driver()
        : pci_driver("UHCI Driver")
    {
    }
    ~uhci_pci_driver()
    {
    }

    int probe(pci_device_t *dev, pci_device_id table)
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
        if (ret) {
            printf("Failed to register block device\n");
            ret = 0; // non fatal.
        }
        return ret;
    }
};

pci_driver *create_uhci_instance(pci_device_t *dev)
{
    return (pci_driver *)new uhci_pci_driver();
}

void destroy_uhci_instance(pci_driver *driver)
{
    delete driver;
}

static pci_device_id supportedDevices[] = {{(uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, PCI_BASE_CLASS_SERIAL, PCI_CLASS_SERIAL_USB, PCI_CLASS_SERIAL_USB_UHCI}};

pci_driver_interface uhci_driver_interface = {supportedDevices, 1, create_uhci_instance, destroy_uhci_instance};

void uhci_init()
{
    printf("init USB\n");
    pci_register_driver(&uhci_driver_interface);
}

//  add to module list
//  MODULT_INIT(uhci_init)