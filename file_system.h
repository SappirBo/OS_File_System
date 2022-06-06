#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once

/**
 * @brief Super Block Struct, it contains 3 parameters:
 *      - num_inodes = amount of inodes.
 *      - num_blocks = number of blocks it contains.
 *      - block_size = size of each block in this super class.
 *      
 *        Size = 12 Bytes -> three Integers.
 */
struct super_block
{
    int num_inodes;
    int num_blocks;
    int block_size;
};

/**
 * @brief  Struct For representing Inode,
 *         It contain name of the file and pointer to the first block.
 *         Size = 16 Bytes (2 Integers and 8 Chars).
 */
struct inode
{
    int size;
    int first_block;
    char name[8];
};

/**
 * @brief Hard Disk Block, 
 *        it has 512 bytes string for data and pointer to the next block.
 *        Size  = 516 bytes (4 bytes for one integer and 512 bytes for 512 chars).
 */
struct block
{
    int next_block;
    char data[512];
};

// Change to mymkfs
void create_fd(int size_bytes); // initialize new file system.
void mount_fs(); // load file system.
void sync_fs(); // write in the file system.