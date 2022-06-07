#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include "file_system.h"

int main()
{   
    
    char *data = "This is the data Iwant to add!\0";
    
    mymkfs(1024*16);
    mymount("myFile.txt", "","",0,"");

    myopen("file1",O_CREAT);
    myopen("file2",O_CREAT);
    myopen("file3",O_CREAT);

    print_fd();
    printf("\n");
    // fs_info();
    printf("\n");

    int w = myopen("file1",O_WRONLY);
    int r = myopen("file2",O_RDONLY);
    int wr = myopen("file3",O_RDWR);

    printf("\n");
    printf("Locations in FD: file 1 = %d, file 2 = %d, file 3 = %d\n",w,r,wr);
    // printf("\n");
    // print_fd();
    // mount_fs();
    // int first = allocate_file("first");
    // set_file_size(first, 1024*5);
    // int second = allocate_file("Second");
    // set_file_size(second, 1024*6);
    // for (int i=0; data[i] != '\0'; i++){
    //     write_byte(0, i    , &(data[i]));
    //     write_byte(0, i+100, &(data[i]));
    //     write_byte(0, i+200, &(data[i]));
    // }
    // set_file_size(0, 1024*7);

    // sync_fs();
    // fs_info();

    printf("\n");
    printf("Done.\nOUT\n");
    return 0;
}