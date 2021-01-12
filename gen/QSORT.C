/*
 *	Quicksort based on the algorithm given in
 *	"Algorithms + Data Structures = Programs" by N. Wirth.
 */

#include	<stdlib.h>
#include	<string.h>

extern void	_swap(unsigned, void *, void *);

void
qsort(void * vbase, size_t nel, size_t width, int (*compar)(void *, void *))
{
	register char *	base;
	register char *	x;
	register int	i,j,l,r;
	int	s;
#if	i8051 && (SMALL_MODEL || MEDIUM_MODEL)
	struct
	{
		int	l,r;
	}	stack[5];
	char	xbuf[16];
#else
#if	m6800
	struct
	{
		int	l,r;
	}	stack[5];
	char	xbuf[80];
#else
	struct
	{
		int	l,r;
	}	stack[16];
	char	xbuf[800];
#endif
#endif

	base = vbase;
	if(width < sizeof xbuf)
		x = xbuf;
	else if(!(x = malloc(width+1)))
		return;			/* can't do much */
	x[width] = 0;
	s = 0;
	stack[0].l = 0;
	stack[0].r = nel-1;
	do {
		/* take top request from stack */
		l = stack[s].l;
		r = stack[s--].r;

		do {
			i = l;
			j = r;
			memcpy(x, base+width*((i+j)/2), width);
			do {
				while((*compar)(base+i*width, x) < 0)
					i++;
				while((*compar)(x, base+j*width) < 0)
					j--;
				if(i <= j) {
					_swap(width, base+i*width, base+j*width);
					i++;
					j--;
				}
			} while(i <= j);
			if(j-l < r-i) {
				if(i < r) {	/* stack right partition */
					stack[++s].l = i;
					stack[s].r = r;
				}
				r = j;		/* continue with left */
			} else {
				if(l < j) {
					stack[++s].l = l;
					stack[s].r = j;
				}
				l = i;
			}
		} while(l < r);
	} while(s >= 0);
	if(x != xbuf)
		free(x);
}
