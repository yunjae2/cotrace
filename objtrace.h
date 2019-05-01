#ifndef _OBJTRACE_H
#define _OBJTRACE_H

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

typedef void *(*malloc_t)(size_t size);
typedef void *(*calloc_t)(size_t nmemb, size_t size);
typedef void *(*realloc_t)(void *ptr, size_t size);
typedef void (*free_t)(void *ptr);

#endif
