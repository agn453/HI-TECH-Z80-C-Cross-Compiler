#include	<stdio.h>
#include	<stdarg.h>

printf(char * f, ...)
{
	va_list	ap;

	va_start(ap, f);
	return(vfprintf(stdout, f, ap));
}
