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
	struct obj_data odata;

	addr = malloc_fn(size);
	GETTIME(time, ts);

	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			time);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void *calloc(size_t nmemb, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;
	struct obj_data odata;

	addr = calloc_fn(nmemb, size);
	GETTIME(time, ts);

	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			time);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void *realloc(void* ptr, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time_free, time_alloc;
	struct obj_data odata;

	GETTIME(time_free, ts);

	OBJ_PACK(odata, -1, (unsigned long) ptr, 0, time_free);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	addr = realloc_fn(ptr, size);
	GETTIME(time_alloc, ts);

	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			time_alloc);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void free(void *ptr)
{
	struct timespec ts;
	unsigned long time;
	struct obj_data odata;

	GETTIME(time, ts);
	free_fn(ptr);

	OBJ_PACK(odata, -1, (unsigned long) ptr, 0, time);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return;
}
