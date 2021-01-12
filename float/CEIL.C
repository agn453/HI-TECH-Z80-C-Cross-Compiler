#include	<limits.h>
#include	<math.h>

extern double	_frndint(double);

double
ceil(x)
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
	if(i < x)
		return i + 1.0;
	return i;
}
