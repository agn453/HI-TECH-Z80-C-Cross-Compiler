/*
 *	_doscan - implement scanf, fscanf, sscanf
 */

#include	<stdio.h>
#include 	<ctype.h>
#include	<stdlib.h>
#include	<stdarg.h>

static FILE *	fp;

static uchar
range(c, base)
int	c;
uchar	base;
{
	if(isdigit(c))
		c -= '0';
	else
		{
		if (isupper(c))
			c = tolower(c) ;
		if (isalpha(c))
			c = c - 'a' + 10 ;
		else
			return 255;
		}
	if (c >= base)
		return 255;
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

vfscanf(FILE * file, char * fmt, va_list ap)
{
	uchar	c, sign, base, n, noass,len;
	char	width ;
	char *	sptr;
	int	ch;
	long	val;
	char	buf[60];

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

			case 'f':
				wspace();
				sptr = buf;
				if(width == 0)
					width = sizeof buf - 1;
				sign = 0;	/* really decimal point seen */
				ch = getc(fp);
				if(ch == '-') {
					*sptr++ = ch;
					ch = getc(fp);
					width--;
				}
				while(width && isdigit(ch) || !sign && ch == '.') {
					*sptr++ = ch;
					if(ch == '.')
						sign++;
					ch = getc(fp);
					width--;
				}
				if(width && (ch == 'e' || ch == 'E')) {
					*sptr++ = ch;
					ch = getc(fp);
					width--;
					if(width && (ch == '-' || ch == '+')) {
						*sptr++ = ch;
						ch = getc(fp);
						width--;
					}
					while(width && isdigit(ch)) {
						*sptr++ = ch;
						ch = getc(fp);
						width--;
					}
				}
				*sptr = 0;
				if(ch != EOF)
					ungetc(ch, fp);
				if(sptr == buf)
					return n ? n : feof(fp) ? EOF : 0;
				if(!noass) {
					n++;
					if(len == sizeof(long))
						*va_arg(ap, double *) = atof(buf);
					else
						*va_arg(ap, float *) = atof(buf);
				}
				continue;

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
					sptr = va_arg(ap, char *);
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
				if(!noass) {
					n++;
					*sptr = 0;
				}
				continue;

			case 'c':
				if ( !noass ) {
					n++;
					sptr = va_arg(ap, char *);
				}
				do {
					if ((ch = getc(fp)) == EOF) 
						return n ? n : EOF;
					if ( !noass )
						*sptr++ = ch;
				} while(--width > 0);
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
			if(range(ch, base) == 255) {
				ungetc(ch, fp);
				return n ? n : feof(fp) ? EOF : 0;
			}
			do {
				val = val * base + range(ch, base);
			} while (( --width != 0 ) && ( range(ch = getc(fp),base) != 255 )) ;
			if (range(ch,base) == 255)
				ungetc(ch, fp);
			if(sign)
				val = -val;
			if ( !noass ) {
				n++;
				if(len == sizeof(long))
					*va_arg(ap, long *) = val;
				else if(len == sizeof(short))
					*va_arg(ap, short *) = val;
				else
					*va_arg(ap, int *) = val;
			}
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
