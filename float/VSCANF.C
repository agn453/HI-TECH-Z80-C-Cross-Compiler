/*
 *	Stdio scanf
 */

#include	<stdio.h>
#include	<stdarg.h>

vscanf(char * fmt, va_list ap)
{
	return vfscanf(stdin, fmt, ap);
}
