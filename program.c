#include <stdio.h>
#include "trace.h"

void baz(int level)
{
	if (level > 0)
		baz(level - 1);
	else
		printf("hello, world!\n");
}

void bar(void)
{
	baz(0);
}

void foo(void)
{
	bar();
	baz(2);
}

int main(void)
{
	foo();
	return 0;
}
