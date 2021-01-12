#include	<string.h>

void *
memset(void * p1, int c, register size_t n)
{
	register char *	p;

	p = p1;
	while(n--)
		*p++ = c;
	return p1;
}
