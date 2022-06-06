#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once

#define BLOCKSIZE 512

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
    char data[BLOCKSIZE];
};

// Change to mymkfs
void create_fd(int size_bytes); // initialize new file system.
void mount_fs(); // load file system.
void sync_fs(); // write in the file system.

void fd_info(); // For printing fd information.

/**
 * @brief for find empty inode.
 * 
 * @return int -> empty inode index.
 */
int find_empty_inode(); 

/**
 * @brief finds empty block in the block array.
 * 
 * @return int -> empty block index.
 */
int find_empty_block();


// return filenumber
int allocate_file(char file_name[8]);

/**
 * @brief Set / Reset the size of the file.
 * 
 * @param file_num - the number of the inode that hold this file.
 * @param file_size - the new size we want to allocate to this file.
 */
void set_file_size(int file_num, int file_size);

void write_byte(int file_num, int offset, char *data);




