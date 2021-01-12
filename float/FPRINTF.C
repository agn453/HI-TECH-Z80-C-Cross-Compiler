#include	<stdio.h>
#include	<stdarg.h>

fprintf(FILE * file, char * f, ...)
{
	va_list	ap;

	va_start(ap, f);
	return(vfprintf(file, f, ap));
}
