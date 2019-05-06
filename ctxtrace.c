#include "ctxtrace.h"

void ctxtrace_init(void)
{
	UPDATE_CTX(0, 0, 0, 0, -1);	// Init context
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

void cotrace_begin(void)
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
	printf("cotrace initialized successfully!\n");
}

void cotrace_end(void)
{
	ctxtrace_term();
	objtrace_term();
}

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	struct timespec ts;
	unsigned long time;

	// Init context has depth of -1
	if (curr_ctx.depth != -1)
		TRACE_PUSH(curr_ctx);

	GETRELTIME(time, ts);
	UPDATE_CTX(nr_ctx++, this_fn, time, 0, curr_ctx.depth + 1);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	struct timespec ts;

	GETRELTIME(curr_ctx.end_time, ts);
	if (curr_ctx.end_time - curr_ctx.start_time >= CTX_MIN_RUNTIME) {
		TRACE_WRITE(ctx_buf, ctx_buf_offset, curr_ctx, fp_ctx);
	}

	// Do not pop when main() ends
	if (curr_ctx.depth != 0)
		curr_ctx = TRACE_POP();
}
