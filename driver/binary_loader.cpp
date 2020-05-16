#include "binary_loader.h"
#include <arch/x86_64/paging.h>
#include <kernel/multitask.h>
#include <utility>
#include <algorithm>

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
    printf("ELF Header:-\n");
    printf("Magic:0x%x, %s, %s, version: [%x], OS [%s], "
           "ABI Version: %x, Entry: %x, Program Header: %x, "
           "Section Header: %x, flags: %x, Program Header size: %x, PH Count: %x\n",
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
    printf("Type: %x, vaddr: %x, paddr: %x, align: %d\n", pheader->p_type, pheader->p_vaddr, pheader->p_paddr, pheader->p_align);
}

void printElfSectionHeader(const Elf64_Shdr *sheader)
{
    printf("Type: %x, addr: %x, offset: %x, align: %d ", sheader->sh_type, sheader->sh_addr, sheader->sh_offset, sheader->sh_addralign);
    printf("size: %x, flags: %x\n", sheader->sh_size, sheader->sh_flags);
}

void binary_loader::load(shared_ptr<vnode> &node)
{
    Elf64_Ehdr elf_hdr = {};
    node->bread(0, sizeof(Elf64_Ehdr), (char *)&elf_hdr, nullptr);
    printElfHeader(elf_hdr);
    // print program header
    Elf64_Phdr *pheader = new Elf64_Phdr[elf_hdr.e_phnum];
    node->bread(elf_hdr.e_phoff, elf_hdr.e_phentsize * elf_hdr.e_phnum, (char *)pheader, nullptr);
    printf("ELF Program Header:-\n");
    for (size_t i = 0; i < elf_hdr.e_phnum; i++)
    {
        printElfProgramHeader(pheader + i);
    }
    // print sections
    Elf64_Shdr *sheader = new Elf64_Shdr[elf_hdr.e_shnum];
    node->bread(elf_hdr.e_shoff, elf_hdr.e_shentsize * elf_hdr.e_shnum, (char *)sheader, nullptr);

    //  read string table
    Elf64_Shdr *string_section_hdr = &sheader[elf_hdr.e_shstrndx];
    char *string_table = new char[string_section_hdr->sh_size];
    node->bread(string_section_hdr->sh_offset, string_section_hdr->sh_size, string_table, nullptr);

    //  create memory map for process
    vector<MemPage> user_memory_map;
    // record all the program sections for the executable. Ordered insertion.
    for (size_t i = 0; i < elf_hdr.e_phnum; i++)
        if (pheader[i].p_type == PT_LOAD)
        {
            MemPage page = {PageManager::roundToPageBoundry(pheader[i].p_vaddr), 0, PageManager::roundToPageSize(pheader[i].p_memsz)};
            auto next_it = lower_bound(user_memory_map.begin(), user_memory_map.end(), page, [](const MemPage &a, const MemPage &val) { return val.vaddr < a.vaddr; });
            next_it = user_memory_map.insert(next_it, page);
        }
    //  merge page entries
    for (int i = 0; i < user_memory_map.size() - 1; i++)
    {
        //  merge condition
        if (user_memory_map[i + 1].vaddr <= user_memory_map[i].vend())
        {
            user_memory_map[i].size = max(user_memory_map[i].vend(), user_memory_map[i + 1].vend()) - user_memory_map[i].vaddr;
            user_memory_map.erase(user_memory_map.begin() + i + 1);
            i--;
        }
    }
    //  allocate user process memory. Map memory to kernel and user process.
    vector<MemPage> kernel_memory_map;
    for (MemPage &page : user_memory_map)
    {
        MemPage kpage = {};
        kpage.size = page.size;
        PageManager::getInstance()->findFreeVirtualMemory(kpage.vaddr, page.size);
        PageManager::getInstance()->setPageAllocation(kpage.vaddr, kpage.size, PageManager::Privilege::Supervisor, PageManager::PageType::Read_Write);
        kpage.paddr = PageManager::getInstance()->getPhysicalAddress(kpage.vaddr);
        page.paddr = kpage.paddr;
        kernel_memory_map.push_back(kpage);
    }

    // process the section headers
    printf("Processing ELF Section Headers:-\n");
    for (size_t i = 0; i < elf_hdr.e_shnum; i++)
    {
        Elf64_Shdr section_header = sheader[i];
        printf("Section: %s\n", &string_table[section_header.sh_name]);
        printElfSectionHeader(&section_header);
        if ((section_header.sh_type & SHT_PROGBITS) && section_header.sh_addr != 0)
        {
            //  find kernel space mapping.
            auto user_map_it = find_if(user_memory_map.begin(), user_memory_map.end(), [&section_header](const MemPage &page) { return page.vinside(section_header.sh_addr); });
            auto kern_map_it = kernel_memory_map.begin() + distance(user_memory_map.begin(), user_map_it);
            //  load the section from file.
            char *kernel_address = (char *)(kern_map_it->vaddr + (section_header.sh_addr - user_map_it->vaddr));
            node->bread(section_header.sh_offset, section_header.sh_size, kernel_address, nullptr);
        }
    }

    //  free the kernel mapping
    for (MemPage &page : kernel_memory_map)
    {
        PageManager::getInstance()->freeVirtualMemory(page.vaddr, page.size);
    }
    kernel_memory_map.clear();
    //  create process
    process_t process = nullptr;
    multitask::getInstance()->createProcess(process, node->getName().c_str(), 3, user_memory_map, elf_hdr.e_entry);
    delete[] pheader;
    pheader = nullptr;
    delete[] sheader;
    sheader = nullptr;
}
