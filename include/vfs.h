/*
JSLK Kernel
Copyright (C) Jacobo Soffer.

First VFS revision */
#ifndef _VFS_H
#define _VFS_H

#include <stdint.h>
#include <string.h>

// For now only files and directories are supported
#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_MOUNTPOINT 0x03
#define FS_UNKNOWN 0x04

struct dirent {
    char name[128];  // File name
    uint32_t ino;    // Inode number
}

struct fs_node;

typedef uint32_t (*read_type_t)(struct fs_node* uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(struct fs_node*, uint8_t, uint8_t);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent* (*readdir_type_t)(struct fs_node*, uint32_t);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*, char* name);

typedef struct fs_node {
    char name[128];  // Filename
    uint32_t mask;   // Permission mask
    uint32_t uid;    // Owning user
    uint32_t gid;    // Owning group
    uint32_t flags;  // Type of file (see defines above)
    uint32_t inode;  // Used by the file system for identification
    uint32_t length; // Length of the file
    uint32_t impl;   // Used to know to which implementation it belongs.
    read_type_t read; 
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
    struct fs_node* ptr;  // Used by mountpoints and symlinks
} fs_node_t;

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(fs_node_t* node, uint8_t read, uint8_t write);
void close_fs(fs_node_t*);
struct dirent* readdir_fs(fs_node_t* node, uint32_t index);
fs_node_t* finddir_fs(fs_node_t* node, char* name);

#endif