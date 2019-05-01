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

typedef void *(*malloc_t)(size_t size);
typedef void *(*calloc_t)(size_t nmemb, size_t size);
typedef void *(*realloc_t)(void *ptr, size_t size);
typedef void (*free_t)(void *ptr);

malloc_t malloc_fn;
calloc_t calloc_fn;
realloc_t realloc_fn;
free_t free_fn;

#endif
