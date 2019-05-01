#ifndef _CTXTRACE_H
#define _CTXTRACE_H

#include "common.h"

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

int buf_offset;
struct trace_data trace_buf[MAX_BUF_OFFSET];

#endif
