/*
 *	Stdio scanf
 */

#include	<stdio.h>
#include	<stdarg.h>

scanf(char * fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	return vfscanf(stdin, fmt, ap);
}
