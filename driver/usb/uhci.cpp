/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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
#include <ddi/driver.h>
#include <ddi/pci_driver.h>
#include <kernel/vfs.h>

#define USB_HC_NAME "/dev/usb-hca"
#define USB_HC_COUNTER_OFFSET 11
static int controller_count=0;

// 	Usb Command
#define	USBCMD(base) (base+0x00)
// 	Usb Status
#define USBSTS(base) (base+0x02)
// 	Usb Interrupt Enable
#define	USBINTR(base) (base+0x04)
//  Frame Number
#define FRNUM(base) (base+0x06)
// 	Frame List Base Address
#define FRBASEADD(base) (base+0x08)
// 	Start Of Frame Modify
#define SOFMOD(base) (base+0x0C)
// 	Port 1 Status/Control
#define PORTSC1(base) (base+0x10)
// 	Port 2 Status/Control 
#define PORTSC2(base) (base+0x12)

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
    uint8_t interrupt : 1;
    uint8_t error_interrupt : 1;
    uint8_t resume_detected : 1;
    uint8_t system_error : 1;
    uint8_t process_error : 1;
    uint8_t halted : 1;
    uint8_t resv0 : 2;
    uint8_t reserved;
};

struct interrupt_enable_register
{
    uint8_t resv0;
    uint8_t resv1 : 4;
    uint8_t short_packet : 1;
    uint8_t complete : 1;
    uint8_t resume : 1;
    uint8_t timeout_crc : 1;
};

struct port_status_control_registers
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

struct frame_list_entry
{
    uint32_t first_descriptor_address : 28;
    uint8_t reserved : 1;
    uint8_t process_full_queue : 1;
    uint8_t is_queue : 1;
    uint8_t is_empty : 1;
};

struct uhci_Descriptor
{
    uint32_t next_descriptor; // 	Same as Frame Entry Above
    uint32_t status;          // 	See below
    uint32_t packet_header;   // 	See below
    uint32_t buffer_address;  // 	32-bit address of data buffer
    uint32_t system_use;      // 	16-byte area reserved for use by the system
};

struct uhci_descriptor_status
{
    uint8_t reserved0 : 2;             //
    uint8_t short_packet_detect : 1;   //
    uint8_t error_counter : 2;         //
    uint8_t low_speed : 1;             //
    uint8_t is_isochronous : 1;        //
    uint8_t interrupt_on_complete : 1; //
    uint8_t active : 1;                //Set by UHCI Controller
    uint8_t stalled : 1;               //Set by UHCI Controller
    uint8_t data_buffer_error : 1;     //Set by UHCI Controller
    uint8_t babble_detected : 1;       //Set by UHCI Controller
    uint8_t non_acknowledged : 1;      //Set by UHCI Controller
    uint8_t timeout_crc : 1;           //Set by UHCI Controller
    uint8_t bit_stuff_error : 1;       //Set by UHCI Controller
    uint8_t reserved1 : 6;             //
    uint16_t actual_length : 11;       //
};

struct uhci_descriptor_packet_header
{
    uint16_t maximum_length : 11; // 	(Length - 1)
    uint8_t reserved : 1;         //
    uint8_t data_toggle : 1;      //
    uint8_t endpoint : 4;         //
    uint8_t device : 7;           //
    uint8_t packet_type;          // 	0x69 = IN, 0xE1 = OUT, 0x2D = SETUP
};

class uhci_char_interface : public vnode
{
  private:
    uint32_t USBBASEPort;

  public:
    uhci_char_interface(uint32_t USBBASE, const string &_name) : USBBASEPort(USBBASE)
    {
        v_name = _name;
        printf("creating interface for [%s]\n", v_name.c_str());
    }
    ~uhci_char_interface()
    {
    }
    int read(size_t offset, size_t count, void *data);
    int write(size_t offset, size_t count, void *data)
    {
        printf("%s not implemented\n", __FUNCTION__);
        __asm__("cli;hlt");
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
    uint32_t USBBASE;
    USBBASE = pci_resource_start(dev, 4);
    printf("USBBASE=0x%x\n", USBBASE);
    char controller_name[] = USB_HC_NAME;
    controller_name[USB_HC_COUNTER_OFFSET] += controller_count++;

    auto pUhciCharInterface = new uhci_char_interface(USBBASE, "USB-HC");
    ret = mknod(controller_name, pUhciCharInterface);
    //ret = register_chardev(ataBlkInterface);
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
    {(uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, PCI_BASE_CLASS_SERIAL, PCI_CLASS_SERIAL_USB, PCI_CLASS_SERIAL_USB_UHCI}
};

pci_driver_t uhci_driver_interface = {
    "Universal Host Controller Driver",
    supportedDevices,
    1,
    uhci_probe,
    uhci_remove,
    uhci_suspend,
    uhci_resume
};

void uhci_init()
{
    printf("init USB\n");
    pci_register_driver(&uhci_driver_interface);
}

//  add to module list
MODULT_INIT(uhci_init)