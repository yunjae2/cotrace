#ifndef _OBJTRACE_H
#define _OBJTRACE_H

#define _GNU_SOURCE
#include <dlfcn.h>
#include "common.h"

#define OBJ_PACK(pkg, _obj, _addr, _size, _time, _ctx, _ctx_addr)	{\
	pkg.obj = _obj;\
	pkg.addr = _addr;\
	pkg.size = _size;\
	pkg.time = _time;\
	pkg.ctx = _ctx;\
	pkg.ctx_addr = _ctx_addr;\
}

struct obj_data {
	long obj;
	unsigned long addr;
	unsigned long size;
	unsigned long time;
	long ctx;
	unsigned long ctx_addr;
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

unsigned char calloc_buffer[8192];

malloc_t malloc_fn;
calloc_t calloc_fn;
realloc_t realloc_fn;
free_t free_fn;

FILE *fp_obj;
int disable_objtrace;
int in_dlsym;
int objid;

int obj_buf_offset;
struct obj_data obj_buf[MAX_BUF_OFFSET];

#endif
