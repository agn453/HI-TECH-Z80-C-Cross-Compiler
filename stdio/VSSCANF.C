/*
 *	Stdio sscanf
 */

#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>

vsscanf(char * str, char * fmt, va_list ap)
{
	FILE	file;

	file._base = file._ptr = str;
	file._size = file._cnt = strlen(str);
	file._flag = _IOBINARY|_IOREAD|_IOSTRG;
	return vfscanf(&file, fmt, ap);
}
