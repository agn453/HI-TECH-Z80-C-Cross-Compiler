#include	<stdio.h>
#include	<stdarg.h>

vprintf(char * f, va_list ap)
{
	return(vfprintf(stdout, f, ap));
}
