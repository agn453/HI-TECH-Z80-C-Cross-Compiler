/*
 *	HI-TECH C stdio fscanf
 */

#include	<stdio.h>
#include	<stdarg.h>

fscanf(FILE * file, char * fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	return vfscanf(file, fmt, ap);
}
