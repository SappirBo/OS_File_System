#include <stdio.h>
#include <stdlib.h>
#include "file_system.h"

// init for the structs.
struct super_block sb;
struct inode *inodes;
struct block *blocks; 


// initialize new file system.
void create_fd(int size_bytes)
{
    // setting size for the inodes, 10% of the total size.
    int inodes_size = (int) size_bytes/10;
    // Setting size of the block, saving 12 bytes for the super block.
    int blocks_size = size_bytes - ((int) size_bytes/10) - 12;

    sb.num_inodes = (int) inodes_size / sizeof(struct inode);
    sb.num_blocks = (int) blocks_size / sizeof(struct block);
    sb.block_size = sizeof(struct block);


} 


void mount_fs(); // load file system.
void sync_fs(); // write in the file system.