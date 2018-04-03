#include <hal.h>
#include <stdint.h>
#include <string.h>

extern page_directory_t* current_directory;
extern uint32_t initial_esp;

void move_stack(void* new_stack_start, uint32_t size) {
    uint32_t i = 0;
    // Allocate space for the new stack
    for (i = (uint32_t)new_stack_start; i <= (uint32_t)new_stack_start-size; i -= 0x1000) {
        alloc_frame(get_page(i, 1, current_directory), 0, 1);
    }
    // Flush the TLB
    uint32_t pd_addr;
    asm volatile ("mov %%cr3, %0" : "=r" (pd_addr));
    asm volatile ("mov %0, %%cr3" : : "r" (pd_addr));
    // Get an offset to the stack
    uint32_t old_stack_pointer; asm volatile ("mov %%esp, %0" : "=r" (old_stack_pointer));
    uint32_t old_base_pointer; asm volatile ("mov %%ebp, %0" : "=r" (old_base_pointer));
    uint32_t offset = (uint32_t)new_stack_start - initial_esp;
    uint32_t new_stack_pointer = old_stack_pointer + offset;
    uint32_t new_base_pointer = old_base_pointer + offset;
    // Copy the stack
    memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);
    for (i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4) {
        uint32_t tmp = *(uint32_t*)i;
        if ((old_stack_pointer < tmp) && (tmp < initial_esp)) {
            tmp = tmp + offset;
            uint32_t* tmp2 = (uint32_t*)i;
            *tmp2 = tmp;
        }
    }
    asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
    asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}