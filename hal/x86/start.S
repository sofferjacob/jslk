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

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	push %esp
	push %ebx
	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kinit

	# Hang if kernel_main unexpectedly returns.
	.hang: jmp .hang
  call _fini # swap this to be called before the loop with the first release
.size _start, . - _start
