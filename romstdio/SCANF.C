/*
 *	Stdio scanf
 */

#include	<stdio.h>
#include	<stdarg.h>

extern int	_doscan(char *, char *, va_list);

scanf(char * fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	return _doscan(0, fmt, ap);
}
