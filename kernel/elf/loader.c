#include "loader.h"
#include <hal.h>
#include <stdbool.h>

bool elf_check_file(Elf32_Hdr* hdr) {
    if (!hdr) return false;
    if (hdr->e_ident[EI_MAG0] != ELFMAG0) {
        kprint("** ERROR: Elf header EI_MAG0 incorrect **\n");
        return false;
    }
    if (hdr->e_ident[EI_MAG1] != ELFMAG1) {
        kprint("** ERROR: Elf header EI_MAG1 incorrect **\n");
        return false;
    }
    if (hdr->e_ident[EI_MAG2] != ELFMAG2) {
        kprint("** ERROR: Elf header EI_MAG2 incorrect **\n");
        return false;
    }
    if (hdr->e_ident[EI_MAG3] != ELFMAG3) {
        kprint("** ERROR: Elf header EI_MAG3 incorrect **\n");
        return false;
    }
    return true;
}

bool elf_check_supported(Elf32_Hdr* hdr) {
    if (!hdr) return false;
    if (hdr->e_ident[EI_CLASS] != ELFCLASS32) {
        kprint("** ERROR: Unsupported ELF file class **\n");
        return false;
    }
    if (hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        kprint("**ERROR: Unsupported ELF data order**\n");
        return false;
    }
    if (hdr->e_machine != EM_386) {
        kprint("**ERROR: Unsupported ELF file target** \n");
        return false;
    }
    if (hdr->e_version != EV_CURRENT) {
        kprint("**ERROR: Unsupported ELF file version** \n");
        return false;
    }
    if (hdr->e_type != ET_REL && hdr->e_type != ET_EXEX) {
        kprint("**ERROR: Unsupported ELF file type** \n");
        return false;
    }
    return true;
}

