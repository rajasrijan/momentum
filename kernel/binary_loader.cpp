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

#include <utility>
#include "binary_loader.h"
#include <algorithm>
#include <arch/paging.h>
#include <kernel/multitask.h>
#include <utility>
#include <kernel/logging.h>
#include <kernel/vnode.h>
#include <string.h>

using namespace std;

binary_loader::binary_loader()
{
}

binary_loader::~binary_loader()
{
}
const char bitness[][6] = {"32Bit", "64Bit"};
const char endianness[][16] = {"little endian", "big endian"};
const char target_os[][16] = {"System V", "HP-UX",          "NetBSD",  "Linux",   "GNU Hurd",       "Solaris", "AIX",      "IRIX",    "FreeBSD",
                              "Tru64",    "Novell Modesto", "OpenBSD", "OpenVMS", "NonStop Kernel", "AROS",    "Fenix OS", "CloudABI"};

void printElfHeader(const Elf64_Ehdr &hdr)
{
    log_debug("ELF Header:-\n");
    log_debug("Magic:0x%x, %s, %s, version: [%x], OS [%s], "
              "ABI Version: %x, Type: %x, Entry: %x, Program Header: %x, "
              "Section Header: %x, flags: %x, Program Header size: %x, PH Count: "
              "%x, Section Header size: %x, SH Count: %x\n",
              *(uint32_t *)hdr.e_ident, bitness[(hdr.e_ident[EI_CLASS] - 1) % 2], endianness[(hdr.e_ident[EI_DATA] - 1) % 2], hdr.e_version,
              target_os[hdr.e_ident[EI_OSABI]], hdr.e_ident[EI_ABIVERSION], hdr.e_type,
              //    hdr.machine,
              //    hdr.version2,
              hdr.e_entry, hdr.e_phoff, hdr.e_shoff, hdr.e_flags, hdr.e_ehsize,
              //    hdr.phentsize,
              hdr.e_phnum, hdr.e_shentsize, hdr.e_shnum
              //   ,
              //    hdr.shstrndx
    );
}

void printElfProgramHeader(const Elf64_Phdr &pheader)
{
    // log_debug("Type: %x, vaddr: %llx, paddr: %llx, align: %lld\n", pheader.p_type, pheader.p_vaddr, pheader.p_paddr,
    // pheader.p_align);
    log_debug("    off    0x%016llx vaddr 0x%016llx paddr 0x%016llx align %d\n", pheader.p_offset, pheader.p_vaddr, pheader.p_paddr, pheader.p_align);
    log_debug("    filesz 0x%016llx memsz 0x%016llx flags rwx\n", pheader.p_filesz, pheader.p_memsz);
}

void printElfSectionHeader(const Elf64_Shdr &sheader)
{
    log_debug("Type: %x, addr: %llx, offset: %llx, align: %lld, ", sheader.sh_type, sheader.sh_addr, sheader.sh_offset, sheader.sh_addralign);
    log_debug("size: %llx, flags: %llx, ", sheader.sh_size, sheader.sh_flags);
    log_debug("ent size: %llx\n", sheader.sh_entsize);
}

int binary_loader::read_elf64_hdr(shared_ptr<vnode> &node, Elf64_Ehdr &elf_hdr)
{
    int ret = 0;

    ret = node->bread(0, sizeof(Elf64_Ehdr), (char *)&elf_hdr, nullptr);
    if (ret != 0)
    {
        log_error("Failed to read ELF64 header.\n");
        return ret;
    }
    printElfHeader(elf_hdr);
    return ret;
}

int binary_loader::read_elf64_prog_hdrs(shared_ptr<vnode> &node, const Elf64_Ehdr &elf_hdr, vector<Elf64_Phdr> &pheader)
{
    int ret = 0;
    if (elf_hdr.e_phentsize != sizeof(Elf64_Phdr))
    {
        printf("Size of program header does not match!\n");
        return -EUNK;
    }
    pheader.resize(elf_hdr.e_phnum);
    ret = node->bread(elf_hdr.e_phoff, sizeof(Elf64_Phdr) * pheader.size(), (char *)pheader.data(), nullptr);
    if (ret < 0)
    {
        printf("Failed to read elf file!\n");
        return ret;
    }
    return ret;
}

