#include	<unixio.h>
#include	<stdio.h>

remove(s)
char *	s;
{
	return unlink(s);
}
