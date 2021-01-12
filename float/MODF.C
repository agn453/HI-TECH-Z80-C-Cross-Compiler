#include	<math.h>

double
modf(value, iptr)
double	value, * iptr;
{
	double	x;

	x = value >= 0 ? floor(value) : ceil(value);
	*iptr = x;
	return value - x;
}
