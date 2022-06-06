#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_system.h"

// init for the structs.
struct super_block sb;
struct inode *inodes;
struct block *blocks; 


/**
 * @brief Create a File Descriptor
 *        Need to change the name to "mymkfs".
 * @param size_bytes -> size of the fd we create (in bytes).
 */
void create_fd(int size_bytes)
{
    // setting size for the inodes, 10% of the total size.
    int inodes_size = (int) size_bytes/10;
    // Setting size of the block, saving 12 bytes for the super block.
    int blocks_size = size_bytes - ((int) size_bytes/10) - 12;

    sb.num_inodes = (int) inodes_size / sizeof(struct inode);
    sb.num_blocks = (int) blocks_size / sizeof(struct block);
    sb.block_size = sizeof(struct block);

    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for(i=0; i<sb.num_inodes; i++){
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name,"");
    } // init inodes.

    blocks = malloc(sizeof(struct block) * sb.num_blocks);
    for(i=0; i<sb.num_blocks; i++){
        blocks[i].next_block = -1;
    } // init blocks.

} 


void mount_fs(); // load file system.

// write in the file system.
void sync_fs(); 