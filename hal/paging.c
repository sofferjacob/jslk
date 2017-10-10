#include "paging.h"
#include "hal.h"
#include <stdint.h>

int allocFrame(pageEntry_t* page, bool kernelMode, bool rw) {
  if (page->frame != 0) {
    return 1;
  }
  atomicalStart();
  uint32_t physFrame = pmmAllocBlock();
  if (!physFrame) {
    system_panic("Out of memory");
  }
  page->present = 1;
  page->rw = (rw)?1:0;
  page->mode = (kernelMode)?0:1;
  page->frame = physFrame;
  atomicalRelease();
}

int freeFrame(pageEntry_t* page) {
  atomicalStart();
  if (page->frame == 0) {
    // Page wasn't allocated
    atomicalRelease();
    return 0;
  }
  pmmFreeBlock(page->frame);
  page->frame = 0x0;
  atomicalRelease();
}
