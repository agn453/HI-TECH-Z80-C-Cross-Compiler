#include	<stdio.h>
#include	<stdarg.h>

extern int	_doprnt(char *, char *, va_list);

vprintf(char * f, va_list ap)
{
	return(_doprnt(0, f, ap));
}
