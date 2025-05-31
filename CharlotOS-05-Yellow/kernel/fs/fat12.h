#ifndef FAT12_H
#define FAT12_H

#include <stdint.h>

#define SECTOR_SIZE 512
#define FAT12_CLUSTER_SIZE 512  // Usually one cluster = 1 sector for FAT12

typedef struct {
    uint8_t  jump_boot[3];
    uint8_t  oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t  fat_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t  media;
    uint16_t fat_size_16;  // sectors per FAT
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    // We only parse up to here for FAT12
} __attribute__((packed)) fat12_boot_sector_t;

typedef struct {
    char     filename[8];
    char     ext[3];
    uint8_t  attr;
    uint8_t  reserved;
    uint8_t  create_time_tenth;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
} __attribute__((packed)) fat12_dir_entry_t;

// Initialize FAT12 module, give pointer to loaded FAT12 image in memory
void fat12_init(void* fs_image_base);

// List root directory files
void fat12_list_files(void);

// Read file contents into buffer, return file size or -1 if not found
int fat12_read_file(const char* filename, uint8_t* buffer, int max_size);

#endif
