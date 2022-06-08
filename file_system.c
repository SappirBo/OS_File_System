#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include "file_system.h"

// init for the structs.
struct super_block sb;
struct inode *inodes;
struct block *blocks; 

// init for the file descriptor struct
struct myopenfile *fd;
int fd_size;


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
        strcpy(inodes[i].name,"____");
    } // init inodes.

    blocks = malloc(sizeof(struct block) * sb.num_blocks);
    for(i=0; i<sb.num_blocks; i++){
        blocks[i].next_block = -1;
    } // init blocks.


    // Setting up the file descriptor for first time.
    // In the task description they said we can asume there are at most 10000 open.
    fd = malloc(sizeof(struct myopenfile)*10000);
    for(i=0;i<10000; i++){
        fd[i].file_node = -1;
        fd[i].cursor = -1;
    }
    fd_size = 0;
    // fd[0].cursor = 0;
    // fd[0].file_node = 0;
    // fd_size = 0;
} 


// load file system.
void mount_fs()
{
    FILE *file;
    file = fopen("fd_data.txt","r");

    if(file == NULL){
        perror("No Such File!\nOUT.\n");
        exit(1);
    }
    
    // Reading the Super Block Info
    fread(&sb,sizeof(struct super_block) ,1 ,file);
    
    // Creat new Inodes and Blocks Arrays.
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    blocks = malloc(sizeof(struct block) * sb.num_blocks);

    // Reading the blocks and the inodes 
    fread(inodes,sizeof(struct inode), sb.num_inodes,file);
    fread(blocks,sizeof(struct block), sb.num_blocks,file);

    fclose(file);

} 

