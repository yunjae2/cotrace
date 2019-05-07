#include "mhint.h"

void import_hint(void)
{
	FILE *fp;
	long _objid;
	unsigned long _offset, _size;
	int i;

	internal_path = 1;

	fp = fopen(hint_path, "r");
	fscanf(fp, "%d", &nr_hints);
	mhint = malloc_fn(nr_hints * sizeof(struct hint_data));

	for (i = 0; i < nr_hints; i++) {
		fscanf(fp, "%ld %lu %lu\n", &_objid, &_offset, &_size);
		mhint[i].objid = _objid;
		mhint[i].offset = _offset;
		mhint[i].size = _size;
	}

	internal_path = 0;
}

void alloc_init(void)
{
	malloc_fn = (malloc_t)dlsym(RTLD_NEXT, "malloc");
	calloc_fn = (calloc_t)dlsym(RTLD_NEXT, "calloc");
	realloc_fn = (realloc_t)dlsym(RTLD_NEXT, "realloc");
	free_fn = (free_t)dlsym(RTLD_NEXT, "free");

	import_hint();
}

void *malloc(size_t size)
{
	void *addr;

	addr = malloc_fn(size);
	if (internal_path)
		return addr;

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
	if (internal_path)
		return addr;

	CHECK_HINT(objid, addr);
	objid++;

	return addr;
}

void *realloc(void* ptr, size_t size)
{
	void *addr;

	addr = realloc_fn(ptr, size);
	if (internal_path)
		return addr;

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
