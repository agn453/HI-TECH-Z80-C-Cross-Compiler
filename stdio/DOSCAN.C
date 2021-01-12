/*
 *	_doscan - implement scanf, fscanf, sscanf
 */

#include	<stdio.h>
#include 	<ctype.h>
#include	<stdlib.h>
#include	<stdarg.h>


static FILE *	fp;

static
range(c, base)
int	c;
uchar	base;
{
	if(isdigit(c))
		c -= '0';
	else {
		if (isupper(c))
			c = tolower(c) ;
		if (isalpha(c))
			c = c - 'a' + 10 ;
		else
			return -1 ;
	}
	if (c >= base)
		return -1 ;
	return c ;
}

static
wspace()
{
	int	c;

	while(isspace(c = getc(fp)))
		continue;
	if(c != EOF)
		ungetc(c, fp);
}

vfscanf(FILE * file, register char * fmt, register va_list arl)
{
	uchar	c, sign, base, n, noass,len;
	char	width ;
	char *	sptr;
	int	ch;
	long	val;

	fp = file;
	n = 0;
	while(c = *fmt++) {

		len = sizeof(int);
		if(isspace(c)) {
			wspace();
			continue;
		}
		if(c == '%') {
			noass = 0;
			width = 0;
loop:
			switch(c = *fmt++) {

			case '\0':
				return n ? n : feof(fp) ? EOF : 0;

			case '*':
				noass++;
				goto loop;

			case 'l':
				len = sizeof(long);
				goto loop;
			case 'h':
				len = sizeof(short);
				goto loop;

			case 'd':
			case 'u':
				base = 10;
				break;

			case 'o':
				base = 8;
				break;

			case 'x':
				base = 16;
				break ;

			case 's':
				wspace();
				if ( !noass )
					sptr = va_arg(arl, char *);
				if ((ch = getc(fp)) == EOF )
					return n ? n : EOF;
				while(ch && ch != EOF && !isspace(ch)) {
					if(ch == *fmt) {
						fmt++;
						break;
					}
					if ( !noass ) 
						*sptr++ = ch;
					if(--width == 0)
						break;
					ch = getc(fp);
				}
				if ( !noass )  {
					n++;
					*sptr = 0;
				}
				continue;

			case 'c':
				if ( !noass )
					sptr = va_arg(arl, char *);
				do {
					if ((ch = getc(fp)) == EOF) 
						return n ? n : EOF;
					if ( !noass )
						*sptr++ = ch;
				} while(--width > 0);
				if(!noass)
					n++;
				continue;
			default:
				if(isdigit(c)) {
					width = atoi(fmt-1);
					while(isdigit(*fmt))
						fmt++;
					goto loop;
				}
				if(c != (ch = getc(fp)))
					if(ch == EOF)
						return n ? n : EOF;
					else {
						ungetc(ch, fp);
						return n;
					}
				continue;
			}
			wspace();
			val = 0;
			sign = 0;
			ch = getc(fp);
			if(ch == '-') {
				sign++;
				ch = getc(fp);
			}
			if(range(ch, base) == -1) {
				ungetc(ch, fp);
				return n ? n : feof(fp) ? EOF : 0;
			}
			do {
				val = val * base + (uchar)range(ch, base);
			} while (( --width != 0 ) && ( range(ch = getc(fp),base) != -1 )) ;
			if (range(ch,base) == -1)
				ungetc(ch, fp);
			if(sign)
				val = -val;
			if ( !noass )
				n++;
				if(len == sizeof(long))
					*va_arg(arl, long *) = val;
				else if(len == sizeof(short))
					*va_arg(arl, short *) = val;
				else
					*va_arg(arl, int *) = val;
			continue;
		} else if(c != (ch = getc(fp))) {
			if(ch != EOF) {
				ungetc(ch, fp);
				return n;
			} else
				return n ? n : EOF;
		}
	}
	return n;
}
