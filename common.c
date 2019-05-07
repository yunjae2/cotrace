#include "common.h"

void flush_trace_buf(void *buf, size_t size, int *offset, FILE *fp)
{
	int disable_objtrace_save = disable_objtrace;
	disable_objtrace = 1;
	if (fwrite(buf, size, *offset, fp) != *offset)
		printf("fwrite error!\n");
	*offset = 0;
	disable_objtrace = disable_objtrace_save;
}
