#include <stdio.h>
#include <stdlib.h>

void baz(int level)
{
	if (level > 0)
		baz(level - 1);
	else
		printf("hello, world!\n");
}

void bar(void)
{
	int *ptr;

	baz(0);

	ptr = (int *)malloc(10 * sizeof(int));
	free(ptr);
	ptr = (int *)calloc(20, sizeof(int));
	ptr = (int *)realloc(ptr, 30000000 * sizeof(int));
	free(ptr);
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
