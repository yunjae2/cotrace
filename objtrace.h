#ifndef _OBJTRACE_H
#define _OBJTRACE_H

#define _GNU_SOURCE
#include <dlfcn.h>
#include "common.h"

#define OBJWRITE(...) {\
	if (!disable_objtrace) {\
		disable_objtrace = 1;\
		fprintf(fp_obj, __VA_ARGS__);\
		disable_objtrace = 0;\
	}\
}

#define OBJ_PACK(pkg, _obj, _addr, _size, _time)	{\
	pkg.obj = _obj;\
	pkg.addr = _addr;\
	pkg.size = _size;\
	pkg.time = _time;\
}

struct obj_data {
	int obj;
	unsigned long addr;
	unsigned long size;
	unsigned long time;
};

typedef void *(*malloc_t)(size_t size);
typedef void *(*calloc_t)(size_t nmemb, size_t size);
typedef void *(*realloc_t)(void *ptr, size_t size);
typedef void (*free_t)(void *ptr);

void *malloc(size_t size) __attribute__ ((no_instrument_function));
void *calloc(size_t nmemb, size_t size)
	__attribute__ ((no_instrument_function));
void *realloc(void* ptr, size_t size) __attribute__ ((no_instrument_function));
void free(void *ptr) __attribute__ ((no_instrument_function));

malloc_t malloc_fn;
calloc_t calloc_fn;
realloc_t realloc_fn;
free_t free_fn;

int objid;

int obj_buf_offset;
struct obj_data obj_buf[MAX_BUF_OFFSET];

#endif
