#include	<string.h>

memcmp(const void *s1, const void *s2, size_t n)
{
	short	i;
	register const char *	a1, * a2;

	a1 = s1;
	a2 = s2;
	while(n--)
		if(i = *a1++ - *a2++)
			return i;
	return 0;
}
