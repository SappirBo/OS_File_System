#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include "file_system.h"



int main()
{   
    
    char *data = "This is the data Iwant to add!\0";
    
    create_fd(1024*16);
    sync_fs();


    // mount_fs();
    
    int first = allocate_file("first");
    set_file_size(first, 1024*2);

    for (int i=0; data[i] != '\0'; i++){
        write_byte(first, i    , &(data[i]));
        write_byte(first, i+100, &(data[i]));
        write_byte(first, i+200, &(data[i]));
    }
    // int second = allocate_file("Second");
    // set_file_size(second, 1024*6);
    // fd_info();

    // set_file_size(first, 1024*4);

    sync_fs();
    fd_info();


    printf("Done.\nOUT\n");
    return 0;
}