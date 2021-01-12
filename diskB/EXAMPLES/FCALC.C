/* 	Test program - this is a single-precision floating point
 *	calculator. Type in expressions of the form A op B, where A and B
 *	are floating point numbers, and op is an operator. The usual
 *	arithmetic operators are recognized, plus = for comparision,
 *	and s for sine. With s only the first number need be given,
 *	e.g. 30 s will give the answer 0.50000.
 *
 */

#include	<ctype.h>
#include	<math.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<conio.h>

float	expr(void);

main()
{
	register float	res;

	printf("\nHI-TECH C Compiler calculator on line\n");
	for(;;) {
		res = expr();
		printf("hex: res = %8.8lX, dec: res = %ld\n", res, (long)res);
		printf("Result = %f\n", res);
	}
}

float
expr()
{
	register float	a, b;
	register char *	cp;
	register char	c;
	char	abuf[20];

	gets(abuf);
	cp = abuf;
	while(isspace(*cp))
		cp++;
	a = atof(cp);
	if(*cp == '-')
		cp++;
	while(isdigit(*cp) || *cp == 'e' || *cp == 'E' || *cp == '.')
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
	b = atof(cp);
	switch(c) {

	case 's':
		return sin(a/180.0*3.141592);

	case '+':
		return a + b;

	case '-':
		return a - b;

	case '*':
		return a * b;

	case '/':
		return a / b;

	case '=':
		if(a < b)
			printf(" < ");
		if(a == b)
			printf(" == ");
		if(a > b)
			printf(" > ");
		if(a >= b)
			printf(" >= ");
		if(a <= b)
			printf(" <= ");
		putch('\n');
		return 0;

	default:
		return 0;
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