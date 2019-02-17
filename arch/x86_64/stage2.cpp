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

#include "global.h"
#include "timer.h"
#include "mm.h"
#include "paging.h"
#include "acpi.h"
#include "apic.h"
#include "stdlib.h"
#include "cpu.h"
#include "keyboard.h"
#include <kernel/multitask.h>
#include "stdio.h"
#include "string.h"
#include <kernel/vfs.h>
#include <kernel/sys_info.h>
#include "pci.h"
#include "../../DDI/pci_driver.h"
#include <DDI/driver.h>
#include <string>
#include "../../DDI/block_driver.h"
#include <list>
#include <map>
#include <driver/binary_loader.h>
#include "descriptor_tables.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <acpi.h>
#ifdef __cplusplus
}
#endif

const char copyright[] = "Momentum Copyright (C) 2009-2018 Srijan Kumar Sharma\n"
						 "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"
						 "This is free software, and you are welcome to redistribute it\n"
						 "under certain conditions; type `show c' for details.\n";

void *t1(void *arg);
void *pnpHotPlug(void *arg);
extern uint32_t stack;
extern "C" uint64_t __CTOR_LIST__;
extern "C" uint64_t __CTOR_END__;
char kernel_cmdline[4096] = {0};

typedef int (*fncmd)(char *);

struct cmd_t
{
	const char *command;
	const function<int(char *)> Cmd;
};

int echo(char *args);
int ver(char *args);
int help(char *args);
int lspci(char *args);
int lsmem(char *args);
int ls(char *args);
int cd(char *args);
int cat(char *filename);
int exec(char *filename);
int poweroff(char *args);

cmd_t cmdMapping[] = {
	{"exec", exec},
	{"cat", cat},
	{"cd", cd},
	{"ls", ls},
	{"lspci", lspci},
	{"lsmem", lsmem},
	{"echo", echo},
	{"poweroff", poweroff},
	{"version", ver},
	{"help", help}};

int cat(char *filename)
{
	int fd = open(filename);
	if (fd == -1)
		return 1;
	char buffer[256] = {0};
	int bytesRead = 0;
	while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytesRead] = 0;
		printf(buffer);
	}
	return 0;
}

int exec(char *filename)
{
	int errorCode = 0;
	shared_ptr<vnode> exec_node;
	errorCode = lookup(filename, exec_node);
	if (errorCode)
		goto error;
	if (!exec_node->isFile())
		return -ENOFILE;
	binary_loader::load(exec_node);
error:
	return errorCode;
}

int cd(char *args)
{
	return chdir(args);
}

int ls(char *args)
{
	vector<string> dirs;
	int fd = openat(FDCWD, args, O_RDONLY | O_NONBLOCK | O_DIRECTORY | O_CLOEXEC, 0);
	getdents(fd, dirs);
	close(fd);
	for (const string &entry : dirs)
	{
		printf("%s\n", entry.c_str());
	}
	return 0;
}

int lspci(char *args)
{
	printf("Connected PCI devices:\n");
	std::vector<pci_device_t> &deviceList = pci_getDevices();
	for (pci_device_t &device : deviceList)
	{
		pci_device_id devId = {0};
		const char *driver_name = nullptr;
		device.getDeviceId(&devId);
		if (device.pDriver && device.pDriver->name)
		{
			driver_name = device.pDriver->name;
		}
		else
		{
			driver_name = "None";
		}
		printf("\tPCI_%x_%x_%x_%x_%x_%x_%x\n", devId.VendorID, devId.DeviceID, devId.SubVendorID, devId.SubSystemID, devId.Class, devId.SubClass, devId.ProgIf);
		printf("\t\tDriver: %s\n", driver_name);
	}
	return 0;
}

int lsmem(char *args)
{
	vector<MemPage> memMap;
	PageManager::getInstance()->getMemoryMap(memMap);
	for (const auto &page : memMap)
	{
		printf("vaddr: [0x%lx], paddr: [0x%lx], size: [0x%lx]\n", page.vaddr, page.paddr, page.size);
	}
	return 0;
}

int echo(char *args)
{
	if (!args)
		return 1;
	printf("%s\n", args);
	return 0;
}

int poweroff(char *args)
{
	AcpiEnterSleepStatePrep(5);
	asm("cli"); // disable interrupts
	AcpiEnterSleepState(5);
	printf("POWEROFF");
	asm("hlt"); // in case it didn't work!
	return 1;
}

int help(char *args)
{
	printf("Supported commands:\n");
	for (size_t i = 0; i < (sizeof(cmdMapping) / sizeof(cmdMapping[0])); i++)
	{
		printf("\t%s <args>\n", cmdMapping[i].command);
	}
	return 0;
}

int decode_commandline_args(const char *cmdline, map<string, string> &args);
int decode_commandline_args(const char *cmdline, map<string, string> &args)
{
	const char *start_ptr = cmdline;
	const char *end_ptr = nullptr;
	while ((end_ptr = strchar(cmdline, ',')))
	{
		char tmp[256] = {0};
		std::copy(start_ptr, end_ptr, tmp);
	}
	return 0;
}

