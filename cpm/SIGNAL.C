#include	"cpm.h"
#include	<signal.h>
#include	<stdlib.h>

static void (*	where)(int);

void (*
signal(sig, action))(int)
int	sig;
void (*	action)(int);
{
	void (*	prev)(int);

	if(sig != SIGINT)
		return (void (*)(int))-1;
	prev = where;
	where = action;
	return prev;
}

void
_sigchk()
{
	char	c;

	if(where == SIG_IGN || bdos(CPMICON) == 0)
		return;
	c = bdos(CPMRCON);
	if(c != CPMRBT)
		return;
	if(where == SIG_DFL)
		exit(-SIGINT);
	(*where)(SIGINT);
}
