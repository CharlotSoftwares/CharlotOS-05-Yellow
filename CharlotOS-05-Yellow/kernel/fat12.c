#include "fat12.h"
#include "../tty.h"
#include <string.h>
#include <stdint.h>

static fat12_boot_sector_t* boot_sector;
static uint8_t* fat_table;
static fat12_dir_entry_t* root_dir;

static uint16_t root_dir_sectors;
static uint16_t fat_size;
static uint16_t first_data_sector;
static uint16_t cluster_count;
static uint16_t bytes_per_sector;
static uint8_t sectors_per_cluster;
static uint16_t root_entry_count;
static uint16_t fat_count;

// Pointer to start of FAT12 filesystem image in memory
static uint8_t* fs_base = 0;

static uint16_t get_fat_entry(uint16_t cluster) {
    // FAT12 entries are 12 bits (1.5 bytes)
    uint32_t fat_offset = cluster + (cluster / 2); // 1.5 * cluster
    uint16_t entry;

    if (cluster & 1) {
        // Odd cluster
        entry = ((fat_table[fat_offset] >> 4) | (fat_table[fat_offset + 1] << 4)) & 0x0FFF;
    } else {
        // Even cluster
        entry = (fat_table[fat_offset] | ((fat_table[fat_offset + 1] & 0x0F) << 8)) & 0x0FFF;
    }
    return entry;
}

// Converts cluster number to absolute sector number
static uint16_t cluster_to_sector(uint16_t cluster) {
    return first_data_sector + ((cluster - 2) * sectors_per_cluster);
}

void fat12_init(void* fs_image_base) {
    fs_base = (uint8_t*)fs_image_base;
    boot_sector = (fat12_boot_sector_t*)fs_base;
    bytes_per_sector = boot_sector->bytes_per_sector;
    sectors_per_cluster = boot_sector->sectors_per_cluster;
    fat_count = boot_sector->fat_count;
    fat_size = boot_sector->fat_size_16;
    root_entry_count = boot_sector->root_entry_count;

    root_dir_sectors = ((root_entry_count * 32) + (bytes_per_sector - 1)) / bytes_per_sector;

    // FAT table location right after reserved sectors
    fat_table = fs_base + (boot_sector->reserved_sector_count * bytes_per_sector);

    // Root dir location is after FAT area
    root_dir = (fat12_dir_entry_t*)(fat_table + (fat_count * fat_size * bytes_per_sector));

    // First data sector = reserved sectors + FATs + root dir
    first_data_sector = boot_sector->reserved_sector_count + (fat_count * fat_size) + root_dir_sectors;
}

void fat12_list_files(void) {
    tty_writes("FAT12 ROOT DIRECTORY:\n");
    for (int i = 0; i < root_entry_count; i++) {
        fat12_dir_entry_t* entry = &root_dir[i];
        if (entry->filename[0] == 0x00) break; // No more files
        if ((uint8_t)entry->filename[0] == 0xE5) continue; // Deleted file
        if (entry->attr == 0x0F) continue; // Long file name entry

        // Build filename string
        char fname[13];
        int len = 0;
        // Copy filename
        for (int j = 0; j < 8; j++) {
            if (entry->filename[j] == ' ') break;
            fname[len++] = entry->filename[j];
        }
        // Copy extension
        if (entry->ext[0] != ' ') {
            fname[len++] = '.';
            for (int j = 0; j < 3; j++) {
                if (entry->ext[j] == ' ') break;
                fname[len++] = entry->ext[j];
            }
        }
        fname[len] = 0;

        tty_writes(" - ");
        tty_writes(fname);
        tty_writes("\n");
    }
}

int fat12_read_file(const char* filename, uint8_t* buffer, int max_size) {
    // Search root dir for filename (8.3 format)
    for (int i = 0; i < root_entry_count; i++) {
        fat12_dir_entry_t* entry = &root_dir[i];
        if (entry->filename[0] == 0x00) break;
        if ((uint8_t)entry->filename[0] == 0xE5) continue;
        if (entry->attr == 0x0F) continue;

        // Build 8.3 filename string to compare
        char entry_name[13];
        int len = 0;
        for (int j = 0; j < 8; j++) {
            entry_name[len++] = entry->filename[j];
        }
        if (entry->ext[0] != ' ') {
            entry_name[len++] = '.';
            for (int j = 0; j < 3; j++) {
                entry_name[len++] = entry->ext[j];
            }
        }
        entry_name[len] = 0;

        if (strcasecmp(filename, entry_name) == 0) {
            // Found the file, read content cluster chain
            uint16_t cluster = entry->first_cluster_low;
            int remaining = entry->file_size;
            int pos = 0;

            while (cluster < 0xFF8) {
                // Read sectors of this cluster
                uint8_t* cluster_addr = fs_base + (cluster_to_sector(cluster) * bytes_per_sector);
                int to_read = (remaining > (sectors_per_cluster * bytes_per_sector)) ? (sectors_per_cluster * bytes_per_sector) : remaining;

                if (pos + to_read > max_size) {
                    // Buffer overflow protection
                    to_read = max_size - pos;
                }

                memcpy(buffer + pos, cluster_addr, to_read);
                pos += to_read;
                remaining -= to_read;

                if (remaining <= 0) break;

                cluster = get_fat_entry(cluster);
                if (cluster == 0xFF7) break; // bad cluster
            }

            return pos; // number of bytes read
        }
    }

    return -1; // Not found
}
