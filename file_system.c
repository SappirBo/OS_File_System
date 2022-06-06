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
        strcpy(inodes[i].name,"Inode");
    } // init inodes.

    blocks = malloc(sizeof(struct block) * sb.num_blocks);
    for(i=0; i<sb.num_blocks; i++){
        blocks[i].next_block = -1;
    } // init blocks.

} 


void mount_fs(); // load file system.

// write in the file system.
void sync_fs()
{
    FILE *file;
    file = fopen("fd_data.txt","w+");
    int i;

    // Super Block
    fwrite(&sb,sizeof(struct super_block) ,1 ,file);
    
    // Inodes
    for(i=0; i<sb.num_inodes; i++){
        fwrite(&(inodes[i]), sizeof(struct inode), 1, file);
    }

    // Blocks
    for(i=0; i<sb.num_blocks; i++){
        fwrite(&(blocks[i]), sizeof(struct block), 1, file);
    }

    fclose(file);
}



/**
 * @brief Print the File System Information:
 *      1. Super Block info.
 *      2. Inodes Info.
 *      3. Blocks Info.
 */
void fd_info()
{
    int i;

    // Printing the Super Block Info.   
    printf("Super Block Info:\n");
    printf("    1. number of inodes = %d\n    2. number of blocks = %d\n    3. block size = %d\n",sb.num_inodes,sb.num_blocks,sb.block_size);
    
    // Printing the Inodes Info.
    printf("Inodes Info:\n");
    for(i=0;i<sb.num_inodes;i++){
        printf("    %d. Inode name: %s, Size: %d, First Block: %d\n",i ,inodes[i].name,inodes[i].size ,inodes[i].first_block);
    }

    // Printing the Blocks Info.
    printf("Blocks Info:\n");
    for(i=0;i<sb.num_blocks;i++){
        printf("    %d. block num: %d, Next Block: %d\n",i ,i, blocks[i].next_block);
    }

}




