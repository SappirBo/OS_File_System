
#pragma once

#include "myfs.h"
#include <stdlib.h>
#include <stdarg.h>

typedef struct myFILE {
	char* data;
	char state[3];
	int fd;
	int size;
	int ptr ;
} myFILE;

myFILE *myfopen(const char *restrict pathname, const char *restrict mode);
int myfclose(myFILE *stream);
size_t myfread(void *restrict ptr, size_t size, size_t nmemb,myFILE *restrict stream);
size_t myfwrite(const void *restrict ptr, size_t size, size_tnmemb, myFILE *restrict stream);
int myfseek(myFILE *stream, long offset, int whence);
int myfscanf(myFILE *restrict stream,const char *restrict format, ...);
int myfprintf(myFILE *restrict stream,const char *restrict format, ...);
