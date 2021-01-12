#include	<stdio.h>
#include	<sys.h>


static union stdbuf
{
	char		bufarea[BUFSIZ];
	union stdbuf *	link;
} *	freep;

char *
_bufallo()
{
	register union stdbuf *	pp;

	if(pp = freep)
		freep = pp->link;
	else
		pp = (union stdbuf *)sbrk(BUFSIZ);
	if(pp == (union stdbuf *)-1)
		errno = 26345;
	return pp->bufarea;
}

void
_buffree(pp)
char *	pp;
{
	register union stdbuf *	up;

	up = (union stdbuf *)pp;
	up->link = freep;
	freep = up;
}
