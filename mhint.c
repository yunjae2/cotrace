#include "mhint.h"

void alloc_init(void)
{
	malloc_fn = (malloc_t)dlsym(RTLD_NEXT, "malloc");
	calloc_fn = (calloc_t)dlsym(RTLD_NEXT, "calloc");
	realloc_fn = (realloc_t)dlsym(RTLD_NEXT, "realloc");
	free_fn = (free_t)dlsym(RTLD_NEXT, "free");
}

void *malloc(size_t size)
{
	void *addr;

	addr = malloc_fn(size);
	CHECK_HINT(objid, addr);
	objid++;

	return addr;
}

void *calloc(size_t nmemb, size_t size)
{
	void *addr;

	/* dlsym() calls calloc() internally, so deal with it */
	if (calloc_fn == NULL)
		return calloc_buffer;

	addr = calloc_fn(nmemb, size);
	CHECK_HINT(objid, addr);
	objid++;

	return addr;
}

void *realloc(void* ptr, size_t size)
{
	void *addr;

	addr = realloc_fn(ptr, size);
	CHECK_HINT(objid, addr);
	objid++;

	return addr;
}

void free(void *ptr)
{
	if (ptr == calloc_buffer)
		return;

	free_fn(ptr);
	return;
}
