#include "ctxtrace.h"

void ctxtrace_init(void)
{
	nr_ctx = 0;
	ctx_depth = 0;
	ctx_buf_offset = 0;
	fp_ctx = fopen("ctx.data", "w");
}

void ctxtrace_term(void)
{
	int disable_objtrace_save = disable_objtrace;

	if (ctx_buf_offset)
		flush_trace_buf(ctx_buf, sizeof(struct ctx_data),
				&ctx_buf_offset, fp_ctx);
	disable_objtrace = 1;
	fclose(fp_ctx);
	disable_objtrace = disable_objtrace_save;
}

void trace_begin(void)
{
	struct timespec ts;
	unsigned long res;
	int disable_objtrace_save = disable_objtrace;

	disable_objtrace = 1;
	GETRES(res, ts);
	GETTIME(start_time, ts);

	objtrace_init();
	ctxtrace_init();
	disable_objtrace = disable_objtrace_save;
}

void trace_end(void)
{
	ctxtrace_term();
	objtrace_term();
}

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	struct timespec ts;
	unsigned long time;
	struct ctx_data cdata;

	curr_ctx = nr_ctx++;
	GETTIME(time, ts);

	CTX_PACK(cdata, curr_ctx, this_fn, RELTIME(time));
	TRACE_WRITE(ctx_buf, ctx_buf_offset, cdata, fp_ctx);
	TRACE_PUSH(cdata);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	struct timespec ts;
	unsigned long ctx_start_time;
	unsigned long ctx_end_time;
	struct ctx_data cdata;

	cdata = TRACE_POP();
	CTX_UNPACK(cdata, curr_ctx, ctx_start_time);

	GETTIME(ctx_end_time, ts);
	CTX_PACK(cdata, curr_ctx, this_fn, RELTIME(ctx_end_time));
	TRACE_WRITE(ctx_buf, ctx_buf_offset, cdata, fp_ctx);
}