void decode_cmdline(char *cmdLine)
{
	auto arg = strtok(cmdLine, "= ");
	while (arg != nullptr)
	{
		printf(arg);
		if (!stricmp("uuid", arg))
		{
			arg = strtok(nullptr, "= ");
			sys_info.root_drive_uuid = to_uuid(arg);
		}
		arg = strtok(nullptr, "= ");
	}
}

void stage2(multiboot_info *mbi)
{
	if ((mbi->flags & MULTIBOOT_INFO_CMDLINE) && mbi->cmdline != 0)
	{
		char *cmdLine = (char *)(uint64_t)mbi->cmdline;
		strcpy(kernel_cmdline, cmdLine);
		printf("Multiboot CMDLINE [%s]\n", cmdLine);
		decode_cmdline(cmdLine);
	}
	init_idt();
	if (PageManager::getInstance()->initialize())
	{
		printf("Paging initialize failed\n");
		__asm__("cli;hlt;");
	}
	create_kernel_heap();
	initilize_memorymanager(mbi);
	//fix_refferances();;
	if (!get_acpi_tables())
	{
		printf("ACPI table failed\n");
		__asm__("cli;hlt;");
	}
	//init_timer();
	__asm__("cli;");
	init_apic_timer(0x001FFFFF);
	init_keyboard();
	uint64_t noOfConstructors = (uint64_t)(&__CTOR_END__ - &__CTOR_LIST__);
	printf("No of constructors: %x\n", noOfConstructors);
	for (uint64_t i = 0; i < noOfConstructors; i++)
	{
		printf("calling...[%x]\n", (&__CTOR_LIST__)[i]);
		void (*constructor_fn)(void) = (void (*)(void))(&__CTOR_LIST__)[i];
		constructor_fn();
	}
	init_multitask();
	pci_init_devices();
	auto kthread = multitask::getInstance()->getKernelThread();
	change_thread(kthread, true);
}

void *t1(void *arg)
{
	setColor(0x8F);
	const char logo[] = {0x0A, 0x0A, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x5F, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x7C, 0x20, 0x7C, 0x2F, 0x5C, 0x7C, 0x20, 0x7C, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x7C, 0x2F, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x60, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x5C, 0x20, 0x20, 0x2F, 0x5C, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x0A, 0x7C, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x7C, 0x0A, 0x7C, 0x20, 0x2E, 0x20, 0x20, 0x2E, 0x20, 0x7C, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x7C, 0x20, 0x7C, 0x5F, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x5C, 0x20, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x0A, 0x7C, 0x20, 0x7C, 0x5C, 0x2F, 0x7C, 0x20, 0x7C, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x60, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x27, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x60, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x5C, 0x0A, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x5C, 0x20, 0x5C, 0x5F, 0x2F, 0x20, 0x2F, 0x5C, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x0A, 0x5C, 0x5F, 0x7C, 0x20, 0x20, 0x7C, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x0A, 0x00};
	printf(logo);
	setColor(0x0F);
	return 0;
}

int ver(char *args)
{
	t1(nullptr);
	return 0;
}
/*
 * Plug 'n' Play Hardware detector thread.
 */
void *pnpHotPlug(void *arg)
{
	//	PCI device detection loop
	while (true)
	{
		sleep(100);
		printf("Checking for PnP devices\n");
		std::vector<pci_device_t> &deviceList = pci_getDevices();
		for (pci_device_t &device : deviceList)
		{
			if (!device.bIsProcessed)
			{
				pci_find_compitable_driver(device);
			}
		}
	}
	return 0;
}
/*
 * kernel thread
 */
void state_c0()
{
	printf("Kernal thread started\n");
	InitializeFullAcpi();
	init_vfs();
	init_drivers();
	printf("Checking for PnP devices\n");
	std::vector<pci_device_t> &deviceList = pci_getDevices();
	for (pci_device_t &device : deviceList)
	{
		if (!device.bIsProcessed)
		{
			pci_find_compitable_driver(device);
		}
	}
	thread_t pnp_hotplug_thread = nullptr;
	multitask::getInstance()->createKernelThread(pnp_hotplug_thread, "pnp_hotplug_thread", &t1, (void *)0);
	char input[4096];
	sleep(100);
	printf(copyright);
	while (true)
	{
		std::string curDir = getCurrentPath();
		printf("%s$", curDir.c_str());
		gets_s(input, sizeof(input) / sizeof(input[0]));
		char *cmd = input;
		char *subCmd = nullptr;
		for (int i = 0; input[i] != 0; i++)
		{
			if (input[i] == ' ')
			{
				input[i] = 0;
				if (input[i + 1] != 0)
					subCmd = &input[i + 1];
				break;
			}
		}
		bool cmdFound = false;
		for (size_t i = 0; i < (sizeof(cmdMapping) / sizeof(cmdMapping[0])); i++)
		{
			if (!strcmp(cmdMapping[i].command, cmd))
			{
				cmdMapping[i].Cmd(subCmd);
				cmdFound = true;
				break;
			}
		}
		if (!cmdFound)
		{
			printf("Invalid command.\n");
		}
	}
	while (1)
	{
		__asm__("hlt;");
	}
}
