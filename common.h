#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
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

#define TRACE_WRITE(buf, offset, data, fp)	{\
	buf[offset++] = data;\
	if (offset >= MAX_BUF_OFFSET)\
		flush_trace_buf(buf, sizeof(data), &offset, fp);\
}

void flush_trace_buf(void *buf, size_t size, int *offset, FILE *fp);
void objtrace_init(void);

int disable_objtrace;
FILE *fp_ctx, *fp_obj;

unsigned long start_time;

#endif
