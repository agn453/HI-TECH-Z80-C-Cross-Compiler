#include	<ctype.h>
#include	<stdlib.h>

long
atol(s)
register char *	s;
{
	register long	a;
	register unsigned char	sign;

	while(*s == ' ' || *s == '\t')
		s++;
	a = 0;
	sign = 0;
	if(*s == '-') {
		sign++;
		s++;
	}
	while(isdigit(*s))
		a = a*10L + (*s++ - '0');
	if(sign)
		return -a;
	return a;
}
