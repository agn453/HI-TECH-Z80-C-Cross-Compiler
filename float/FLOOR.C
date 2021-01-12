#include	<math.h>
#include	<limits.h>

extern double	_frndint(double);

double
floor(x)
double	x;
{
	double	i;
	int	expon;

	i = frexp(x, &expon);
	if(expon < 0)
		return 0.0;
	if((unsigned)expon > sizeof(double) * CHAR_BIT - 4)
		return x;		/* already an integer */
	i = _frndint(x);
	if(i > x)
		return i - 1.0;
	return i;
}
