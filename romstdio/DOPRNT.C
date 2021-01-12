#include	<stdio.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdarg.h>
#include	<conio.h>
#include	<sys.h>


/*
 *	doprnt for ROM
 *	All output goes via putch unless it is going to a string
 */

#define	NDIG	11		/* max number of digits to be printed */

static char *	sptr;

#define	pputc(c)	if(sptr) { *sptr++= (c);} else putch((c))

_doprnt(char * str, register char * f, register va_list ap)
{
	short	prec;
	char	c;
	uchar	fill, left;
	short	i;
	uchar	base, sign;
	unsigned short	width, len, upcase, fw;
	unsigned long	val;
	register char *	cp;
	char	buf[NDIG];

	sptr = str;
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
				width = atoi(f);
				while(isdigit((unsigned)*f))
					f++;
			} else if(*f == '*') {
				width = va_arg(ap, int);
				f++;
			}
			if(*f == '.')
				if(*++f == '*') {
					prec = va_arg(ap, int);
					f++;
				} else {
					prec = atoi(f);
					while(isdigit((unsigned)*f))
						f++;
				}
			else
				prec = fill ? width : -1;
			if(*f == 'l') {
				f++;
				len = sizeof(long);
			}
			switch(c = *f++) {

			case 0:
				if(str) {
					*sptr++ = 0;
					return sptr - str;
				}
				return 0;
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
				cp = va_arg(ap, char *);
				if(!cp)
					cp = "(null)";
				i = strlen(cp);
dostring:
				if(prec < 0)
					prec = 0;
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
					pputc(*cp++);
				if(left)
					while(width--)
						pputc(' ');
				continue;
			case 'c':
				c = va_arg(ap, int);
			default:
				cp = &c;
				i = 1;
				goto dostring;

			case 'u':
				break;

			}
			if(left) {
				left = width;
				width = 0;
			}
			if(prec < 0)
				prec = 0;
			if(len == sizeof(long))
				val = va_arg(ap, long);
			else if(sign)
				val = (long)va_arg(ap, int);
			else
				val = va_arg(ap, unsigned);
			if(prec > NDIG)
				prec = NDIG;
		
			if(sign && (long)val < 0)
				val = -val;
			else
				sign = 0;
			if(prec == 0 && val == 0)
				prec++;
		
			cp = &buf[NDIG];
			while(val || prec > 0) {
				*--cp = (upcase ? "0123456789ABCDEF" : "0123456789abcdef")[val%base];
				val /= base;
				prec--;
			}
			fw = prec = (&buf[NDIG] - cp) + sign;
			if(fw < width)
				fw = width;
			while(width-- > prec)
				pputc(' ');
			if(sign) {
				putch('-');
				prec--;
			}
			while(prec--)
				putch(*cp++);
			while(left-- > fw)
				pputc(' ');
		}
	if(str) {
		*sptr++ = 0;
		return sptr - str;
	}
	return 0;
}
