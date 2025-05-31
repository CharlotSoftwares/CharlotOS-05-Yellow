// kernel/fs/fs.c
#include "fs.h"
#include "fat12.h"  // Your FAT12 low-level driver idk if it works

int fs_init(void) {
    return fat12_init();
}

int fs_read_root_dir(fs_file_t* files, int max_files) {
    return fat12_read_root_dir(files, max_files);
}

int fs_read_file_cluster(uint32_t cluster, void* buffer, uint32_t size) {
    return fat12_read_cluster(cluster, buffer, size);
}