int binary_loader::read_elf64_sect_hdrs(shared_ptr<vnode> &node, const Elf64_Ehdr &elf_hdr, vector<Elf64_Shdr> &sheader)
{
    int ret = 0;
    if (elf_hdr.e_shentsize != sizeof(Elf64_Shdr))
    {
        printf("Size of section header does not match!\n");
        return -EUNK;
    }
    sheader.resize(elf_hdr.e_shnum);
    ret = node->bread(elf_hdr.e_shoff, sizeof(Elf64_Shdr) * sheader.size(), (char *)sheader.data(), nullptr);
    if (ret < 0)
    {
        printf("Failed to read elf file!\n");
        return ret;
    }
    return ret;
}
int binary_loader::read_elf64_section(shared_ptr<vnode> &node, const Elf64_Shdr &sect_hdr, char *data)
{
    int ret = 0;

    // log_debug("Reading section: offset %llx, size %llx\n", sect_hdr.sh_offset, sect_hdr.sh_size);

    ret = node->bread(sect_hdr.sh_offset, sect_hdr.sh_size, data, nullptr);
    if (ret < 0)
    {
        printf("Failed to read elf file!\n");
        return ret;
    }

    return ret;
}

int binary_loader::load(shared_ptr<vnode> &node, vector<MemPage> &memory_map, uint64_t &entry_point)
{

    int ret = 0;
    Elf64_Ehdr elf_hdr = {};
    vector<Elf64_Phdr> pheader;
    vector<Elf64_Shdr> sheader;
    vector<char> string_table;
    vector<MemPage> kernel_memory_map;

    ret = read_elf64_hdr(node, elf_hdr);
    if (ret != 0)
        goto error_exit;

    log_debug("ELF Program Header:-\n");
    ret = read_elf64_prog_hdrs(node, elf_hdr, pheader);
    if (ret != 0)
        goto error_exit;

    for (size_t i = 0; i < elf_hdr.e_phnum; i++)
    {
        printElfProgramHeader(pheader[i]);
    }

    // print sections
    ret = read_elf64_sect_hdrs(node, elf_hdr, sheader);
    if (ret != 0)
        goto error_exit;

    //  read string table
    string_table.resize(sheader[elf_hdr.e_shstrndx].sh_size);
    ret = read_elf64_section(node, sheader[elf_hdr.e_shstrndx], string_table.data());
    if (ret != 0)
        goto error_exit;

    //  create memory map for process
    // record all the program sections for the executable. Ordered insertion.
    for (size_t i = 0; i < elf_hdr.e_phnum; i++)
        if (pheader[i].p_type == PT_LOAD)
        {
            MemPage page = {};
            page.vaddr = PageManager::round_down_to_pagesize(pheader[i].p_vaddr);
            page.paddr = 0;
            page.size = PageManager::round_up_to_pagesize(pheader[i].p_memsz);

            auto next_it = lower_bound(memory_map.begin(), memory_map.end(), page, [](const MemPage &a, const MemPage &val) { return val.vaddr < a.vaddr; });
            next_it = memory_map.insert(next_it, page);
        }
    //  sort list of entries
    std::quick_sort(memory_map.begin(), memory_map.end(), [](MemPage &a, MemPage &b) { return a.vaddr < b.vaddr; });
    //  merge page entries
    for (size_t i = 0; i < memory_map.size() - 1; i++)
    {
        //  merge condition
        if (memory_map[i + 1].vaddr <= memory_map[i].vend())
        {
            memory_map[i].size = max(memory_map[i].vend(), memory_map[i + 1].vend()) - memory_map[i].vaddr;
            memory_map.erase(memory_map.begin() + i + 1);
            i--;
        }
    }
    //  allocate user process memory. Map memory to kernel and user process.
    for (MemPage &page : memory_map)
    {
        MemPage kpage = {};
        kpage.size = page.size;
        ret = PageManager::findFreeVirtualMemory(kpage.vaddr, page.size);
        if (ret < 0)
        {
            log_error("failed to findFreeVirtualMemory\n");
            asm("cli;hlt");
        }

        if (PageManager::setPageAllocation(kpage.vaddr, kpage.size, PageManager::Privilege::Supervisor, PageManager::PageType::Read_Write))
        {
            log_error("Failed to set page address\n");
            asm("cli;hlt");
        }

        if (PageManager::getPhysicalAddress(kpage.vaddr, kpage.paddr))
        {
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
    for (size_t i = 0; i < elf_hdr.e_shnum; i++)
    {
        const Elf64_Shdr &section_header = sheader[i];
        log_debug("Section: %s\n", &string_table[section_header.sh_name]);
        printElfSectionHeader(section_header);
        if ((section_header.sh_type & SHT_PROGBITS) && section_header.sh_addr != 0)
        {
            //  find kernel space mapping.
            auto user_map_it = find_if(memory_map.begin(), memory_map.end(),
                                       [&section_header](const MemPage &page) { return page.vinside(section_header.sh_addr); });
            auto kern_map_it = kernel_memory_map.begin() + distance(memory_map.begin(), user_map_it);
            //  load the section from file.
            char *kernel_address = (char *)(kern_map_it->vaddr + (section_header.sh_addr - user_map_it->vaddr));
            ret = node->bread(section_header.sh_offset, section_header.sh_size, kernel_address, nullptr);
            if (ret != 0)
                goto error_exit;
        }
        else if ((section_header.sh_type & SHT_NOBITS) && section_header.sh_addr != 0)
        {
            //  find kernel space mapping.
            auto user_map_it = find_if(memory_map.begin(), memory_map.end(),
                                       [&section_header](const MemPage &page) { return page.vinside(section_header.sh_addr); });
            auto kern_map_it = kernel_memory_map.begin() + distance(memory_map.begin(), user_map_it);
            //  load the section from file.
            char *kernel_address = (char *)(kern_map_it->vaddr + (section_header.sh_addr - user_map_it->vaddr));
            //  initialize memory
            memset(kernel_address, 0x00, section_header.sh_size);
        }
    }

    //  free the kernel mapping
    for (MemPage &page : kernel_memory_map)
    {
        ret = PageManager::freeVirtualMemory(page.vaddr, page.size);
        if (ret < 0)
        {
            log_error("failed to freeVirtualMemory\n");
            asm("cli;hlt");
        }
    }
    entry_point = elf_hdr.e_entry;
error_exit:
    kernel_memory_map.clear();
    return ret;
}

int binary_loader::load_kernel(shared_ptr<vnode> &node, vector<MemPage> &memory_map, uint64_t &entry_point, vector<symbol_info> &symbol_table)
{
    int ret = 0;
    Elf64_Ehdr elf_hdr = {};
    vector<Elf64_Phdr> pheader;
    vector<Elf64_Shdr> sheader;
    vector<char> string_table;
    vector<char> sym_strtab;

    ret = read_elf64_hdr(node, elf_hdr);
    if (ret != 0)
        goto error_exit;

    log_debug("Program Header:\n");
    ret = read_elf64_prog_hdrs(node, elf_hdr, pheader);
    if (ret != 0)
        goto error_exit;

    for (size_t i = 0; i < elf_hdr.e_phnum; i++)
    {
        printElfProgramHeader(pheader[i]);
    }

    // print sections
    ret = read_elf64_sect_hdrs(node, elf_hdr, sheader);
    if (ret != 0)
        goto error_exit;

    //  read string table
    if (elf_hdr.e_shstrndx != SHN_UNDEF)
    {
        log_info("String table is at idx %d\n", elf_hdr.e_shstrndx);
        string_table.resize(sheader[elf_hdr.e_shstrndx].sh_size + 1);
        memset(string_table.data(), 0, string_table.size());
        ret = read_elf64_section(node, sheader[elf_hdr.e_shstrndx], string_table.data());
        if (ret != 0)
            goto error_exit;
    }
    else
        log_info("String table section not found!\n");

    // read symbol name string table
    for (const auto &section_header : sheader)
    {
        if (!strcmp(&(string_table[section_header.sh_name]), ".strtab"))
        {
            sym_strtab = vector<char>(section_header.sh_size);
            read_elf64_section(node, section_header, sym_strtab.data());
            log_info("Read symbol name string table\n");
            break;
        }
    }

    //  create memory map for process
    // record all the program sections for the executable.
    for (size_t i = 0; i < elf_hdr.e_phnum; i++)
        if (pheader[i].p_type == PT_LOAD)
        {
            MemPage page = {};
            page.vaddr = pheader[i].p_vaddr;
            page.paddr = pheader[i].p_paddr;
            page.size = pheader[i].p_memsz;
            log_debug("page.vaddr=[%llx],page.paddr=[%llx],page.size=[%llx]\n", page.vaddr, page.paddr, page.size);
            memory_map.emplace_back(page);
        }

    //  allocate user process memory. Map memory to kernel and user process.
    for (MemPage &page : memory_map)
    {
        //  try to allocate exact pages
        ret = PageManager::setPageAllocation(page.paddr, page.size, PageManager::Privilege::Supervisor, PageManager::PageType::Read_Write);
        if (ret)
        {
            log_warning("Failed to allocate exact physical pages.\n");
            asm("cli;hlt");
        }
        //  memset memory for debug purpose
        memset((void *)page.paddr, 0x90, page.size);
    }

    // print the section headers
    log_debug("Section Headers:\n");
    for (const Elf64_Shdr &section_header : sheader)
    {
        log_debug("Section: %s, ", &(string_table[section_header.sh_name]));
        printElfSectionHeader(section_header);
    }
    // process sections
    log_debug("Copying Section to memory\n");
    for (const Elf64_Shdr &section_header : sheader)
    {
        uint64_t addr = 0;

        //  find nearest memory
        get_physical_address(section_header.sh_addr, memory_map, addr);

        if ((section_header.sh_flags & SHF_ALLOC) &&
            ((section_header.sh_type == SHT_PROGBITS) || (section_header.sh_type == SHT_INIT_ARRAY) || (section_header.sh_type == SHT_FINI_ARRAY)))
        {
            log_debug("Loading %s into memory %llx\n", &(string_table[section_header.sh_name]), addr);
            ret = read_elf64_section(node, section_header, (char *)addr);
            if (ret != 0)
                goto error_exit;
        }
        else if ((section_header.sh_flags & SHF_ALLOC) && (section_header.sh_type == SHT_NOBITS))
        {
            log_debug("Zeroing %s at memory %llx\n", &(string_table[section_header.sh_name]), addr);
            memset((char *)addr, 0, section_header.sh_size);
        }
        else if (section_header.sh_type == SHT_SYMTAB)
        {
            vector<Elf64_Sym> symtab(section_header.sh_size / section_header.sh_entsize);
            symbol_table = vector<symbol_info>(section_header.sh_size / section_header.sh_entsize);
            read_elf64_section(node, section_header, (char *)symtab.data());
            log_info("Read %d symbols\n", symtab.size());
            for (size_t i = 0; i < symbol_table.size(); i++)
            {
                auto &symbol = symbol_table[i];
                const auto &elfsym = symtab[i];
                symbol.name = strdup(&(sym_strtab[elfsym.st_name]));
                symbol.addr = elfsym.st_value;
                symbol.size = elfsym.st_size;
            }
        }
    }
    entry_point = elf_hdr.e_entry;
error_exit:
    return ret;
}

int binary_loader::get_physical_address(uint64_t vaddr, const vector<MemPage> &memory_map, uint64_t &paddr)
{
    paddr = 0;
    for (const MemPage &page : memory_map)
    {
        if (page.vinside(vaddr))
        {
            paddr = (vaddr - page.vaddr) + page.paddr;
            return 0;
        }
    }
    return -ENOENT;
}

void *binary_loader::get_symbol_by_name(const char *symbol_name, const vector<symbol_info> &symbol_table, const vector<MemPage> &memory_map,
                                        size_t *symbol_size)
{
    auto symbol_it = find_if(symbol_table.begin(), symbol_table.end(), [symbol_name](const symbol_info &symbol) { return !strcmp(symbol.name, symbol_name); });
    if (symbol_it == symbol_table.end())
    {
        log_error("Symbol [%s] not found\n", symbol_name);
        return nullptr;
    }
    uint64_t physical_address = 0;
    if (symbol_size)
        *symbol_size = symbol_it->size;
    get_physical_address(symbol_it->addr, memory_map, physical_address);
    log_info("Found symbol [%s] at vaddr [%llx] of size [%lld]\n", symbol_it->name, symbol_it->addr, symbol_it->size);
    return (void *)physical_address;
}