// write in the file system.
void sync_fs(const char *str)
{
    FILE *file;
    file = fopen(str,"w+");
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
void fs_info()
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


/**
 * @brief for find empty inode.
 * @return int - If the operation was successful it returns the empty inode index, else it returns -1 (something is wrong/ no empty inode).
 */
int find_empty_inode()
{
    int i;
    for(i=0; i<sb.num_inodes; i++){
        if(inodes[i].first_block == -1){
            return i;
        }
    }
    return -1;
}

/**
 * @brief finds empty block in the block array.
 * @return int -> If the operation was successful it returns the empty block index,  else it returns -1 (something is wrong/ no empty block).
 */
int find_empty_block()
{
    int i;
    for(i=0; i<sb.num_blocks; i++){
        if(blocks[i].next_block == -1){
            return i;
        }
    }
    return -1;
}

/**
 * @brief find the currect inode by the file name.
 * @param file_name -> name of the file we look for.
 * @return int  -> number of the inode, -1 in case of failure.
 */
int find_file_inode(const char *file_name){
    int i=0;
    while(i<sb.num_inodes){
        if(!strcmp(inodes[i].name,file_name)){
            return i;
        }
        i++;
    }
    return -1;
}

int find_empty_fd(){
    int i=0;
    while(i<10000){
        if(fd[i].file_node == -1){
            return i;
        }
        i++;
    }
    return -1;
}

int find_index_of_inode_in_fd(int inode_num){
    int i;
    for(i=0; i<fd_size; i++){
        if(fd[i].file_node == inode_num){
            return i;
        }
    }
    return -1;
}

/**
 * @brief Function for creating and allocate new file in are Hard Disk.
 * 
 * @param file_name -> Name Of the file we want to create.
 * @return int -> the inode contains this new file.
 */
 // before TEST***********char file_name[8]
int allocate_file(const char *file_name)
{
    char name[8];
    for(int i=0; i<8; i++){
        name[i] = file_name[i];
    }
    // Find empty inode and empty block.
    int em_inode = find_empty_inode();
    int em_block = find_empty_block();

    inodes[em_inode].first_block = em_block;
    strcpy(inodes[em_inode].name,name);
    blocks[em_block].next_block = -2;

    return em_inode;
}

/**
 * @brief Helper for set_file_size 
 *      -> after finish resetting the size, we will look for empty blocks that we can reuse.
 * @param block_num  = the last block index we used in "set_file_size".
 */
void shorten_file(int block_num)
{
    int num = blocks[block_num].next_block;
    if(num >= 0){
        shorten_file(num);
    }
    blocks[block_num].next_block = -1;

}


void set_file_size(int file_num, int file_size)
{
    // Find out how many blocks will we need.
    int number_of_blocks = file_size/BLOCKSIZE;
    if(file_size%BLOCKSIZE != 0){
        number_of_blocks += 1;
    }

    int block_num = inodes[file_num].first_block;
    
    // Grow the file blocks as much as needed.
    while(number_of_blocks > 0){
        // Checking the next block.
        int next_block = blocks[block_num].next_block;
        if(next_block == -2){
            // If there is no other block allocated next, we setting it up right now.
            int em_block = find_empty_block();
            blocks[block_num].next_block = em_block;
            blocks[em_block].next_block = -2;
        }
        block_num = blocks[block_num].next_block;
        number_of_blocks--;
    }

    // Check For shrinking
    shorten_file(block_num);

    blocks[block_num].next_block = -2;
}

int get_block_number(int file_num, int offset){
    int block_num = inodes[file_num].first_block;
    int tmp = offset; 
    while (tmp > 0)
    {
        block_num = blocks[block_num].next_block;
        tmp--;
    }
    return block_num;
    
}


void write_byte(int file_num, int pos, char *data)
{
    // Find the right block.
    int block = pos/BLOCKSIZE;

    //Get this block's number.
    int block_num = get_block_number(file_num,block);

    int offset = pos % BLOCKSIZE;

    blocks[block_num].data[offset] = (*data);
}

//////////////////////////////////////////////////////////////////////////////////
//                   IMPLEMENTION OF TASK 7.
// This part holds the functions Signatures as given in the task info.
//////////////////////////////////////////////////////////////////////////////////


void print_fd(){
    printf("File Descriptor: size = %d\n",fd_size);
    printf("nodes: ");
    int i;
    for(i=0; i<fd_size; i++){
        printf("->(fd = %d,File = %s) ",fd[i].file_node , inodes[fd[i].file_node].name);
    }
    printf("\n");
}

void mymkfs(int bytes){
    FILE *file;
    file = fopen("myFile.txt","w+");
    for(int i=0; i<bytes; i++){
        fwrite(" ",sizeof(char) ,1 ,file);
    }
    create_fd(bytes);
    printf("mymkfs: Hard Disk created -> myFile.txt\n");
}

int mymount(const char *source, const char *target,const char *filesystemtype, unsigned long mountflags, const void *data)
{   
    sync_fs(source);
    printf("mymount: File System created, name: %s\n", source);
}

/**
 * @brief open file from are Hard Disk
 * 
 * @param pathname = name of the path / file we look for. 
 * @param flags: 1. O_CREAT = Create new file.
 *               2. O_RDONLY = Read only.
 *               3. O_WRONLY = Write only.
 *               4. O_RDWR = Read & Write.
 * @return int = the number of this file in the File Descriptor.
 */
int myopen(const char *pathname, int flags){
    // TODO: Check pathname validation.
    if(flags == O_CREAT){
        int inode_num = allocate_file(pathname);
        int index = find_empty_fd();
        fd[index].file_node = inode_num;
        fd[index].cursor = 0;
        fd_size++;
        return index;
    }
    // Flag for getting Existing file.
    else if(flags == O_RDONLY || flags == O_WRONLY || flags == O_RDWR){
        if(fd_size == 0){
            perror("No File found. Try diffrent pathname.\n");
        }else{
            int inode_num = find_file_inode(pathname);
            if (inode_num == -1){
                perror("No File found. Try diffrent pathname.\n");
            }
            else{
                return find_index_of_inode_in_fd(inode_num);
            }
        }
    }else{
        return -1;
    } 
}

/**
 * @brief Function for close file in the File Descriptor. 
 * 
 * @param myfd - index number of the file we want to close (some id number you get from "open").
 * @return int - 0 on succsses, -1 otherwise.
 */
int myclose(int myfd){
    if(fd_size == 0 || fd[myfd].file_node == -1){
        return -1;
    }
    else{
        fd[myfd].file_node = -1;
        fd[myfd].cursor = 0;
        fd_size--;
        return 0;
    }
}
























