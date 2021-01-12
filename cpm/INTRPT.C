/*
 *	Interrupt handling for the Z80
 */

#include	<intrpt.h>

/*
 *	This routine is called to initialize a vector. It attempts
 *	to initialize the real vector, if this fails it attempts to
 *	determine if it points to a jmp instruction, and if so
 *	tries to modify the jmp instruction. If this fails it
 *	returns 0. The normal return is the previous
 *	interrupt vector address.
 *
 *	The arguments are the vector address (e.g. 0x66) and the
 *	address of an interrupt service routine. For example:
 *
 *	#include	<intrpt.h>
 *	extern isr nmi_serv(void);
 *
 *		set_vector((isr *)0x66, nmi_serv);
 *
 *	It is the callers responsibility to disable interrupts if
 *	necessary before calling this routine.
 */

isr
set_vector(volatile isr * vp, isr func)
{
	register isr oldp;

	if ((unsigned) vp <= 0x66) {
		*(unsigned char *)vp = 0xC3;
		if (*(unsigned char *)vp != 0xC3)
			return 0;
		vp = (isr *)((char *)vp + 1);
	}
	oldp = *vp;
	*vp = func;
	if(*vp == func)
		return oldp;
	if(*(unsigned char *)oldp == 0xC3) {
		vp = (isr *)((char *)oldp + 1);
		oldp = *vp;
		*vp = func;
		if(*vp == func)
			return oldp;
	}
	return 0;
}
