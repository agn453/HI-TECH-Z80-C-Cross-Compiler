#include	<conio.h>
#include	<stdlib.h>

srand1(s)
char *	s;
{
	int	i;
	while(*s)
		putch(*s++);
	while(!kbhit())
		i++;
	srand(i);
}
