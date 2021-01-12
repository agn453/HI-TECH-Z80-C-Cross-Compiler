#include	<stdlib.h>

static	long	randx;
static	char	randf;

void
srand(x)
unsigned x;
{
	randx = x;
	randf = 1;
}

rand()
{
	if(!randf)
		srand(1);
	return(((randx = randx*1103515245L + 12345)>>16) & 077777);
}
