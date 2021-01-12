/*
 *	Stdio sscanf
 */

#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>

sscanf(char * str, char * fmt, ...)
{
	FILE	file;
	va_list	ap;

	va_start(ap, fmt);
	file._base = file._ptr = str;
	file._size = file._cnt = strlen(str);
	file._flag = _IOBINARY|_IOREAD|_IOSTRG;
	return vfscanf(&file, fmt, ap);
}
