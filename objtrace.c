#include "objtrace.h"

void objtrace_init(void)
{
	objid = 0;
	fp_obj = fopen("obj.data", "w");
}

void objtrace_term(void)
{
	if (obj_buf_offset) {
		flush_trace_buf(obj_buf, sizeof(struct obj_data),
				&obj_buf_offset, fp_obj);
		printf("objtrace buffer flushed!\n");
	}

	disable_objtrace = 1;
	fclose(fp_obj);
}

void *malloc(size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;
	struct obj_data odata;

	if (malloc_fn == NULL) {
		in_dlsym = 1;
		malloc_fn = (malloc_t)dlsym(RTLD_NEXT, "malloc");
		in_dlsym = 0;
	}

	addr = malloc_fn(size);

	if (disable_objtrace)
		return addr;

	GETTIME(time, ts);
	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			RELTIME(time), curr_ctx.id, curr_ctx.addr);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void *calloc(size_t nmemb, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time;
	struct obj_data odata;

	/* dlsym() calls calloc() internally, so deal with it */
	if (in_dlsym)
		return calloc_buffer;

	if (calloc_fn == NULL) {
		in_dlsym = 1;
		calloc_fn = (calloc_t)dlsym(RTLD_NEXT, "calloc");
		in_dlsym = 0;
	}

	addr = calloc_fn(nmemb, size);

	if (disable_objtrace)
		return addr;

	GETTIME(time, ts);
	OBJ_PACK(odata, objid++, (unsigned long) addr,
			(unsigned long) (nmemb * size), RELTIME(time),
			curr_ctx.id, curr_ctx.addr);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void *realloc(void* ptr, size_t size)
{
	void *addr;
	struct timespec ts;
	unsigned long time_free, time_alloc;
	struct obj_data odata;

	if (realloc_fn == NULL) {
		in_dlsym = 1;
		realloc_fn = (realloc_t)dlsym(RTLD_NEXT, "realloc");
		in_dlsym = 0;
	}

	if (disable_objtrace) {
		addr = realloc_fn(ptr, size);
		return addr;
	}

	GETTIME(time_free, ts);
	OBJ_PACK(odata, -1, (unsigned long) ptr, 0, RELTIME(time_free),
			curr_ctx.id, curr_ctx.addr);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	addr = realloc_fn(ptr, size);

	GETTIME(time_alloc, ts);
	OBJ_PACK(odata, objid++, (unsigned long) addr, (unsigned long) size,
			RELTIME(time_alloc), curr_ctx.id, curr_ctx.addr);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return addr;
}

void free(void *ptr)
{
	struct timespec ts;
	unsigned long time;
	struct obj_data odata;

	if (free_fn == NULL) {
		in_dlsym = 1;
		free_fn = (free_t)dlsym(RTLD_NEXT, "free");
		in_dlsym = 0;
	}

	if (ptr == calloc_buffer)
		return;

	if (disable_objtrace) {
		free_fn(ptr);
		return;
	}

	GETTIME(time, ts);
	free_fn(ptr);

	OBJ_PACK(odata, -1, (unsigned long) ptr, 0, RELTIME(time), curr_ctx.id,
			curr_ctx.addr);
	TRACE_WRITE(obj_buf, obj_buf_offset, odata, fp_obj);

	return;
}
