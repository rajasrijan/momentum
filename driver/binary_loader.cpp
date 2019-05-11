#include "binary_loader.h"
#include <arch/x86_64/paging.h>
#include <kernel/multitask.h>
#include <utility>

binary_loader::binary_loader()
{
}

binary_loader::~binary_loader()
{
}
const char birness[][6] = {"32Bit", "64Bit"};
const char endianness[][16] = {"little endian", "big endian"};
const char target_os[][16] = {"System V", "HP-UX", "NetBSD", "Linux", "GNU Hurd", "Solaris", "AIX", "IRIX", "FreeBSD", "Tru64", "Novell Modesto", "OpenBSD", "OpenVMS", "NonStop Kernel", "AROS", "Fenix OS", "CloudABI"};
void printElfHeader(const ELFHeader &hdr)
{
    printf("ELF Header:-\n");
    printf("Magic:0x%x, %s, %s, version: [%x], OS [%s], "
           "ABI Version: %x, Entry: %x, Program Header: %x, "
           "Section Header: %x, flags: %x, Program Header size: %x, PH Count: %x\n",
           hdr.magic,
           birness[(hdr.bitness - 1) % 2],
           endianness[(hdr.endianness - 1) % 2],
           hdr.version,
           target_os[hdr.target_os],
           hdr.abi_version,
           //    hdr.type,
           //    hdr.machine,
           //    hdr.version2,
           hdr.entry,
           hdr.phoff,
           hdr.shoff,
           hdr.flags,
           hdr.ehsize,
           //    hdr.phentsize,
           hdr.phnum
           //   ,
           //    hdr.shentsize,
           //    hdr.shnum,
           //    hdr.shstrndx
    );
}

void printElfProgramHeader(const ProgramHeader *pheader)
{
    printf("Type: %x, vaddr: %x, paddr: %x, align: %d\n", pheader->type, pheader->vaddr, pheader->paddr, pheader->align);
}

void printElfSectionHeader(const SectionHeader *sheader)
{
    printf("Type: %x, addr: %x, offset: %x, align: %d ", sheader->type, sheader->addr, sheader->offset, sheader->addralign);
    printf("size: %x, flags: %x\n", sheader->size, sheader->flags);
}

void binary_loader::load(shared_ptr<vnode> &node)
{
    ELFHeader header = {};
    node->bread(0, sizeof(ELFHeader), (char *)&header, nullptr);
    printElfHeader(header);
    // print program header
    ProgramHeader *pheader = new ProgramHeader[header.phnum];
    node->bread(header.phoff, header.phentsize * header.phnum, (char *)pheader, nullptr);
    printf("ELF Program Header:-\n");
    for (size_t i = 0; i < header.phnum; i++)
    {
        printElfProgramHeader(pheader + i);
    }
    // print sections
    SectionHeader *sheader = new SectionHeader[header.shnum];
    node->bread(header.shoff, header.shentsize * header.shnum, (char *)sheader, nullptr);
    printf("ELF Section Header:-\n");
    for (size_t i = 0; i < header.shnum; i++)
    {
        printElfSectionHeader(sheader + i);
    }
    // process the program headers
    printf("Processing ELF Program Headers:-\n");
    vector<pair<uint64_t, uint64_t>> program;
    for (size_t i = 0; i < header.phnum; i++)
    {
        if (pheader[i].type == PT_LOAD)
        {
            auto v_start = (pheader[i].vaddr / PageManager::PAGESIZE) * PageManager::PAGESIZE;
            auto v_end = ((pheader[i].vaddr + pheader[i].memsz + PageManager::PAGESIZE - 1) / PageManager::PAGESIZE) * PageManager::PAGESIZE;
            if (PageManager::getInstance()->setPageAllocation(v_start, v_end - v_start, PageManager::User, PageManager::Read_Write))
            {
                printf("Failed to allocate pages for program header:\n");
                printElfProgramHeader(pheader + i);
            }
            program.push_back(make_pair<uint64_t, uint64_t>(v_start, v_end - v_start));
            printf("Allocated addr [%x], of size [%x]\n", v_start, v_end - v_start);
            printf("Copying Program section to memory\n");
            int bytes_read = 0;
            node->bread(pheader[i].offset, pheader[i].filesz, (char *)pheader[i].vaddr, &bytes_read);
            printf("[%x] bytes copied from file\n", bytes_read);
        }
        else
        {
            printf("Program Header of this type not supported:\n");
            printElfProgramHeader(pheader + i);
        }
    }
    process_t process = nullptr;
    multitask::getInstance()->createProcess(process, node->getName().c_str(), 3, program, header.entry);
    delete[] pheader;
    pheader = nullptr;
    delete[] sheader;
    sheader = nullptr;
}
