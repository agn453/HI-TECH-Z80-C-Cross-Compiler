#include	<math.h>
#include	<stdlib.h>

/*	This _fibcd() only works where doubles are the same
	as float, i.e. 32 bits */

unsigned
_fibcd(double val, register char * buf)
{
	register unsigned long	l;
	register unsigned char	cnt;

	cnt = 0;
	*--buf = 0;
	l = (unsigned long)val;
	do {
		*--buf = l % 10 + '0';
		cnt++;
	} while(l /= 10);
	return cnt;
}

float
_bcdif(register char *	buf)
{
	long	l;

	l = atol(buf);
	return (float)l;
}
