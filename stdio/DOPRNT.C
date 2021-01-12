#include	<stdio.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdarg.h>
#include	<sys.h>



/*
 *	doprnt for Z80
 */

static uchar	ival;
static char *	x;
static FILE *	ffile;

static void
pputc(register int c)
{
	putc(c, ffile);
}

static char *
icvt(cp)
register char *	cp;
{
	ival = atoi(cp);
	while(isdigit((unsigned)*cp))
		cp++;
	return cp;
}

vfprintf(FILE * file, register char * f, register va_list arl)
{
	char	c;
	uchar	fill, left;
	unsigned short	i;
	unsigned short	base, prec, sign;
	unsigned short	width, len, upcase;
	unsigned long	val;

	ffile = file;
	while(c = *f++)
		if(c != '%')
			pputc(c);
		else {
			base = 10;
			width = 0;
			sign = 0;
			left = 0;
			upcase = 0;
			len = sizeof(int);
			if(*f == '-') {
				f++;
				left++;
			}
			fill = *f == '0';
			if(isdigit((unsigned)*f)) {
				f = icvt(f);
				width = ival;
			} else if(*f == '*') {
				width = va_arg(arl, int);
				f++;
			}
			if(*f == '.')
				if(*++f == '*') {
					prec = va_arg(arl, int);
					f++;
				} else {
					f = icvt(f);
					prec = ival;
				}
			else
				prec = fill ? width : 0;
			if(*f == 'l') {
				f++;
				len = sizeof(long);
			}
			switch(c = *f++) {

			case 0:
				return;
			case 'o':
				base = 8;
				break;
			case 'd':
				sign = 1;
				break;

			case 'X':
				upcase = 1;
			case 'x':
				base = 16;
				break;

			case 's':
				x = va_arg(arl, char *);
				if(!x)
					x = "(null)";
				i = strlen(x);
dostring:
				if(prec && prec < i)
					i = prec;
				if(width > i)
					width -= i;
				else
					width = 0;
				if(!left)
					while(width--)
						pputc(' ');
				while(i--)
					pputc(*x++);
				if(left)
					while(width--)
						pputc(' ');
				continue;
			case 'c':
				c = va_arg(arl, int);
			default:
				x = &c;
				i = 1;
				goto dostring;

			case 'u':
				break;
			}
			if(left) {
				left = width;
				width = 0;
			}
			if(len == sizeof(long))
				val = va_arg(arl, long);
			else if(sign)
				val = (long)va_arg(arl, int);
			else
				val = va_arg(arl, unsigned);
			width = _pnum(val, prec, width, sign, base, pputc, upcase);
			while(left-- > width)
				pputc(' ');
		}
}
