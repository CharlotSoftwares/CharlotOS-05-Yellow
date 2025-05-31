// kernel/fs/fs.h
#ifndef FS_H
#define FS_H

#include <stdint.h>

// File info struct (simplified)
typedef struct {
    char name[12];    // 8.3 filename
    uint32_t size;    // file size in bytes
    uint32_t first_cluster; // start cluster
} fs_file_t;

// Initialize filesystem (mount etc) very shitty
int fs_init(void);

// Read directory entries, store max_files files info
int fs_read_root_dir(fs_file_t* files, int max_files);

// Read file data by cluster number
int fs_read_file_cluster(uint32_t cluster, void* buffer, uint32_t size);

#endif // FS_H
