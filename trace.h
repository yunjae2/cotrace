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

void trace_begin(void) __attribute__ ((constructor, no_instrument_function));
void trace_end(void) __attribute__ ((destructor, no_instrument_function));
void __cyg_profile_func_enter(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));


unsigned long start_time;
static FILE *fp_trace;

void trace_begin(void)
{
	struct timespec ts;

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

	GETTIME(time, ts);
	fprintf(fp_trace, "func start: %p\t\t time: %lu\n",
			call_site, RELTIME(time));
}

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	struct timespec ts;
	unsigned long time;

	GETTIME(time, ts);
	fprintf(fp_trace, "func end: %p\t\t time: %lu\n",
			call_site, RELTIME(time));
}

#endif
