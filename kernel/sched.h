#ifndef _sched_h
#define _sched_h

#include <stdint.h>
#include <hal.h>

#define MAX_THREADS 5

/* JSLK Scheduler
* I will start with a simple 
* Round Robin FIFO scheduler
* with 2 task states and a 
* very simple PCB and will add
* stuff so it evolves into a more
* complex one with time.
*/

// Possible task states
typedef enum _proc_state {
    running = 0,
    not_running = 1,
    // ready_nr
    // blocked_nr
    // suspended
} proc_state, proc_state_t;

typedef enum {
    kernel = 0,
    user = 1,
} proc_type_t;

typedef struct {
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t edi;
    uint32_t esi;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t flags;
} task_tegisters, task_registers_t;

typedef struct {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t cs;
    uint32_t flags;
} stack_frame, stack_frame_t;

struct _thread;
typedef struct _thread thread, thread_t;

typedef struct _process {
    uint32_t id;
    uint32_t priority;
    page_directory_t* pdir;
    thread threads[MAX_THREADS];  // will switch for a linked list later.
    proc_state state;
    proc_type_t type;
} process, process_t;

struct _thread {
    process* parent;  // Parent process
    void* initial_stack;
    void* stack_limit;
    void* kernel_stack;
    uint32_t priority;
    proc_state state;
    task_registers_t regs;
};


#endif