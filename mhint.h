#ifndef _MHINT_H
#define _MHINT_H

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <sys/mman.h>

#define MAX_NR_HINT_OBJS	10
#define CHECK_HINT(_objid, _addr)	{\
	if (mhint[target_idx].objid == _objid) {\
		if(mlock(_addr + mhint[target_idx].offset, mhint[target_idx].size))\
			printf("obj %ld: mlock failed!\n", _objid);\
		else\
			printf("obj %ld: mlock succeed!\n", _objid);\
		target_idx++;\
	}\
}

struct hint_data {
	long objid;
	unsigned long offset;
	unsigned long size;
};

typedef void *(*malloc_t)(size_t size);
typedef void *(*calloc_t)(size_t nmemb, size_t size);
typedef void *(*realloc_t)(void *ptr, size_t size);
typedef void (*free_t)(void *ptr);

void alloc_init(void) __attribute__ ((constructor));

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void* ptr, size_t size);
void free(void *ptr);

unsigned char calloc_buffer[8192];

malloc_t malloc_fn;
calloc_t calloc_fn;
realloc_t realloc_fn;
free_t free_fn;

long objid;

int target_idx;
struct hint_data mhint[MAX_NR_HINT_OBJS] = {
	{4, 0, 7200000},
	{6, 0, 140600},
	{7, 0, 895200},
	{-1, 0, 0},
};

#endif
