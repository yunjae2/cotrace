#include "objtrace.h"

void objtrace_init(void)
{
	malloc_fn = (malloc_t)dlsym(RTLD_NEXT, "malloc");
	calloc_fn = (calloc_t)dlsym(RTLD_NEXT, "calloc");
	realloc_fn = (realloc_t)dlsym(RTLD_NEXT, "realloc");
	free_fn = (free_t)dlsym(RTLD_NEXT, "free");
	fp_obj = fopen("obj.data", "w");

	objid = 0;
}

void *malloc(size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;

	addr = malloc_fn(size);
	GETTIME(time, ts);

	OBJWRITE("[%10lu]\t obj %2d: %p (%zu)\n",
			RELTIME(time), objid++, addr, size);

	return addr;
}

void *calloc(size_t nmemb, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;

	addr = calloc_fn(nmemb, size);
	GETTIME(time, ts);

	OBJWRITE("[%10lu]\t obj %2d: %p (%zu)\n",
			RELTIME(time), objid++, addr, nmemb * size);

	return addr;
}

void *realloc(void* ptr, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time_free, time_alloc;

	GETTIME(time_free, ts);
	addr = realloc_fn(ptr, size);
	GETTIME(time_alloc, ts);

	OBJWRITE("[%10lu]\t   free: %p\n", RELTIME(time_free), ptr);
	OBJWRITE("[%10lu]\t obj %2d: %p (%zu)\n",
			RELTIME(time_alloc), objid++, addr, size);
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
