/*
 *	Stdio sscanf
 */

#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>

extern int	_doscan(char *, char *, va_list);

sscanf(char * str, char * fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	return _doscan(str, fmt, ap);
}
