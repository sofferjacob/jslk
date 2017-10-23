#ifndef _x86_gdt_h
#define _x86_gdt_h

#include <stdint.h>

/* -------------------------------------------------------------------------------------------
                                        GDT STRUCTURES
* gdt_descriptor: Skeleton for a GDT Descriptor (entry).
* gdtr: Struct used to tell the CPU where the GDT is located.
------------------------------------------------------------------------------------------- */
struct gdt_descriptor
{
    // GDT Limit (Bits 0-15)
    uint16_t limit_low;
    // GDT Base Low-Middle (Bits 16-39)
    uint16_t baseLow;
    uint8_t baseMiddle;
    // Flags (Gran and Access) (Bits 40-55)
    uint8_t flags;
    uint8_t gran;
    // Base High (Bits 56-63)
    uint8_t baseHigh;
} __attribute__((packed));

typedef struct gdt_descriptor gdt_descriptor_t;

struct gdtptr
{
    uint32_t base;
    uint16_t limit;
} __attribute__((packed));
typedef struct gdtptr gdtptr_t;

/* -------------------------------------------------------------------------------------------
                                        GDT Flags Typenames
Access Flags: 0x0001 - 0x0080
Granularity Flags: The rest
------------------------------------------------------------------------------------------- */
#define GDT_DESC_ACCESS 0x0001        // Set access bit
#define GDT_DESC_RW 0x0002            // Descriptor is readable and writeable.
#define GDT_DESC_EXPANSION 0x0004     // Expansion Direction Bit
#define GDT_DESC_EXECUTABLE 0x0008    // Set descriptor to be executable
#define GDT_DESC_TYPE_CODEDATA 0x0010 // Descriptor type: code or data.
#define GDT_DESC_DPL 0x0060           // Descriptor DPL
#define GDT_DESC_INMEM 0x0080         // Is segment in memory?
#define GDT_DESC_LIMITHI 0x0f         // High 4 bits of limit
#define GDT_DESC_OS 0x10              // Reserved for OS
#define GDT_DESC_32BIT 0x40           // If set segment is 32 bit. Else is 16 bit.
#define GDT_GRAN_4K 0x80              // Default is none.

#define GDT_KCODE32_COMMON 0x9A
#define GDT_KDATA32_COMMON 0x92
#define GDT_UCODE32_COMMON 0xFA
#define GDT_UDATA32_COMMON 0xF2
#define GDT32_GRANCOMMON 0xCF

// Type used to check availability of segments / interrupts
enum somestat
{
    available,
    taken,
};
typedef enum somestat status_t;

#endif