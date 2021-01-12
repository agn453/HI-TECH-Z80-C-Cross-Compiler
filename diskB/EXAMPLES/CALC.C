/* test program for Z80 - talks to a terminal via the serial port
 * and acts as a calculator. Accepted syntax is e.g.
 *	3 + 4
 *	123456 / 12345
 *
 *	Numbers are long (32 bits signed). The result is given as a 16 bit
 *	hex number followed by a long decimal number.
 *	Relational tests done with = *	operator, e.g. 1 = -1.
 *	The comparison will be done for unsigned long, signed long, signed
 *	int and unsigned int. For each type the set of { <, <=, ==, >, >= }
 *	which is true for that pair of numbers will be displayed.
 */

#include	<ctype.h>
#include	<stdlib.h>

void	putch(char);
char	getch(void);
char *	gets(char *);
void	putstr(char *);
void	putlong(long);
void	puthex(unsigned);
long	expr(void);
extern int	atoi();

main()
{
	register long	res;

	putstr("OK\n");
	for(;;) {
		res = expr();
		putstr("result = ");
		puthex((int)res);
		putstr("   ");
		putlong(res);
		putch('\n');
	}
}

long
expr()
{
	register long	a, b;
	register unsigned long au, bu;
	register int ai, bi;
	register unsigned aiu, biu;
	register char *	cp;
	register char	c;
	char	abuf[20];

	gets(abuf);
	cp = abuf;
	while(isspace(*cp))
		cp++;
	a = atol(cp);
	if(*cp == '-')
		cp++;
	while(*cp && isdigit(*cp))
		cp++;
	while(isspace(*cp))
		cp++;
	c = *cp;
	if(c == 0) {
		return a;
	}
	cp++;
	while(isspace(*cp))
		cp++;
	b = atol(cp);
	switch(c) {

	case '+':
		return a + b;

	case '-':
		return a - b;

	case '*':
		return a * b;

	case '/':
		return a / b;

	case '%':
		return a % b;

	case '^':
		return a ^ b;

	case '&':
		return a & b;

	case '|':
		return a | b;

	case '=':
		putstr("long: ");
		if(a < b)
			putstr(" < ");
		if(a == b)
			putstr(" == ");
		if(a > b)
			putstr(" > ");
		if(a >= b)
			putstr(" >= ");
		if(a <= b)
			putstr(" <= ");
		au = a;
		bu = b;
		putstr("\nunsigned long: ");
		if(au < bu)
			putstr(" < ");
		if(au == bu)
			putstr(" == ");
		if(au > bu)
			putstr(" > ");
		if(au >= bu)
			putstr(" >= ");
		if(au <= bu)
			putstr(" <= ");
		ai = a;
		bi = b;
		putstr("int: ");
		if(ai < bi)
			putstr(" < ");
		if(ai == bi)
			putstr(" == ");
		if(ai > bi)
			putstr(" > ");
		if(ai >= bi)
			putstr(" >= ");
		if(ai <= bi)
			putstr(" <= ");
		aiu = a;
		biu = b;
		putstr("\nunsigned int: ");
		if(aiu < biu)
			putstr(" < ");
		if(aiu == biu)
			putstr(" == ");
		if(aiu > biu)
			putstr(" > ");
		if(aiu >= biu)
			putstr(" >= ");
		if(aiu <= biu)
			putstr(" <= ");
		putch('\n');
		return 0;

	default:
		return 0;
	}
}
void
putlong(i)
register long	i;
{
	long	l;

	if(i < 0) {
		putch('-');
		i = -i;
	}
	if(l = i/10)
		putlong(l);
	putch((i % 10) + '0');
}

void
puthex(i)
register unsigned	i;
{
	register unsigned	j;

	if(j = i/16)
		puthex(j);
	putch("0123456789ABCDEF"[i & 0xF]);
}

void
putstr(register char * s)
{
	while(*s) {
		if(*s == '\n')
			putch('\r');
		putch(*s++);
	}
}

char *
gets(register char * buf)
{
	register char	c;
	register char *	cp;

	cp = buf;
	for(;;) {
		c = getch();
		putch(c);
		if(c == '\n') {
			*cp = 0;
			return buf;
		}
		*cp++ = c;
	}
}
