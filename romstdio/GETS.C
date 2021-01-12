/*
 *	gets and fgets for HI-TECH C stdio
 */

#include	<stdio.h>
#include	<string.h>
#include	<conio.h>

char *
gets(s)
char *	s;
{
	return cgets(s);
}
