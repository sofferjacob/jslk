.global error
.type error, @function
# Used to panic the loader and show an error message. The error message must be moved to al before calling.
error:
    movl $0x4f524f45, 0xb8000
    movl $0x4f3a4f52, 0xb8004
    movl $0x4f204f20, 0xb8008
    mov %al, 0xb800a
    hlt