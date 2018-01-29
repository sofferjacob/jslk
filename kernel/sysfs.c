#include <stdint.h>
#include <vfs.h>

const SYSFS_MAX_FILES = 3;  // 0 - root, 1 - stdio, 3 - stdout

char stdin[512];
char stdout[512];

typedef struct {
    char* buf;
    uint8_t max_len;
} file_ptr_t;

file_ptr_t vfiles[SYSFS_MAX_FILES];

FILE* sysfs_root;  // Root directory

static uint32_t sysfs_read(FILE* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    node->eof = 0;
    file_ptr_t fptr = vfiles[node->ino];
    if (offset > fptr.max_len) return 0;
    if (size + offset >= fptr.max_len) {
        size = fptr.max_len - offset;
    } 
    memcpy(buffer, fptr.buf, size);
    if (size == fptr.max_len) {
        node->eof = 1;
    }
    return size;
}

static uint32_t sysfs_write(FILE *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    file_ptr_t fptr = vfiles[node->ino];
    if (size + offset >= fptr.max_len)
    {
        size = fptr.max_len;
    }
    memcpy(buffer, fptr.buf, size);
    if (size == fptr.max_len)
    {
        node->eof = 1;
    }
    return size;
}

void startSysFS() {
    VOLUME* sysfs = kmalloc(sizeof(VOLUME));

}