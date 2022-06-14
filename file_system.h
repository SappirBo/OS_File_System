#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#pragma once

#define BLOCKSIZE 508
#define PATH_MAX 256
#define MAX_DIR_FILES 20
#define KB 1024
#define NAME_SIZE 8
#define FIRST_INODE 0

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
struct inode{
    int size;
    int first_block;
    char name[NAME_SIZE];
    int inode_type; // 0 for file, 1 for dir, -1 if not initialize.
};

/**
 * @brief Hard Disk Block, 
 *        it has 508 bytes string for data and pointer to the next block.
 *        Size  = 512 bytes (4 bytes for one integer and 512 bytes for 512 chars).
 */
struct block{
    int next_block;
    char data[BLOCKSIZE];
};
/**
 * @brief This is a singal file in the File Discriptor,
 *        Each File have the inode number of that file, cursor (Starts as 0),
 *        And permission: - 0 read only, 1 write only, 2 read and write, -1 before init.       
 */ 
struct myopenfile{
    int file_node;
    int cursor;
    int permission; // 
};

/**
 * @brief DIR struct, 
 * 
 */
typedef struct myDIR {
    char d_name[NAME_SIZE];
    int files[MAX_DIR_FILES];
    int inode_num;
}myDIR;

struct mydirent {
    /* File position within stream */
    long d_off;

    /* Structure size */
    unsigned short d_reclen;

    /* Length of name without \0 */
    size_t d_namlen;

    /* File type */
    int d_type;

    /* File name */
    char d_name[NAME_SIZE];
};


void create_fs(int size_bytes); // initialize new file system.
void mount_fs(); // load file system.
void sync_fs(const char *str); // write the file system.

/**
 * @brief After creating new file system we will want to add it a root directory,
 * THis function will add root dir (mydirent) to the file system.
 */
void add_root();

void fs_info(); // For printing fd information.

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
int allocate_file(const char *file_name);

int allocate_dir(const char *file_name);

/**
 * @brief Set / Reset the size of the file.
 * 
 * @param file_num - the number of the inode that hold this file.
 * @param file_size - the new size we want to allocate to this file.
 */
void set_file_size(int file_num, int file_size);

void write_byte(int file_num, int offset, char *data);

void * read_byte(int file_num, int pos, size_t length);

//////////////////////////////////////////////////////////////////////////////////
//                   IMPLEMENTION OF TASK 7.
// This part holds the functions Signatures as given in the task info.
//////////////////////////////////////////////////////////////////////////////////


void print_fd();

void mymkfs(int bytes);

int mymount(const char *source, const char *target,const char *filesystemtype, unsigned long mountflags, const void *data);

int myopen(const char *pathname, int flags);

int myclose(int myfd);

ssize_t myread(int myfd, void *buf, size_t count);

ssize_t mywrite(int myfd, const void *buf, size_t count);

off_t mylseek(int myfd, off_t offset, int whence);

myDIR *myopendir(const char *name);

// struct mydirent *myreaddir(myDIR *dirp);

// int myclosedir(myDIR *dirp);













