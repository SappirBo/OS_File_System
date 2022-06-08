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
    printf("Test3: Opening Existing files: 'file1', 'file2', 'file3' (Using diffrent flags). Expect to get back the right locations in the fd.\n");
    int f1 = myopen("file1",O_WRONLY);
    int f2 = myopen("file2",O_RDONLY);
    int f3 = myopen("file3",O_RDWR);
    printf("       Locations in FD: 'file1' = %d, 'file2' = %d, 'file3' = %d\n",f1,f2,f3);
    printf("\n");


    sync_fs("myFile.txt");
    

    // Test 4: Using myclose to close file2. 
    printf("\n");
    printf("Test4: Closing 'file2' and Creating 'file4' (Expecting it will be allocate where 'file2' was and 'file2' location now will be -1).\n");
    if( myclose(f2) < 0 ){
        perror("Close Failed\n");
    }
    int f4 = myopen("file4",O_CREAT);
    int f5 = myopen("file5",O_CREAT);
    f2 = myopen("file2",O_RDWR);
    printf("       Locations in FD: 'file 4' = %d, 'file5' = %d, 'file2' = %d\n",f4,f5,f2);
    printf("\n");
 

    // Test5: Write And Read from file.
    printf("Test5: Write 'Hello World' to file1 and file3 (permission write only and read and write only),\n");
    printf("       Try to write to file4 (that have read only permission).\n");
    f4 = myopen("file4",O_RDONLY);
    int a = mywrite(f1,"Hello World\n",strlen("Hello World\n"));
    int b = mywrite(f3,"Hello World\n",strlen("Hello World\n"));
    int c = mywrite(f4,"Hello World\n",strlen("Hello World\n"));
    printf("       Data Written to: 'file1': %d Bytes, 'file3': %d Bytes, 'file4': %d Bytes.\n",a,b,c);
    printf("\n");
    char *test = malloc(sizeof(char) * 12);
    myread(f1,test, 12);
    printf("\n       now read the content in file1: %s",test);






    sync_fs("myFile.txt");

    printf("\n");
    // fs_info();
    printf("\n");
    printf("Done.\nOUT\n");
    return 0;
}