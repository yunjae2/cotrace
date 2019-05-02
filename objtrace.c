#include "objtrace.h"

void objtrace_init(void)
{
	objid = 0;

	malloc_fn = (malloc_t)dlsym(RTLD_NEXT, "malloc");
	calloc_fn = (calloc_t)dlsym(RTLD_NEXT, "calloc");
	realloc_fn = (realloc_t)dlsym(RTLD_NEXT, "realloc");
	free_fn = (free_t)dlsym(RTLD_NEXT, "free");
	fp_obj = fopen("obj.data", "w");
}

void objtrace_term(void)
{
	if (obj_buf_offset)
		flush_trace_buf(obj_buf, sizeof(struct obj_data),
				&obj_buf_offset, fp_obj);
	disable_objtrace = 1;
	fclose(fp_obj);
}

void *malloc(size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;
	struct obj_data odata;

	addr = malloc_fn(size);

	if (disable_objtrace)
		return addr;

	GETTIME(time, ts);
	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			RELTIME(time));
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

	if (disable_objtrace)
		return addr;

	GETTIME(time, ts);
	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			RELTIME(time));
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void *realloc(void* ptr, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time_free, time_alloc;
	struct obj_data odata;

	if (disable_objtrace) {
		addr = realloc_fn(ptr, size);
		return addr;
	}

	GETTIME(time_free, ts);
	OBJ_PACK(odata, -1, (unsigned long) ptr, 0, RELTIME(time_free));
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	addr = realloc_fn(ptr, size);

	GETTIME(time_alloc, ts);
	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			RELTIME(time_alloc));
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void free(void *ptr)
{
	struct timespec ts;
	unsigned long time;
	struct obj_data odata;

	if (disable_objtrace) {
		free_fn(ptr);
		return;
	}

	GETTIME(time, ts);
	free_fn(ptr);

	OBJ_PACK(odata, -1, (unsigned long) ptr, 0, RELTIME(time));
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return;
}
