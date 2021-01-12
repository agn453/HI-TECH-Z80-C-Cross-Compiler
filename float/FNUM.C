/*
 *	_fnum() - converts floating numbers to ascii decimal
 *	representations.
 */

#include	<string.h>
#include	<stdlib.h>
#include	<math.h>
#include	<float.h>
#include	<sys.h>

#define	uchar	unsigned char
#define	putc(c)	(*xputc)(c)

#define	NDIG	DBL_DIG+1
extern double	_frndint(double);
extern unsigned	_fibcd(double, char *);

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

void (*	xputc)(int);

/* this routine returns a value to round to the number of decimal
	places specified */

double
fround(int prec)
{
	/* prec is guaranteed to be less than NDIG */

	double	delta;

	delta = 0.5;
#if	NDIG >= 10
	if(prec >= 10) {
		delta *= npowers[prec/10];
		prec %= 10;
	}
#endif
	if(prec)
		return delta * npowers[prec];
	return delta;
}

/* this routine scales a double by the a number equal to 1 to the decimal
   power supplied */

static double
scale(val, scl)
double val;
int	scl;
{
	register int	texp;

	if(scl < 0) {
		texp = -scl;
#if	DBL_MIN_10_EXP < -20
		while(texp > 100) {
			val *= 1e-100;
			texp -= 100;
		}
#endif
		if(texp > 10) {
			val *= npowers[texp/10 + 9];
			texp %= 10;
		}
		if(texp)
			val *= npowers[texp];
	} else if(scl > 0) {
		texp = scl;
#if	DBL_MAX_10_EXP > 20
		while(texp > 100) {
			val *= 1e100;
			texp -= 100;
		}
#endif
		if(texp > 10) {
			val *= powers[texp/10 + 9];
			texp %= 10;
		}
		if(texp)
			val *= powers[texp];
	}
	return val;
}

/*
 *	Put a fractional part, number of digits is prec.
 */

static void
putfrac(frac, prec)
double	frac;
int	prec;
{
	register int	oprec, digs;
	register char *	cp;
	char	buf[NDIG+4];

	if(prec > NDIG) 
		oprec = NDIG;
	else
		oprec = prec;
	putc('.');
	frac = scale(frac, oprec);
	digs = _fibcd(frac, buf+NDIG+4);
	cp = buf+NDIG-digs+3;
	while(digs < oprec) {
		putc('0');
		digs++;
	}
	while(*cp)
		putc(*cp++);
	while(oprec != prec) {
		putc('0');
		oprec++;
	}
}

/* efmt is 0 for f format, 1 for g format, and 2 for e format */

_fnum(val, prec, width, efmt, pputc, upcase)
double	val;
short	width, prec, efmt, upcase;
void (*	pputc)(int);
{
	register char *	cp;
	int		exp;
	short		digs;
	short		twid;
	uchar		sign;
	double		frac, integ, integ1;
	char		buf[NDIG+4];

	xputc = pputc;
	if(prec < 0)
		prec = 6;
	else if(prec > NDIG)
		prec = NDIG;
	twid = 0;
	if(val < 0.0) {
		val = -val;
		sign = 1;
	} else
		sign = 0;
	exp = 0;
	frexp(val, &exp);		/* get binary exponent */
	exp--;				/* adjust 0.5 -> 1.0 */
	exp *= 3;
	exp /= 10;			/* estimate decimal exponent */
	frac = modf(val, &integ);
	if(exp <= NDIG)
		digs = _fibcd(integ, buf+NDIG+4);
	else
		digs = exp;
	cp = buf+NDIG-digs+3;
	if(efmt == 0 && prec == 0 || efmt == 1 && frac == 0.0 && digs <= prec) {
		/* using d format */

		twid = digs;
		width -= digs;
		while(width > 0) {
			putc(' ');
			width--;
			twid++;
		}
		while(*cp)
			putc(*cp++);
		return twid;
	}

	if(exp < -4 || exp >= prec)	/* convert g format to e format? */
		efmt++;
	if(efmt >= 2) {		/* use e format */
		if(exp > 0) {
			val = scale(val, -exp);
		} else if(exp < 0) {
			val = scale(val, -exp);
		}
		while(val >= 10.0) {
			val *= 1e-1;
			exp++;
		}
		while(val < 1.0) {
			val *= 10.0;
			exp--;
		}
		frac = modf(val, &val);
		width -= (twid = (short)sign + prec + 6);
		if(abs(exp) >= 100)		/* 3 digit exponent */
			width--;
		while(width > 0) {
			putc(' ');
			width--;
			twid++;
		}
		if(sign)
			putc('-');
		putc((int)val + '0');
		putfrac(frac, prec);
		if(upcase)
			putc('E');
		else
			putc('e');
		if(exp < 0) {
			exp = -exp;
			putc('-');
		} else
			putc('+');
		if(exp >= 100) {
			putc(exp / 100 + '0');
			exp %= 100;
		}
		putc(exp / 10 + '0');
		putc(exp % 10 + '0');
		return twid;
	}
	/* here for f format */

	modf(val+fround(prec), &integ1);	/* test for rounding */
	if(integ1 != integ) {
		val += fround(prec);		/* round the value */
		frac = modf(val, &integ);
	}
	frexp(integ, &exp);		/* get binary exponent */
	exp--;				/* adjust 0.5 -> 1.0 */
	exp *= 3;
	exp /= 10;			/* estimate decimal exponent */
	if(exp > NDIG) {
		exp -= NDIG;
		integ = scale(integ, -exp);
	} else
		exp = 0;
	digs = _fibcd(integ, buf+NDIG+4);
	cp = buf+NDIG-digs+3;
	twid = 1 + prec + (short)sign + digs + exp;
	width -= twid;
	while(width > 0) {
		putc(' ');
		width--;
	}
	if(sign)
		putc('-');
	while(*cp) {
		putc(*cp++);
	}
	while(exp > 0) {
		putc('0');
		exp--;
	}
	putfrac(frac, prec);
	return twid;
}
