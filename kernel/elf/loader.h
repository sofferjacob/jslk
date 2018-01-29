#ifndef _ELF_LOADER_H
#define _ELF_LOADER_H

#include <stdint.h>

// ELF Data Types
typedef uint16_t Elf32_Half;  // Unsigned half-int
typedef uint32_t Elf32_Off;   // Unsigned offset
typedef uint32_t Elf32_Addr;  // Unsigned address
typedef uint32_t Elf32_Word;  // Unsigned word (int)
typedef int32_t Elf32_Sword;  // Signed word (int)

// ELF Header
#define ELF_NIDENT 16

typedef struct {
    uint8_t e_ident[ELF_NIDENT];
    Elf32_Half e_type;  // 32 or 64 bit
    Elf32_Half e_machine;  // Little or big endian
    Elf32_Word e_version;  // ELF version
    Elf32_Addr e_entry;    // Entry point
    Elf32_Off  e_phoff;    // section header offset
    Elf32_Word e_flags;    // Flags
    Elf32_Half e_hsize;    // Header Size
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Hdr;

enum Elf_Ident {
    EI_MAG0 = 0,  // 0x7f
    EI_MAG1 = 1,  // 'E'
    EI_MAG2 = 2,  // 'L'
    EI_MAG3 = 3,  // 'F'
    EI_CLASS = 4, // Architecture (32/64)
    EI_DATA = 5,  // Byte order
    EI_VERSION = 6, // ELF Version
    EI_OSABI = 7,
    EI_ABIVERSION = 8,
    EI_PAD = 9;
};

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ELFDATA2LSB (1)  // Little endian
#define ELFCLASS32  (1)  // 32 bit architecture

enum Elf_Type {
    ET_NONE = 0,  // Unknown
    ET_REL = 1,   // Reloacatable
    ET_EXEC = 3,  // Executable
};

#define EM_386  (3)     // x86 machine
#define EV_CURRENT (1)  // Curremt elf version

#endif