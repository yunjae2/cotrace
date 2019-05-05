#ifndef _CTXTRACE_H
#define _CTXTRACE_H

#include "common.h"

#define CTX_MIN_RUNTIME		100	// In nanoseconds
#define MAX_TRACE_DEPTH		1000
#define CTX_PACK(pkg, _ctx, _depth, _addr, _start_time, _end_time)	{\
	pkg.ctx = _ctx;\
	pkg.depth= _depth;\
	pkg.addr = (unsigned long)_addr;\
	pkg.start_time = _start_time;\
	pkg.end_time = _end_time;\
}
#define CTX_UNPACK(pkg, _ctx, _addr, _start_time, _end_time)	{\
	_ctx = pkg.ctx;\
	_addr = pkg.addr;\
	_start_time = pkg.start_time;\
	_end_time = pkg.end_time;\
}
#define TRACE_PUSH(data)	(ctx_stack[ctx_depth++] = data)
#define TRACE_POP()		(ctx_stack[--ctx_depth])

struct ctx_data {
	int ctx;
	int depth;
	unsigned long addr;
	unsigned long start_time;
	unsigned long end_time;
};

void trace_begin(void) __attribute__ ((constructor, no_instrument_function));
void trace_end(void) __attribute__ ((destructor, no_instrument_function));
void __cyg_profile_func_enter(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));

int nr_ctx;
int curr_ctx;
int ctx_depth;
struct ctx_data ctx_stack[MAX_TRACE_DEPTH];

int ctx_buf_offset;
struct ctx_data ctx_buf[MAX_BUF_OFFSET];

#endif
