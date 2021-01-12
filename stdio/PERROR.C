#include	<stdio.h>
#include	<string.h>

static
ps(s)
char *	s;
{
	while(*s)
		putc(*s++, stderr);
}

void
perror(const char * s)
{
	ps(s);
	putc(':', stderr);
	ps(strerror(errno));
	putc('\n', stderr);
}
