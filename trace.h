#ifndef _TRACE_H
#define _TRACE_H

#include <time.h>

#define S_IN_NS			1000000000
#define GETTIME(time, ts)	{\
	clock_gettime(CLOCK_REALTIME, &ts);\
	time = ((unsigned long)(ts.tv_sec) * S_IN_NS +\
			(unsigned long)ts.tv_nsec);\
}
#define RELTIME(time)		(time - start_time)

#define MAX_TRACE_DEPTH		1000
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


struct trace_data {
	int ctx;
	unsigned long time;
};

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

void trace_begin(void)
{
	struct timespec ts;

	nr_ctx = 0;
	trace_depth = 0;

	fp_trace = fopen("trace.out", "w");
	GETTIME(start_time, ts);
}

void trace_end(void)
{
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
	fprintf(fp_trace, "ctx start: %d\t\t time: %lu\n",
			curr_ctx, RELTIME(time));

	TRACE_PACK(tdata, curr_ctx, time);
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
	fprintf(fp_trace, "ctx end: %d\t\t time: %lu\n",
			curr_ctx, RELTIME(ctx_end_time));
}

#endif
