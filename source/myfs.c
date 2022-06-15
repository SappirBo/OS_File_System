#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <unistd.h>
#include "myfs.h"
#include <errno.h>

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
void create_fs(int size_bytes)
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
        inodes[i].inode_type = -1;
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
        fd[i].permission = -1;
    }
    fd_size = 0;
} 


// load file system.
void mount_fs(const char *source)
{
    FILE *file;
    file = fopen(source,"r");

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
        printf("    %d. Inode name: %s, Size: %d, First Block: %d, Inode Type: %d\n",i ,inodes[i].name,inodes[i].size ,inodes[i].first_block,inodes[i].inode_type);
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
    inodes[em_inode].inode_type = 0;
    strcpy(inodes[em_inode].name,name);
    blocks[em_block].next_block = -2;

    return em_inode;
}

int allocate_dir(const char *file_name)
{
    char name[8];
    for(int i=0; i<8; i++){
        name[i] = file_name[i];
    }
    // Find empty inode and empty block.
    int em_inode = find_empty_inode();
    int em_block = find_empty_block();

    inodes[em_inode].first_block = em_block;
    inodes[em_inode].inode_type = 1;
    strcpy(inodes[em_inode].name,name);
    blocks[em_block].next_block = -2;

    return em_inode;
}

/**
 * @brief After creating new file system we will want to add it a root directory,
 * THis function will add root dir (mydirent) to the file system.
 */
void add_root(){
    if( inodes[FIRST_INODE].inode_type != -1 ){
        printf("Init Failed.\n");
        exit(1);
    }
    else if ( inodes[FIRST_INODE].inode_type == 1 && inodes[FIRST_INODE].name == "root"){
        printf("root allready exists.\n");
    }
    // Creating new dir 'root' and setting it's values.
    int root_inode = allocate_dir("root");
    myDIR dir;
    myDIR* root = &dir;
    strcpy(root->d_name,"root");
    int i;
    for(i=0; i<MAX_DIR_FILES; i++){
        root->d_name[i] = -1;
    }
    root->inode_num = root_inode;
    
    char* data = (char*) root;
    for(int i=0; i<sizeof(*root); i++){
        write_byte(root_inode, i, data);
    }

    int index = find_empty_fd();
    fd[index].file_node = root_inode;
    fd[index].permission = 0;
    fd_size++;
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

    int i=0;
    blocks[block_num].data[offset+i] = data[i];
    inodes[file_num].size++;
}

char* read_byte(int file_num, int pos, size_t length){
    // Find the right block.
    int block = pos/BLOCKSIZE;
    //Get this block's number.
    int block_num = get_block_number(file_num,block);
    int offset = pos % BLOCKSIZE;
    
    char *data;
    data = malloc(length);
    int i;
    for(i=0; i<length; i++){
        data[i] = blocks[block_num].data[offset+i];
    }
    return data;
}

//////////////////////////////////////////////////////////////////////////////////
//                   IMPLEMENTION OF TASK 7.
// This part holds the functions Signatures as given in the task info.
//////////////////////////////////////////////////////////////////////////////////

void print_fd(){
    printf("       File Descriptor: size = %d\n",fd_size);
    printf("       nodes:");
    int i;
    for(i=0; i<fd_size; i++){
        if(i%3 == 0 && i!=0){printf("\n             ");}
        if(inodes[fd[i].file_node].inode_type == 0){
            printf(" -> [fd = %d, File = '%s', Inode = %d] ",i,inodes[fd[i].file_node].name,fd[i].file_node );
        }else if(inodes[fd[i].file_node].inode_type == 1){
            printf(" -> [fd = %d, DIR = '%s', Inode = %d] ",i,inodes[fd[i].file_node].name,fd[i].file_node );
        }
    }
    printf("\n");
}

void mymkfs(int bytes){
    create_fs(bytes);
    add_root();
}

int mymount(const char *source, const char *target,const char *filesystemtype, unsigned long mountflags, const void *data)
{
    if (source == NULL && target == NULL){
        // In this case we got bad input, return -1 for failure.
        return -1;
    }
    else if ( target != NULL && source == NULL){
        // In this case source is null, but target is not, so we will save target.
        mymkfs(KB*16); // Create FS with 16 KB.
        sync_fs(target); 
        return 0;
    }
    else if( access(source, F_OK) == -1){
        // In case The FS Doent Exists.
        return -1;
    } 
    else if (source != NULL && target == NULL){
        // in this case target is null but source is not, we will upload the source fs.
        mount_fs(source);
        return 0;
    }
    // Check if the file system allready exists -> source is the file system name.
    else{
        mount_fs(source);
        sync_fs(target);
        return 0;
    }
    return -1;
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
        int index_check = find_file_inode(pathname);
        if( index_check !=  -1){
            int index = find_empty_fd();
            fd[index].file_node = index_check;
            fd[index].cursor = 0;
            fd_size++;
            int i=0;
            // while(currDir->files[i] != -1 && i<MAX_DIR_FILES){
            //     i++;
            // }
            // currDir->files[i] = index_check;
            return index_check;
        }
        int inode_num = allocate_file(pathname);
        int index = find_empty_fd();
        fd[index].file_node = inode_num;
        fd[index].cursor = 0;
        fd_size++;

        int i=0;
        // while(currDir->files[i] != -1 && i<MAX_DIR_FILES){
        //     i++;
        // }
        // currDir->files[i] = inode_num;
        
        return index;
    }
    // Flag for getting Existing file.
    else if(flags == O_RDONLY || flags == O_WRONLY || flags == O_RDWR){
        if(fd_size == 0){
            perror("No File found, fd Empty.\n");
        }else{
            int inode_num = find_file_inode(pathname);
            if (inode_num == -1){
                perror("No File found. Try diffrent pathname.\n");
            }
            else{
                // Get the index of this file in the FD.
                int index = find_index_of_inode_in_fd(inode_num);
                // Setup the right permission.
                if(flags == O_WRONLY){fd[index].permission = 0;}
                else if(flags == O_RDONLY){fd[index].permission = 1;}
                else if(flags == O_RDWR){fd[index].permission = 2;}
                // Return the index of this file in the FD.
                return index;
            }
        }
    }else{
        // Something went wrong.
        printf("Eror: Can't Open File.\n");
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
        fd[myfd].cursor = -1;
        fd[myfd].permission = -1;
        fd_size--;
        return 0;
    }
}


