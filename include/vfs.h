#ifndef _fs_h
#define _fs_h

#include <stdint.h>

// File types
#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_MOUNTPOINT 0x03
#define FS_SYMLINK 0x04
#define FS_UNKNOWN 0x05

struct dirent {
    char name[128]; // Filename
    uint32_t ino;   // Inode number
};

struct _FILE;

typedef uint32_t (*read_type_t)(struct _FILE*, uint32_t, uint32_t, uint8_t *);
typedef uint32_t (*write_type_t)(struct _FILE*, uint32_t, uint32_t, uint8_t *);
typedef void (*open_type_t)(struct _FILE*, uint8_t, uint8_t);
typedef void (*close_type_t)(struct _FILE*);
typedef struct dirent *(*readdir_type_t)(struct _FILE*, uint32_t);
typedef struct _FILE *(*finddir_type_t)(struct _FILE *, char* name);

typedef struct {
    char name[15];  // Device / FS name
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
} VOLUME, volume_t;

typedef struct _FILE {
    char name[128];     // Filename
    uint32_t length;    // Length
    uint8_t type;       // File type (see defines above)
    uint32_t ino;       // Inode number
    VOLUME* volume;      // Owning volume
    uint32_t impl;      // Reserved for the implementation
    struct _FILE* ptr;  // Used by mountpoints / symlinks
} FILE, file_t;

extern FILE* root_fs;

// Standarized VFS functions
uint32_t read_fs(FILE* node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(FILE *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(FILE *node, uint8_t read, uint8_t write);
void close_fs(FILE *node);
struct dirent *readdir_fs(FILE *node, uint32_t index);
FILE *finddir_fs(FILE *node, char *name);

#endif