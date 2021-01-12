#include	<string.h>

void *
memcpy(void * d1, const void * s1, size_t n)
{
	if(n >= sizeof(int) * 4 && ((unsigned)d1 & (sizeof(int)-1)) == ((unsigned)s1 & (sizeof(int)-1))) {
		if((unsigned)d1 & (sizeof(int)-1)) { 

			register char *		d;
			register const char *	s;
			register unsigned	x;

			x = (unsigned)d1 & (sizeof(int)-1);
			d = d1;
			s = s1;
			while(x--)
				*d++ = *s++;
			d1 = d;
			s1 = d;
		}
		{
			register int *	d;
			register const int *	s;
			register size_t	x;

			x = n / sizeof(int);
			n -= x * sizeof(int);
			d = d1;
			s = s1;
			while(x--)
				*d++ = *s++;
			d1 = d;
			s1 = s;
		}
	}
	{
		register char *		d;
		register const char *	s;

		s = s1;
		d = d1;
		while(n--)
			*d++ = *s++;
		return d1;
	}
}
