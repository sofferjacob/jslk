#include "string.h"
#include <stdint.h>

string::size_t length() {
  size_t len = 0;
  while (this.str) {
    len++;
  }
  return len;
}

string operator=(char* c) {
  this.str = c;
}
