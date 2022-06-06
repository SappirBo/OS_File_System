#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include "file_system.h"



int main()
{
    create_fd(1024*16);
    // sync_fs();
    // mount_fs();
    // int first;
    int first = allocate_file("first");
    set_file_size(first, 1024*2);
    int second = allocate_file("Second");
    set_file_size(second, 1024*6);
    fd_info();

    set_file_size(first, 1024*4);

    // sync_fs();
    fd_info();


    printf("Done.\nOUT\n");
    return 0;
}