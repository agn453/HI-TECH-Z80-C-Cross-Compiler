#include	<stdio.h>
#include	<stdarg.h>

extern int	_doprnt(char *, char *, va_list);

vsprintf(char * wh, char * f, va_list ap)
{
	return _doprnt(wh, f, ap);
}
