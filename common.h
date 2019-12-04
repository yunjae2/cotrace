#ifndef _COMMON_H
#define _COMMON_H

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <time.h>

/* Macros for measuring time */
#define S_IN_NS			(1000 * 1000 * 1000)
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
#define GETRELTIME(time, ts)	{\
	GETTIME(time, ts);\
	time = RELTIME(time);\
}

/* Macros for write buffer */
#define MAX_BUF_OFFSET		(100 * 1000)
#define TRACE_WRITE(buf, offset, data, fp)	{\
	buf[offset++] = data;\
	if (offset >= MAX_BUF_OFFSET)\
		flush_trace_buf(buf, sizeof(data), &offset, fp);\
}

struct ctx_data {
	long id;
	unsigned long addr;
	unsigned long start_time;
	unsigned long end_time;
	int depth;
};

void flush_trace_buf(void *buf, size_t size, int *offset, FILE *fp)
	__attribute__ ((no_instrument_function));
void objtrace_init(void) __attribute__ ((no_instrument_function));
void objtrace_term(void) __attribute__ ((no_instrument_function));
void ctxtrace_init(void) __attribute__ ((no_instrument_function));
void ctxtrace_term(void) __attribute__ ((no_instrument_function));

extern int disable_objtrace;
extern struct ctx_data curr_ctx;
extern FILE *fp_ctx, *fp_obj;

unsigned long start_time;
int in_dlsym;

#endif
