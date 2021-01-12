/*
 *	Stdio scanf
 */

#include	<stdio.h>
#include	<stdarg.h>

extern int	_doscan(char *, char *, va_list);

vscanf(char * fmt, va_list ap)
{
	return _doscan(0, fmt, ap);
}
