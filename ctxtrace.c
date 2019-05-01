#include "ctxtrace.h"

void ctxtrace_init(void)
{
	nr_ctx = 0;
	trace_depth = 0;
	buf_offset = 0;
	fp_ctx = fopen("ctx.data", "w");
}

void trace_begin(void)
{
	struct timespec ts;
	unsigned long res;

	GETRES(res, ts);
	GETTIME(start_time, ts);

	disable_objtrace = 1;
	objtrace_init();
	ctxtrace_init();
	disable_objtrace = 0;
}

void trace_end(void)
{
	if (buf_offset)
		flush_trace_buf(trace_buf, sizeof(struct trace_data),
				&buf_offset, fp_ctx);

	disable_objtrace = 1;
	fclose(fp_ctx);
	fclose(fp_obj);
}

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	struct timespec ts;
	unsigned long time;
	struct trace_data tdata;

	curr_ctx = nr_ctx++;
	GETTIME(time, ts);

	TRACE_PACK(tdata, curr_ctx, RELTIME(time));
	TRACE_WRITE(trace_buf, buf_offset, tdata, fp_ctx);
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
	TRACE_WRITE(trace_buf, buf_offset, tdata, fp_ctx);
}
