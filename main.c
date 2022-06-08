#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include "file_system.h"

int main()
{   
    
    char *data = "This is the data Iwant to add!\0";
    
    // Test 1 : Creating New File System (FS).
    printf("Test1: Creating new File System.\n");
    mymkfs(1024*16);
    mymount("myFile.txt", "","",0,"");
    printf("\n");

    // Test 2: Creating 3 new files in the FS using myopen.
    printf("Test2: Creating 3 files.\n");
    myopen("file1",O_CREAT);
    myopen("file2",O_CREAT);
    myopen("file3",O_CREAT);
    print_fd();
    printf("\n");

    // Test 3: Using myopen to get back index in the fd of the files we created.
    printf("\n");
    printf("Test3: Opening Existing files: file1, file2, file3 (Using diffrent flags). Expect to get back the right locations in the fd.\n");
    int f1 = myopen("file1",O_WRONLY);
    int f2 = myopen("file2",O_RDONLY);
    int f3 = myopen("file3",O_RDWR);
    printf("Locations in FD: file 1 = %d, file 2 = %d, file 3 = %d\n",f1,f2,f3);
    printf("\n");


    sync_fs("myFile.txt");
    

    // Test 4: Using myclose to close file2. 
    printf("\n");
    printf("Test4: Closing file 2 and Creating file4 (Exppecting it will be allocate where file2 was).\n");
    if( myclose(f2) < 0 ){
        perror("Close Failed\n");
    }
    int f4 = myopen("file4",O_CREAT);
    int f5 = myopen("file5",O_CREAT);
    print_fd();
    printf("\n");

    sync_fs("myFile.txt");
    
    
    
    
    
    
    
    
    
    
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

    printf("\n");
    // fs_info();
    printf("\n");
    printf("Done.\nOUT\n");
    return 0;
}