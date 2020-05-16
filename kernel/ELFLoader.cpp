/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include <string.h>
#include <string>
#include "ELFLoader.h"
#include "vfs.h"
#include "errno.h"
#include <utility>
#include <algorithm>

#define EI_MAG0 0		//File identification
#define EI_MAG1 1		//File identification
#define EI_MAG2 2		//File identification
#define EI_MAG3 3		//File identification
#define EI_CLASS 4		//File class
#define EI_DATA 5		//Data encoding
#define EI_VERSION 6	//File version
#define EI_OSABI 7		//Operating system / ABI identification
#define EI_ABIVERSION 8 //ABI version
#define EI_PAD 9		//Start of padding bytes
#define EI_NIDENT 16	//Size of e_ident[]

#define ELFCLASSNONE 0 //Invalid class
#define ELFCLASS32 1   //32 - bit objects
#define ELFCLASS64 2   //64 - bit objects

#define ELFDATANONE 0 //Invalid data encoding
#define ELFDATA2LSB 1 //See below
#define ELFDATA2MSB 2 //See below

#define ET_NONE 0		 //No file type
#define ET_REL 1		 //Relocatable file
#define ET_EXEC 2		 //Executable file
#define ET_DYN 3		 //Shared object file
#define ET_CORE 4		 //Core file
#define ET_LOOS 0xfe00	 //Operating system - specific
#define ET_HIOS 0xfeff	 //Operating system - specific
#define ET_LOPROC 0xff00 //Processor - specific
#define ET_HIPROC 0xffff //Processor - specific

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_TLS 7
#define PT_LOOS 0x60000000
#define PT_HIOS 0x6fffffff
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7fffffff

#define DEFINE_STRING(x) \
	{                    \
		x, #x            \
	}

pair<uint32_t, const char *> elf_type[] = {
	{ET_NONE, "No file type"},
	{ET_REL, "Relocatable file"},
	{ET_EXEC, "Executable file"},
	{ET_DYN, "Shared object file"},
	{ET_CORE, "Core file"},
	{ET_LOOS, "Operating system - specific"},
	{ET_HIOS, "Operating system - specific"},
	{ET_LOPROC, "Processor - specific"},
	{ET_HIPROC, "Processor - specific"}};
pair<uint32_t, const char *> program_type[] = {
	DEFINE_STRING(PT_NULL),
	DEFINE_STRING(PT_LOAD),
	DEFINE_STRING(PT_DYNAMIC),
	DEFINE_STRING(PT_INTERP),
	DEFINE_STRING(PT_NOTE),
	DEFINE_STRING(PT_SHLIB),
	DEFINE_STRING(PT_PHDR),
	DEFINE_STRING(PT_TLS),
	DEFINE_STRING(PT_LOOS),
	DEFINE_STRING(PT_HIOS),
	DEFINE_STRING(PT_LOPROC),
	DEFINE_STRING(PT_HIPROC)};
struct Elf32_Phdr
{
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
	void printHeader()
	{
		auto type = find_if(program_type, program_type + (sizeof(program_type) / sizeof(program_type[0])), [&](pair<uint32_t, const char *> e) { return e.first == p_type; });
		if (type == (program_type + (sizeof(program_type) / sizeof(program_type[0]))))
			return;
		printf("\nType [%s]", type->second);
	}
} __attribute__((packed));

struct Elf32_Ehdr
{
	unsigned char e_ident[EI_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shtrndx;
	bool verifyHeader()
	{
		if (memcmp(e_ident, "\x7f"
							"ELF",
				   4))
			return false;
		printf("\nElf signature present");
		switch (e_ident[EI_CLASS])
		{
		case ELFCLASSNONE:
			return false;
		case ELFCLASS32:
			printf("\n32 bit binary");
			break;
		case ELFCLASS64:
			printf("\n64 bit binary");
			break;
		default:
			return false;
		}
		switch (e_ident[EI_DATA])
		{
		case ELFDATANONE:
			return false;
		case ELFDATA2LSB:
			printf("\nLSB data");
			break;
		case ELFDATA2MSB:
			printf("\nMSB data");
			break;
		default:
			return false;
		}
		auto type = find_if(elf_type, elf_type + (sizeof(elf_type) / sizeof(elf_type[0])), [&](pair<uint32_t, const char *> e) { return e.first == e_type; });
		if (type == (elf_type + (sizeof(elf_type) / sizeof(elf_type[0]))))
			return false;
		printf("\nType [%s]", type->second);

		return true;
	}
};

typedef struct
{
	unsigned char e_ident[EI_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shtrndx;
} Elf64_Ehdr;

ELFLoader::ELFLoader()
{
}

ELFLoader::~ELFLoader()
{
}

int ELFLoader::loadFile(std::string filePath, ELFFile &elfFile)
{
	int elfFd = open(filePath);
	if (elfFd < 0)
		return -ENOFILE;
	Elf32_Ehdr elfHeader;
	read(elfFd, (char *)&elfHeader, sizeof(elfHeader));
	if (!elfHeader.verifyHeader())
		return ENOEXEC;
	if (elfHeader.e_phoff != 0)
	{
		if (elfHeader.e_phentsize != sizeof(Elf32_Phdr))
			return ENOEXEC;
		vector<Elf32_Phdr> programHdr(elfHeader.e_phnum);
		fseek(elfFd, elfHeader.e_phoff, SEEK_SET);
		read(elfFd, (char *)programHdr.data(), elfHeader.e_phentsize * elfHeader.e_phnum);
		for (size_t i = 0; i < programHdr.size(); i++)
			programHdr[i].printHeader();
	}
	else
		printf("\nNo program header.");
	return 0;
}