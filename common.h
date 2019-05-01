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

int disable_objtrace;
FILE *fp_ctx, *fp_obj;

unsigned long start_time;

#endif
