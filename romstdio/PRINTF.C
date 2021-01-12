#include	<stdio.h>
#include	<stdarg.h>

extern int	_doprnt(char *, char *, va_list);

printf(char * f, ...)
{
	va_list	ap;

	va_start(ap, f);
	return(_doprnt(0, f, ap));
}
