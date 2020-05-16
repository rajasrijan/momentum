#include <stdint.h>
#include <memory>
#include <kernel/vfs.h>

#define EI_NIDENT 16
#define Elf64_Addr alignas(8) uint64_t
#define Elf64_Off alignas(8) uint64_t
#define Elf64_Half alignas(2) uint16_t
#define Elf64_Word alignas(4) uint32_t
#define Elf64_Xword alignas(8) uint64_t

//File identification
#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
//File class
#define EI_CLASS 4
//Data encoding
#define EI_DATA 5
//File version
#define EI_VERSION 6
//Operating system / ABI identification
#define EI_OSABI 7
//ABI version
#define EI_ABIVERSION 8
//Start of padding bytes
#define EI_PAD 9
//Size of e_ident[]
#define EI_NIDENT 16

#pragma pack(push, 1)
typedef struct
{
    unsigned char e_ident[EI_NIDENT];
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;
    Elf64_Off e_phoff;
    Elf64_Off e_shoff;
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} Elf64_Ehdr;
// Marks an unused section header
#define SHT_NULL 0
//Contains information defined by the program
#define SHT_PROGBITS 1
//Contains a linker symbol table
#define SHT_SYMTAB 2
//Contains a string table
#define SHT_STRTAB 3
//Contains “Rela” type relocation entries
#define SHT_RELA 4
//Contains a symbol hash table
#define SHT_HASH 5
//Contains dynamic linking tables
#define SHT_DYNAMIC 6
//Contains note information
#define SHT_NOTE 7
//Contains uninitialized space; does not occupy any space in the file
#define SHT_NOBITS 8
//Contains “Rel” type relocationentries
#define SHT_REL 9
//Reserved
#define SHT_SHLIB 10
// Contains a dynamic loader symbol table
#define SHT_DYNSYM 11
//Environment-specific use
#define SHT_LOOS 0x60000000
#define SHT_HIOS 0x6FFFFFFF
//Processor-spe
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7FFFFFFF

#define SHF_WRITE 0x01
#define SHF_ALLOC 0x02
#define SHF_EXECINSTR 0x04
#define SHF_MASKOS 0x0F000000
#define SHF_MASKPROC 0xF0000000

typedef struct
{
    Elf64_Word sh_name;
    Elf64_Word sh_type;
    Elf64_Xword sh_flags;
    Elf64_Addr sh_addr;
    Elf64_Off sh_offset;
    Elf64_Xword sh_size;
    Elf64_Word sh_link;
    Elf64_Word sh_info;
    Elf64_Xword sh_addralign;
    Elf64_Xword sh_entsize;
} Elf64_Shdr;

typedef struct
{
    Elf64_Word p_type;
    Elf64_Word p_flags;
    Elf64_Off p_offset;
    Elf64_Addr p_vaddr;
    Elf64_Addr p_paddr;
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz;
    Elf64_Xword p_align;
} Elf64_Phdr;

#pragma pack(pop)

#define PT_NULL 0x00000000
#define PT_LOAD 0x00000001
#define PT_DYNAMIC 0x00000002
#define PT_INTERP 0x00000003
#define PT_NOTE 0x00000004
#define PT_SHLIB 0x00000005
#define PT_PHDR 0x00000006
#define PT_LOOS 0x60000000
#define PT_HIOS 0x6FFFFFFF
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7FFFFFFF

class binary_loader
{
private:
public:
    binary_loader(/* args */);
    ~binary_loader();
    static void load(shared_ptr<vnode> &node);
};