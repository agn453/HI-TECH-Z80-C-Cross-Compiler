/*
 *	puts and fputs for HI-TECH C stdio
 */

#include	<stdio.h>
#include	<conio.h>

puts(s)
char *		s;
{
	cputs(s);
	putch('\n');
}
