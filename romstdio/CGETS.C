#include	<conio.h>

char *
cgets(s)
char *	s;
{
	register char *	s1 = s;
	int	c;

	for(;;) {
		switch(c = getche()) {

		case '\b':
			if(s == s1)
				break;
			putch(' ');
			putch('\b');
			s1--;
			break;

		case 'U'-'@':
			while(s1 > s) {
				putch('\b');
				putch(' ');
				putch('\b');
				s1--;
			}
			break;

		case '\n':
		case '\r':
			*s1 = 0;
			return s;

		default:
			*s1++ = c;
			break;
		}
	}
}
