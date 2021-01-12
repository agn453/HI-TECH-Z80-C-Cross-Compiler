#include	<stdio.h>
#include	<stdarg.h>

sprintf(char * wh, char * f, ...)
{
	FILE	spf;
	va_list	ap;

	va_start(ap, f);
	spf._size = 32767;
	spf._cnt = 0;
	spf._base = spf._ptr = wh;
	spf._flag = _IOWRT|_IOBINARY|_IOSTRG;
	vfprintf(&spf, f, ap);
	*spf._ptr = 0;
	return spf._ptr - wh;
}
