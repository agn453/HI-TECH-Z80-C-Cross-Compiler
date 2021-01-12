#include	<sys.h>

/*
 *	Formatted number printing for Z80 printf and debugger
 */
#define	NDIG	30		/* max number of digits to be printed */

_pnum(i, f, w, s, base, putch, upcase)
unsigned long	i;
unsigned char	base, upcase;
void	(*putch)(int);
unsigned char	s;
char	f, w;
{
	register char *	cp;
	char		fw;
	char		buf[NDIG];

	if(f > NDIG)
		f = NDIG;

	if(s && (long)i < 0)
		i = -i;
	else
		s = 0;
	if(f == 0 && i == 0)
		f++;

	cp = &buf[NDIG];
	while(i || f > 0) {
		*--cp = (upcase ? "0123456789ABCDEF" : "0123456789abcdef")[i%base];
		i /= base;
		f--;
	}
	fw = f = (&buf[NDIG] - cp) + s;
	if(fw < w)
		fw = w;
	while(w-- > f)
		putch(' ');
	if(s) {
		putch('-');
		f--;
	}
	while(f--)
		putch(*cp++);
	return fw;
}
