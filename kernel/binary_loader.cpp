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

#include "binary_loader.h"
#include <algorithm>
#include <arch/x86_64/paging.h>
#include <kernel/multitask.h>
#include <utility>
#include <kernel/logging.h>
#include <kernel/vnode.h>
#include <string.h>

binary_loader::binary_loader()
{
}

binary_loader::~binary_loader()
{
}
const char bitness[][6] = {"32Bit", "64Bit"};
const char endianness[][16] = {"little endian", "big endian"};
const char target_os[][16] = {"System V", "HP-UX", "NetBSD", "Linux", "GNU Hurd", "Solaris", "AIX", "IRIX", "FreeBSD", "Tru64", "Novell Modesto", "OpenBSD", "OpenVMS", "NonStop Kernel", "AROS", "Fenix OS", "CloudABI"};
void printElfHeader(const Elf64_Ehdr &hdr)
{
    log_debug("ELF Header:-\n");
    log_debug("Magic:0x%x, %s, %s, version: [%x], OS [%s], "
              "ABI Version: %x, Entry: %x, Program Header: %x, "
              "Section Header: %x, flags: %x, Program Header size: %x, PH Count: "
              "%x\n",
              *(uint32_t *)hdr.e_ident,
              bitness[(hdr.e_ident[EI_CLASS] - 1) % 2],
              endianness[(hdr.e_ident[EI_DATA] - 1) % 2],
              hdr.e_version,
              target_os[hdr.e_ident[EI_OSABI]],
              hdr.e_ident[EI_ABIVERSION],
              //    hdr.type,
              //    hdr.machine,
              //    hdr.version2,
              hdr.e_entry,
              hdr.e_phoff,
              hdr.e_shoff,
              hdr.e_flags,
              hdr.e_ehsize,
              //    hdr.phentsize,
              hdr.e_phnum
              //   ,
              //    hdr.shentsize,
              //    hdr.shnum,
              //    hdr.shstrndx
    );
}

void printElfProgramHeader(const Elf64_Phdr *pheader)
{
    log_debug("Type: %x, vaddr: %x, paddr: %x, align: %d\n", pheader->p_type, pheader->p_vaddr, pheader->p_paddr, pheader->p_align);
}

void printElfSectionHeader(const Elf64_Shdr *sheader)
{
    log_debug("Type: %x, addr: %x, offset: %x, align: %d ", sheader->sh_type, sheader->sh_addr, sheader->sh_offset, sheader->sh_addralign);
    log_debug("size: %x, flags: %x\n", sheader->sh_size, sheader->sh_flags);
}

