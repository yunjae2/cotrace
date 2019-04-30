#include "ctxtrace.h"

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
