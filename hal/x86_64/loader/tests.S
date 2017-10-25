#include "loader.h"

.global checkCPUID
.type checkCPUID, @function
checkCPUID:
    pushf
    pop %eax
    mov %eax, %ecx
    xor 1<<21, %eax
    push %eax
    popf
    pushf
    pop %eax
    push %ecx
    popf
    cmp %ecx, %eax
    je .no_cpuid
    ret

.type .no_cpuid, @function
.no_cpuid:
    mov ERR_CODE1, %al
    jmp error