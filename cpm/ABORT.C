#include	<cpm.h>
#include	<stdlib.h>

static char	mess[] = "Fatal error - program aborted\r\n$";

void
abort()
{
	bdos(9, mess);
	exit(-1);
}
