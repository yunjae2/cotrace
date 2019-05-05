#ifndef _CTXTRACE_H
#define _CTXTRACE_H

#include "common.h"

#define CTX_MIN_RUNTIME		4 * 1000 * 1000	// In nanoseconds
#define MAX_TRACE_DEPTH		1000
#define TRACE_PUSH(data)	(ctx_stack[curr_ctx.depth] = data)
#define TRACE_POP()		(ctx_stack[curr_ctx.depth - 1])
#define UPDATE_CTX(_id, _addr, _start_time, _end_time, _depth)	{\
	curr_ctx.id = _id;\
	curr_ctx.addr = (unsigned long) _addr;\
	curr_ctx.start_time = _start_time;\
	curr_ctx.end_time = _end_time;\
	curr_ctx.depth = _depth;\
}


void trace_begin(void) __attribute__ ((constructor, no_instrument_function));
void trace_end(void) __attribute__ ((destructor, no_instrument_function));
void __cyg_profile_func_enter(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));

FILE *fp_ctx;
long nr_ctx;
struct ctx_data curr_ctx;
struct ctx_data ctx_stack[MAX_TRACE_DEPTH];

int ctx_buf_offset;
struct ctx_data ctx_buf[MAX_BUF_OFFSET];

#endif
