#include "loader.h"

# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.global mboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp
	push %ebx
	# Transfer control to the main kernel.
	# call kernel_main
    movl   $0x2f4b2f4f,0xb8000
	mov    ERR_CODE1,%al
	call error
	# Hang if kernel_main unexpectedly returns.
	.hang: jmp .hang
  call _fini # swap this to be called before the loop with the first release
.size _start, . - _start
