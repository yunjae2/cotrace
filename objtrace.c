#include "objtrace.h"

void objtrace_init(void)
{
	malloc_fn = (malloc_t)dlsym(RTLD_NEXT, "malloc");
	calloc_fn = (calloc_t)dlsym(RTLD_NEXT, "calloc");
	realloc_fn = (realloc_t)dlsym(RTLD_NEXT, "realloc");
	free_fn = (free_t)dlsym(RTLD_NEXT, "free");
	fp_obj = fopen("obj.data", "w");
}

void *malloc(size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;

	GETTIME(time, ts);
	addr = malloc_fn(size);

	OBJWRITE("[%10lu]\t malloc: %p (%zu)\n", RELTIME(time), addr, size);
	return addr;
}

void *calloc(size_t nmemb, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;

	GETTIME(time, ts);
	addr = calloc_fn(nmemb, size);

	OBJWRITE("[%10lu]\t calloc: %p (%zu x %zu)\n",
			RELTIME(time), addr, nmemb, size);
	return addr;
}

void *realloc(void* ptr, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;

	GETTIME(time, ts);
	addr = realloc_fn(ptr, size);

	OBJWRITE("[%10lu]\trealloc: %p -> %p (%zu)\n",
			RELTIME(time), ptr, addr, size);
	return addr;
}

void free(void *ptr)
{
	struct timespec ts;
	unsigned long time;

	GETTIME(time, ts);
	free_fn(ptr);

	OBJWRITE("[%10lu]\t   free: %p\n", RELTIME(time), ptr);
	return;
}
