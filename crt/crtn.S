.section .init
    # GCC will fill in here
    popl %ebp
    ret

.section .fini
    # gcc will fill in here
    popl %ebp
    ret
