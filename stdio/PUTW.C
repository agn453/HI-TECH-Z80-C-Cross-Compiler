#include	<stdio.h>

/*
 *	putw for HI-TECH C
 */

putw(w, stream)
register FILE *	stream;
{
	if(putc(w, stream) == EOF || putc((w >> 8), stream) == EOF)
		return(EOF);
	return(w);
}