ssize_t myread(int myfd, void *buf, size_t count){
    // Checking for valid fd ID and that the fd have some files in it.
    if(fd[myfd].file_node == -1 || fd_size == 0){
        printf("Eror, file is not allocated (fd code: %d, fd size: %d)\n",fd[myfd].file_node,fd_size);
        return -1;
    }
    if(inodes[fd[myfd].file_node].inode_type == 0){
        // Checking for Right permission.
        if(fd[myfd].permission == 0 || fd[myfd].permission == 2){
            int inode_num = fd[myfd].file_node;
            if(count+fd[myfd].cursor > inodes[inode_num].size){
                count = inodes[inode_num].size - fd[myfd].cursor;
            }
            int cursor = fd[myfd].cursor;
            strcpy(buf,read_byte(inode_num, cursor, count));
            fd[myfd].cursor = cursor + count;
            return strlen(buf);
        }
        // In case something went wrong 
        else{
            printf("Eror: Premission Denied.\n");
            return -1;
        }
    }
}

ssize_t mywrite(int myfd, const void *buf, size_t count){
    // Checking for valid fd ID and that the fd have some files in it.
    if(fd[myfd].file_node == -1 || fd_size == 0){return -1;}
    // Checking for Right permission.
    else if(fd[myfd].permission == 1 || fd[myfd].permission == 2){
        int inode_num = fd[myfd].file_node;
        int cursor = fd[myfd].cursor;
        size_t i;
        char *data = (char*) buf; // Cast to Bytes.
        for(i=0; i<count; i++){
            write_byte(inode_num, cursor+i,&data[i]);
        }      
        fd[myfd].cursor = cursor+count;
        return count;
    }
    // In case something went wrong 
    return -1;
}

// TODO: Check Valid Offset. 
off_t mylseek(int myfd, off_t offset, int whence){
    // Checking for valid fd ID and that the fd have some files in it.
    if(fd[myfd].file_node == -1 || fd_size == 0){return -1;}
    if(offset + inodes[fd[myfd].file_node].size < 0){return -1;}
    if(whence == SEEK_CUR){
        int current = fd[myfd].cursor;
        current += offset;
        fd[myfd].cursor = current;
        return current;
    }
    else if(whence == SEEK_SET){
        fd[myfd].cursor = offset;
        return offset;
    }
    else if(whence == SEEK_END){
        fd[myfd].cursor = offset + inodes[fd[myfd].file_node].size;
        return fd[myfd].cursor;
    }
    else{
        return -1;
    }
}

myDIR *myopendir(const char *name){
    int dir_inode = find_file_inode(name);
    // Check if this name exists but it is a file -> eror.
    if(dir_inode == -1){
        int dir_inode = allocate_dir(name);
        myDIR* dir;
        strcpy(dir->d_name,name);
        int i;
        for(i=0; i<MAX_DIR_FILES; i++){
            dir->d_name[i] = -1;
        }
        dir->inode_num = dir_inode;
        
        char* data = (char*) dir;
        
        for(int i=0; i<sizeof(myDIR); i++){
            write_byte(dir_inode, i, data);
        }

        int index = find_empty_fd();
        fd[index].file_node = dir_inode;
        fd[index].cursor = 0;
        fd[index].permission = 0;
        fd_size++;
        return dir;
    }
    else if(inodes[dir_inode].inode_type == 0){
        printf("Cant Open File as Dir (type: %d, inode:%d).\n",inodes[dir_inode].inode_type,dir_inode);
    }
    // Check if this Dir exists.
    else if(inodes[dir_inode].inode_type == 1){
        myDIR* data = malloc(sizeof(myDIR));
        data = (myDIR*) read_byte(dir_inode, 0, sizeof(myDIR));
        return data;
    }   
    return NULL;
}

struct mydirent *myreaddir(myDIR *dirp){
    int file_index = find_file_inode(dirp->d_name);
    if(file_index == -1){
        printf("Dir is not Found!\n");
        exit(1);
    }
    int curr_index = -1;
    int i;
    for(i=0; i <fd_size; i++){
        if(fd[i].file_node == file_index){
            curr_index = fd[i].cursor;
            break;
        }
    }
    if(curr_index == -1){printf("Dir Has No Items.");exit(1);}
    struct mydirent m_dirent;
    struct mydirent* ptr_dirent = &m_dirent;
    strcpy(m_dirent.d_name ,inodes[curr_index].name);
    m_dirent.d_type = inodes[curr_index].inode_type;

    fd[i].cursor++;

    return ptr_dirent;
}

int myclosedir(myDIR *dirp){
    int inode;
    inode = dirp->inode_num;
    if(fd_size == 0){
        return -1;
    }
    int index = -1;
    for(int i=0; i<fd_size; i++){
        if(fd[i].file_node == inode){
            index = i;
            break;
        }
    }
    if(index == -1){
        return -1;
    }
    else{
        fd[index].file_node = -1;
        fd[index].cursor = -1;
        fd[index].permission = -1;
        fd_size--;

        return 0;
    }
}














