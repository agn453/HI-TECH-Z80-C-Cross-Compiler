/*
 *	calloc - alloc space for n items of size s, and clear it to nulls
 */

#include	<stdlib.h>

void *
calloc(n, s)
register size_t n, s;
{
	register char *cp, * xp;

	xp = cp = malloc(n *= s);
	if(cp == 0)
		return 0;
	while(n--)
		*cp++ = 0;
	return xp;
}
