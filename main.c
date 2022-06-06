#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include "file_system.h"



int main()
{
    // create_fd(1024*2);
    // sync_fs();
    mount_fs();
    fd_info();

    printf("Done.\nOUT\n");
    return 0;
}