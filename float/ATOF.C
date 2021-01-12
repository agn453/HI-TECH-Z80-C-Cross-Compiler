#include	<ctype.h>
#include	<float.h>
#include	<math.h>

#define	NDIG	DBL_DIG+2

extern double	_bcdif(char *);

static double	powers[] =
{
	1e0,
	1e1,
	1e2,
	1e3,
	1e4,
	1e5,
	1e6,
	1e7,
	1e8,
	1e9,
	1e10,
#if	DBL_MAX_10_EXP >= 20
	1e20,
	1e30,
#if	DBL_MAX_10_EXP >= 40
	1e40,
	1e50,
	1e60,
	1e70,
	1e80,
	1e90,
#endif
#endif
};

static double	npowers[] =
{
	1e-0,
	1e-1,
	1e-2,
	1e-3,
	1e-4,
	1e-5,
	1e-6,
	1e-7,
	1e-8,
	1e-9,
	1e-10,
#if	DBL_MIN_10_EXP <= -20
	1e-20,
	1e-30,
#if	DBL_MIN_10_EXP <= -40
	1e-40,
	1e-50,
	1e-60,
	1e-70,
	1e-80,
	1e-90,
#endif
#endif
};
	
double
atof(s)
register char *	s;
{
	char	sign;
	double	l;
	short	expon;
	short	eexp;
	char	expsign;
	char	abuf[NDIG+2];
	register char *	cp;

	while(isspace(*s))
		s++;
	expsign = sign = 0;
	if(*s == '-') {
		s++;
		sign = 1;
	}
	l = 0;
	expon = 0;
	cp = abuf;
	while(isdigit(*s)) {
		if(cp == abuf+NDIG)
			expon++;
		else
			*cp++ = *s;
		s++;
	}
	if(*s == '.')
		while(isdigit(*++s)) {
			if(cp != abuf+NDIG) {
				expon--;
				*cp++ = *s;
			}
		}
	*cp = 0;
	eexp = 0;
	if(*s == 'e' || *s == 'E') {
		if(*++s == '-') {
			expsign = 1;
			s++;
		}
		if(*s == '+')
			s++;
		while(isdigit(*s))
			eexp = eexp*10 + *s++ - '0';
		if(expsign)
			eexp = -eexp;
	}
	expon += eexp;
	l = _bcdif(abuf);
	if(expon < 0) {
		if(expon < DBL_MIN_10_EXP)
			return sign ? -DBL_MIN : DBL_MIN;
		expon = -expon;
#if	DBL_MAX_10_EXP >= 100
		while(expon >= 100) {
			l *= 1e-100;
			expon -= 100;
		}
#endif
		if(expon >= 10) {
			l *= npowers[expon/10+9];
			expon %= 10;
		}
		if(expon)
			l *= npowers[expon];
	} else if(expon > 0) {
		if(expon > DBL_MAX_10_EXP)
			return sign ? -DBL_MAX : DBL_MAX;
#if	DBL_MAX_10_EXP >= 100
		while(expon >= 100) {
			l *= 1e100;
			expon -= 100;
		}
#endif
		if(expon >= 10) {
			l *= powers[expon/10+9];
			expon %= 10;
		}
		if(expon)
			l *= powers[expon];
	}
	if(sign)
		return -l;
	return l;
}
