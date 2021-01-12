#include	<math.h>

#define	PI	3.14159265358979
#define	TWO_PI	6.28318530717958
#define	HALF_PI	1.570796326794895

double
asin(x)
double	x;
{
	double	y;
	double	sgn;

	if(fabs(x) > 1.0)
		return 0.0;
	sgn = 1.0;
	y = sqrt(1 - x*x);
	if(fabs(x) < 0.71)
		return atan(x/y);
	if(x < 0.0)
		sgn = -sgn;
	return sgn * (HALF_PI - atan(sgn * y/x));
}
