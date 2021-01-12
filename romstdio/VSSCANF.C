/*
 *	Stdio sscanf
 */

#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>

extern int	_doscan(char *, char *, va_list);

vsscanf(char * str, char * fmt, va_list ap)
{
	return _doscan(str, fmt, ap);
}