int binary_loader::load(shared_ptr<vnode> &node, vector<MemPage> &memory_map, uint64_t &entry_point)
{
    int ret = 0;
    Elf64_Phdr *pheader = nullptr;
    Elf64_Shdr *sheader = nullptr;
    Elf64_Shdr *string_section_hdr = nullptr;
    char *string_table = nullptr;
    Elf64_Ehdr elf_hdr = {};

    vector<MemPage> kernel_memory_map;

    ret = node->bread(0, sizeof(Elf64_Ehdr), (char *)&elf_hdr, nullptr);
    if (ret != 0)
        goto error_exit;
    printElfHeader(elf_hdr);
    // print program header
    pheader = new Elf64_Phdr[elf_hdr.e_phnum];
    ret = node->bread(elf_hdr.e_phoff, elf_hdr.e_phentsize * elf_hdr.e_phnum, (char *)pheader, nullptr);
    if (ret != 0)
        goto error_exit;

    log_debug("ELF Program Header:-\n");
    for (size_t i = 0; i < elf_hdr.e_phnum; i++) {
        printElfProgramHeader(pheader + i);
    }
    // print sections
    sheader = new Elf64_Shdr[elf_hdr.e_shnum];
    ret = node->bread(elf_hdr.e_shoff, elf_hdr.e_shentsize * elf_hdr.e_shnum, (char *)sheader, nullptr);
    if (ret != 0)
        goto error_exit;

    //  read string table
    string_section_hdr = &sheader[elf_hdr.e_shstrndx];
    string_table = new char[string_section_hdr->sh_size];
    ret = node->bread(string_section_hdr->sh_offset, string_section_hdr->sh_size, string_table, nullptr);
    if (ret != 0)
        goto error_exit;

    //  create memory map for process
    // record all the program sections for the executable. Ordered insertion.
    for (size_t i = 0; i < elf_hdr.e_phnum; i++)
        if (pheader[i].p_type == PT_LOAD) {
            MemPage page = {PageManager::roundToPageBoundry(pheader[i].p_vaddr), 0, PageManager::roundToPageSize(pheader[i].p_memsz)};
            auto next_it = lower_bound(memory_map.begin(), memory_map.end(), page, [](const MemPage &a, const MemPage &val) { return val.vaddr < a.vaddr; });
            next_it = memory_map.insert(next_it, page);
        }
    //  sort list of entries
    std::quick_sort(memory_map.begin(), memory_map.end(), [](MemPage &a, MemPage &b) { return a.vaddr < b.vaddr; });
    //  merge page entries
    for (size_t i = 0; i < memory_map.size() - 1; i++) {
        //  merge condition
        if (memory_map[i + 1].vaddr <= memory_map[i].vend()) {
            memory_map[i].size = max(memory_map[i].vend(), memory_map[i + 1].vend()) - memory_map[i].vaddr;
            memory_map.erase(memory_map.begin() + i + 1);
            i--;
        }
    }
    //  allocate user process memory. Map memory to kernel and user process.
    for (MemPage &page : memory_map) {
        MemPage kpage = {};
        kpage.size = page.size;
        ret = PageManager::findFreeVirtualMemory(kpage.vaddr, page.size);
        if (ret < 0) {
            log_error("failed to findFreeVirtualMemory\n");
            asm("cli;hlt");
        }

        if (PageManager::setPageAllocation(kpage.vaddr, kpage.size, PageManager::Privilege::Supervisor, PageManager::PageType::Read_Write)) {
            log_error("Failed to set page address\n");
            asm("cli;hlt");
        }

        if (PageManager::getPhysicalAddress(kpage.vaddr, kpage.paddr)) {
            log_error("Failed to get physical address\n");
            asm("cli;hlt");
        }
        page.paddr = kpage.paddr;
        kernel_memory_map.push_back(kpage);
        //  memset memory for debug purpose
        memset((void *)kpage.vaddr, 0x90, kpage.size);
    }

    // process the section headers
    log_debug("Processing ELF Section Headers:-\n");
    for (size_t i = 0; i < elf_hdr.e_shnum; i++) {
        Elf64_Shdr section_header = sheader[i];
        log_debug("Section: %s\n", &string_table[section_header.sh_name]);
        printElfSectionHeader(&section_header);
        if ((section_header.sh_type & SHT_PROGBITS) && section_header.sh_addr != 0) {
            //  find kernel space mapping.
            auto user_map_it = find_if(memory_map.begin(), memory_map.end(), [&section_header](const MemPage &page) { return page.vinside(section_header.sh_addr); });
            auto kern_map_it = kernel_memory_map.begin() + distance(memory_map.begin(), user_map_it);
            //  load the section from file.
            char *kernel_address = (char *)(kern_map_it->vaddr + (section_header.sh_addr - user_map_it->vaddr));
            ret = node->bread(section_header.sh_offset, section_header.sh_size, kernel_address, nullptr);
            if (ret != 0)
                goto error_exit;
        } else if ((section_header.sh_type & SHT_NOBITS) && section_header.sh_addr != 0) {
            //  find kernel space mapping.
            auto user_map_it = find_if(memory_map.begin(), memory_map.end(), [&section_header](const MemPage &page) { return page.vinside(section_header.sh_addr); });
            auto kern_map_it = kernel_memory_map.begin() + distance(memory_map.begin(), user_map_it);
            //  load the section from file.
            char *kernel_address = (char *)(kern_map_it->vaddr + (section_header.sh_addr - user_map_it->vaddr));
            //  initialize memory
            memset(kernel_address, 0x00, section_header.sh_size);
        }
    }

    //  free the kernel mapping
    for (MemPage &page : kernel_memory_map) {
        ret = PageManager::freeVirtualMemory(page.vaddr, page.size);
        if (ret < 0) {
            log_error("failed to freeVirtualMemory\n");
            asm("cli;hlt");
        }
    }
    entry_point = elf_hdr.e_entry;
error_exit:
    kernel_memory_map.clear();
    delete[] pheader;
    pheader = nullptr;
    delete[] sheader;
    sheader = nullptr;
    return ret;
}
