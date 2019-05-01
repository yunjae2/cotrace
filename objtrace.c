#include "objtrace.h"

void *malloc(size_t size)
{
	malloc_t malloc_fn;
	void *addr;

	malloc_fn = (malloc_t)dlsym(RTLD_NEXT, "malloc");
	addr = malloc_fn(size);

	fprintf(stderr, "malloc: %p (%zu)\n", addr, size);
	return addr;
}

void *calloc(size_t nmemb, size_t size)
{
	calloc_t calloc_fn;
	void *addr;

	calloc_fn = (calloc_t)dlsym(RTLD_NEXT, "calloc");
	addr = calloc_fn(nmemb, size);

	fprintf(stderr, "calloc: %p (%zu x %zu)\n", addr, nmemb, size);
	return addr;
}

void *realloc(void* ptr, size_t size)
{
	realloc_t realloc_fn;
	void *addr;

	realloc_fn = (realloc_t)dlsym(RTLD_NEXT, "realloc");
	addr = realloc_fn(ptr, size);

	fprintf(stderr, "realloc: %p -> %p (%zu)\n", ptr, addr, size);
	return addr;
}

void free(void *ptr)
{
	free_t free_fn;

	free_fn = (free_t)dlsym(RTLD_NEXT, "free");
	free_fn(ptr);

	fprintf(stderr, "free: %p\n", ptr);
	return;
}
