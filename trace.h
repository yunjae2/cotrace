#ifndef _TRACE_H
#define _TRACE_H

#include <time.h>

#define S_IN_NS			1000000000
#define GETRES(res, ts)	{\
	clock_getres(CLOCK_REALTIME, &ts);\
	res = ((unsigned long)(ts.tv_sec) * S_IN_NS +\
			(unsigned long)ts.tv_nsec);\
}
#define GETTIME(time, ts)	{\
	clock_gettime(CLOCK_REALTIME, &ts);\
	time = ((unsigned long)(ts.tv_sec) * S_IN_NS +\
			(unsigned long)ts.tv_nsec);\
}
#define RELTIME(time)		(time - start_time)

#define MAX_TRACE_DEPTH		1000
#define MAX_BUF_OFFSET		1000
#define TRACE_PACK(pkg, _ctx, _time)	{\
	pkg.ctx = _ctx;\
	pkg.time = _time;\
}
#define TRACE_UNPACK(pkg, _ctx, _time)	{\
	_ctx = pkg.ctx;\
	_time = pkg.time;\
}
#define TRACE_PUSH(data)	(trace_stack[trace_depth++] = data)
#define TRACE_POP()		(trace_stack[--trace_depth])
#define TRACE_WRITE(data)	{\
	trace_buf[buf_offset++] = data;\
	if (buf_offset >= MAX_BUF_OFFSET)\
		flush_trace_buf();\
}


struct trace_data {
	int ctx;
	unsigned long time;
};

void flush_trace_buf(void) __attribute__ ((no_instrument_function));
void trace_begin(void) __attribute__ ((constructor, no_instrument_function));
void trace_end(void) __attribute__ ((destructor, no_instrument_function));
void __cyg_profile_func_enter(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));

int nr_ctx;
int curr_ctx;
int trace_depth;
struct trace_data trace_stack[MAX_TRACE_DEPTH];
unsigned long start_time;
static FILE *fp_trace;

int buf_offset;
struct trace_data trace_buf[MAX_BUF_OFFSET];


void flush_trace_buf(void)
{
	fwrite(trace_buf, sizeof(struct trace_data) * buf_offset, 1, fp_trace);
	buf_offset = 0;
}

void trace_begin(void)
{
	struct timespec ts;
	unsigned long res;

	nr_ctx = 0;
	trace_depth = 0;
	buf_offset = 0;

	fp_trace = fopen("trace.out", "w");

	GETRES(res, ts);
	GETTIME(start_time, ts);
	printf("struct size: %ld\n", sizeof(struct trace_data));
}

void trace_end(void)
{
	if (buf_offset)
		flush_trace_buf();

	if (fp_trace)
		fclose(fp_trace);
}

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	struct timespec ts;
	unsigned long time;
	struct trace_data tdata;

	curr_ctx = nr_ctx++;
	GETTIME(time, ts);

	TRACE_PACK(tdata, curr_ctx, RELTIME(time));
	TRACE_WRITE(tdata);
	TRACE_PUSH(tdata);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	struct timespec ts;
	unsigned long ctx_start_time;
	unsigned long ctx_end_time;
	struct trace_data tdata;

	tdata = TRACE_POP();
	TRACE_UNPACK(tdata, curr_ctx, ctx_start_time);

	GETTIME(ctx_end_time, ts);
	TRACE_PACK(tdata, curr_ctx, RELTIME(ctx_end_time));
	TRACE_WRITE(tdata);
}

#endif
