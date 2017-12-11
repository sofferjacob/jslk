#ifndef fs_h
#define fs_h

#include <stdint.h>

// The standard file types supported by JSLK
#define FS_FILE 0x01
#define FS_DIR  0x02
#define FS_MOUNTPOINT 0x03

typedef struct fs_node {
    char name[128]; // Name of the node
    uint32_t len;   // Size of file
    uint8_t flags;  // Type of node
    uint32_t ino;   // Used by fs
    char impl;      // Which implementation does it belong to?

};


#endif