#ifndef _TRACE_H
#define _TRACE_H

void trace_begin(void) __attribute__ ((constructor, no_instrument_function));
void trace_end(void) __attribute__ ((destructor, no_instrument_function));
void __cyg_profile_func_enter(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));


static FILE *fp_trace;

void trace_begin(void)
{
	fp_trace = fopen("trace.out", "w");
}

void trace_end(void)
{
	if (fp_trace)
		fclose(fp_trace);
}

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	fprintf(fp_trace, "func start: %p\n", call_site);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	fprintf(fp_trace, "func end: %p\n", call_site);
}

#endif
