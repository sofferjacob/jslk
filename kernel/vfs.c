#include <vfs.h>

FILE* root_fs = 0;

uint32_t read_fs(FILE* node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    if (node->volume->read != 0) {
        return node->volume->read(node, offset, size, buffer);
    }
    return 0;
}

uint32_t write_fs(FILE* node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    if (node->volume->write != 0) {
        return node->volume->write(node, offset, size, buffer);
    }
    return 0;
}

void open_fs(FILE* node, uint8_t read, uint8_t write) {
    if (node->volume->open != 0) {
        return node->volume->open(node, read, write);
    }
}

void close_fs(FILE* node) {
    if (node->volume->close != 0) {
        return node->volume->close(node);
    }
}

struct dirent *readdir_fs(FILE* node, uint32_t index) {
    if (node->type == FS_DIRECTORY && node->volume->readdir != 0) {
        return node->volume->readdir(node, index);
    }
    return 0;
}

FILE* finddir_fs(FILE* node, char* name) {
    if (node->type == FS_DIRECTORY && node->volume->finddir != 0) {
        return node->volume->finddir(node, name);
    }
    return 0;
}
