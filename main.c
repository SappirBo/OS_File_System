#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include "file_system.h"



int main()
{
    create_fd(1024*5);
    fd_info();
    sync_fs();
    printf("Done\n");
    return 0;
}