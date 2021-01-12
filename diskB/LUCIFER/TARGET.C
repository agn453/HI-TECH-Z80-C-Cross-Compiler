/*
 * $Header: /u1/colin/lucifer/8bit/z80/RCS/target.c,v 1.8 89/10/10 16:55:31 colin Exp $
 *
 * Lucifer monitor for Z80.   v1.00, Hi-Tech Software, August 1989.
 *
 * C.G.W. 1/8/89
©  2018 Microchip Technology Inc. and its subsidiaries.
 
Subject to your compliance with these terms, you may use this Microchip software and any derivatives of this software. You must retain the above copyright notice with any redistribution of this software and the following disclaimers. It is your responsibility to comply with third party license terms applicable to your use of third party software (including open source software) that may accompany this Microchip software.
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  MICROCHIP WILL NOT SUPPORT THIS SOFTWARE IN ANY MANNER. NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE. 
 
IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THIS SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

 *
 * $Log:	target.c,v $
 * Revision 1.8  89/10/10  16:55:31  colin
 * Assigned version 3.00 after changing protocol to improve download
 * 
 * Revision 1.7  89/10/04  15:04:02  colin
 * Improved BDOS trap installer to follow JP at location 5, thus preserving
 * the CP/M top of RAM value
 * 
 * Revision 1.6  89/10/03  17:09:00  colin
 * Shifted break checking to bdosgetch()
 * 
 * Revision 1.5  89/10/03  16:48:16  colin
 * Fixed bug in direct console I/O call
 * 
 * Revision 1.4  89/10/03  15:23:02  colin
 * Modified to use set_vector() to install interrupt handlers
 * 
 * Revision 1.3  89/09/29  14:06:43  colin
 * Installed version 2.00 IDENT modifications.  Now uses full v2.0 protocol
 * 
 * Revision 1.2  89/08/28  16:08:41  colin
 * Removed linefeed delay in bdosputch()
 * 
 * Revision 1.1  89/08/28  13:01:24  colin
 * Initial revision
 * 
 */

#include	<setjmp.h>
#include	<intrpt.h>
#include	"proto.h"
#include	"machine.h"

jmp_buf		loop, sys;

static char	ustack[100];

/*
** The following three definitions are used to uniquely identify the version
** of the target code.  VERNO is the major release number of the target code,
** MACHID is the machine ID byte for the processor (from "machine.h")
**
** For a more complete discussion of the identification protocol, see
** Appendix 1 of the Lucifer manual.
*/

static unsigned char	MACHID = ID_Z80;			/* Z80 */
static unsigned char	VERNO = 3;				/* v3.xx */
static unsigned char	ident[] = "Z80 Lucifer JED v3.00";

/*
** Prototypes for all functions used in target.c
*/

static	void		step(void);
static	void		go(void);
static	void		dummy(void);
static	void interrupt	intr(void);
static	void		putch(unsigned char);
static	unsigned char	char_ready(void);
static	unsigned char	getch(void);

/*
** macros used for stuffing around with memory
*/

#define CALL_SUB		(unsigned char)0xCD
#define POKE(addr, byte)	*((unsigned char *)addr) = (unsigned char)byte
#define WPOKE(addr, word)	*((unsigned *)addr) = (unsigned)word
#define PEEK(addr)		*((unsigned char *)addr)
#define WPEEK(addr)		*((unsigned *)addr)

/*
** NOTE:
**	the ordering of the registers in struct regs is the same as the
**	stacking order of the registers when lucifer gets control via
**	the intr function.
*/

struct regs
{
	unsigned short	IR;
	unsigned short	AF2;
	unsigned short	BC2;
	unsigned short	DE2;
	unsigned short	HL2;
	unsigned short	AF;
	unsigned short	BC;
	unsigned short	DE;
	unsigned short	HL;
	unsigned short	IX;
	unsigned short	IY;
	unsigned short	PC;
} *rptr;
unsigned	soffs;

/*
** Vectors used by target
*/

#define	RST0	(isr *) 0x00		/* RST 0 vector */
#define RSTB	(isr *) 0x34		/* NSC800 RST B vector */
#define NMI	(isr *) 0x66		/* NMI vector */

/*
** console i/o for JED STD-801 - initialize uart, 
** send and receive characters
*/

port char	STATUS_A @ 0x91;	/* 88C681 channel status reg */
port char	CHANNEL_A @ 0x93;	/* 88C681 channel data reg */
port char	IN9412 @ 0xDA;		/* IN9412 status input (801) */
port char	IMR_A @ 0x95;		/* 88C681 interrupt mask register */
port char	INTEN @ 0xBB;		/* JED STD-801 interrupt masks */

/*
** putch:
**		transmit a character via the internal serial port
*/

static void
putch(c)
unsigned char	c;
{
	di();
	for (;;) {
		if (IN9412 & 0x10)
			continue;
		if (STATUS_A & 4)
			break;
	}
	CHANNEL_A = c;
	ei();
}

/*
** char_ready:
**		return 1 if a character is ready, 0 otherwise
*/

static unsigned char
char_ready()
{
	return STATUS_A & 1;
}

/*
** getch:
**		get an incoming character from the internal serial port
*/

static unsigned char
getch()
{
	unsigned char	c;

	di();
	while (!char_ready()) ;
	c = CHANNEL_A;
	ei();
	return c;
}

#ifdef	CPMCALLS
/*
 * The code in this set of routines is used to emulate all non disk
 * CP/M-80 system calls.  These calls are executed as a series of
 * putch() and getch() calls using the appropriate lucifer host
 * communication protocol
 */

static void	setup_bdos_trap(void);
static void	bdosputch(char);
static void	bdoserr(void);
static char	bdosgetch(void);
static void	bdosreadbuf(unsigned char *);
extern void	bdostrap(void);

void 		*oldbdos;		/* old bdos entry vector */

/*
** setup_bdos_trap will attempt to install the Lucifer BDOS handler
** onto any previous BDOS jump instruction present using the following
** algorithm:
**
** If there is not a BDOS jump present at location 5 then
**	install a jump to our handler at location 5
** otherwise
**	if the jump at location 5 does not point to another jump then
**		install our bdos handler address at 6, set oldbdos
**		to point to the handler which was present before
**	otherwise
**		install our bdos handler address into the JP instruction
**		pointed to by the JP at location 5, thereby preserving
**		the top of RAM value in the JP at location 5
**
** setup_bdos_trap attempts to preserve the original BDOS jump at 5 by
** following it to another JP instruction.  This is done because if the
** JP at location 5 was pointed directly into the handler in the target
** code, some CP/M programs would trash target by attempting to use
** the RAM just below our BDOS handler
*/

static void
setup_bdos_trap()
{
	unsigned	baddr;

	if (PEEK(5) != 0xC3) {
		POKE(5, 0xC3);
		WPOKE(6, bdostrap);
		oldbdos = bdoserr;
	} else {
		baddr = WPEEK(6);
		if (PEEK(baddr) != 0xC3) {
			oldbdos = (void *) baddr;
			WPOKE(6, bdostrap);
		} else {
			++baddr;
			oldbdos = (void *) WPEEK(baddr);
			WPOKE(baddr, bdostrap);
		}
	}
}

static void
bdosputch(c)
char	c;
{
	putch(OUTPUT);
	putch(c);
}

static void
bdoserr()
{
	static char	errmsg[] = "Unsupported BDOS call: aborted";
	register char	*ep;

	for (ep = errmsg; *ep; bdosputch(*ep++)) ;
	asm("rst 0");
}

static char
bdosgetch()
{
	unsigned char	c;

	for (;;) {
		putch(INPUT);
		if ((c = getch()) == STOP) {
			asm("rst 0");
			continue;
		}
		return c;
	}
}

static void
bdosreadbuf(bufptr)
register unsigned char	*bufptr;
{
	unsigned short	offset, buflen;
	unsigned char	c;

	offset = 2;
	buflen = *bufptr + 2;
	for (;;) {
		c = bdosgetch();
		if (c == 10 || c == 13)
			break;
		*(bufptr + offset++) = c;
		if (offset == buflen)
			break;
	}
	*(bufptr + 1) = offset - 2;
}

#asm
	PSECT	text
;
	GLOBAL	_oldbdos
	GLOBAL	_bdosputch, _bdosgetch, _bdostrap, _char_ready, _bdosreadbuf
;
; _bdostrap:
;	this routine gets control when a bdos call (location 5) is made
;	it filters those calls which Lucifer can handle and passes the
;	remainder through to the old bdos handler unchanged
;
_bdostrap:
	XOR	A
	OUT	(0BBH),A	;disable ALL JED interrupts
	LD	A,C		;function # --> A
	CP	1		;is it console input ?
	JR	Z,myconin
	CP	2		;is it console output ?
	JR	Z,myconout
	CP	6		;is it direct console i/o ?
	JR	Z,mydirio
	CP	10		;is it read buffer ?
	JR	Z,myreadbuf
	CP	11		;is it console status ?
	JR	Z,myconstat
	PUSH	HL		;user's HL --> stack
	LD	HL,(_oldbdos)	;get old bdos vector
	EX	(SP),HL		;exchange with user HL
	LD	A,4
	OUT	(0BBH),A	;enable RST B interrupt
	RET			;and jump to it
;
; myconin: (bdos 1)
;	return a console character in A
;
myconin:
	CALL	_bdosgetch	;get character (in HL)
	LD	A,4
	OUT	(0BBH),A	;enable RST B interrupt
	LD	A,L
	RET
;
; myconout: (bdos 2)
;	print character in E to console
;
myconout:
	PUSH	DE
	CALL	_bdosputch	;print character in E
	POP	DE
	LD	A,4
	OUT	(0BBH),A	;enable RST B interrupt
	RET
;
; mydirio: (bdos 6)
;	direct console i/o
;
mydirio:
	LD	A,E		;subfunction --> A
	CP	0FFH		;is it input ?
	JR	Z,myconin	;yes, do normal input
	CP	0FEH		;is it status ?
	JR	NZ,myconout	;no, output character in E
	JR	myconstat	;otherwise get status
;
; myreadbuf: (bdos 10)
;	read a line of characters into the buffer pointed to by DE
;
myreadbuf:
	PUSH	DE		;pass buffer address
	CALL	_bdosreadbuf	;call C read buffer routine
	POP	DE		;clear goo from stack
	LD	A,4
	OUT	(0BBH),A	;enable RST B interrupt
	RET
;
; myconstat: (bdos 11)
;	return console status (0 if no char ready, FF if char ready)
;
myconstat:
	CALL	_char_ready	;get serial port status
	LD	A,4
	OUT	(0BBH),A	;enable RST B interrupt
	XOR	A		;pre-load status 0
	BIT	0,L		;is a character ready ?
	RET	Z
	DEC	A		;status = FF
	RET
#endasm

#endif

/*
** step:
**		single step one instruction.  step works by scheduling an
**		internal timer interrupt to occur 1 clock cycle after it
**		exits via the RETN instruction.  The interrupt occurs one
**		cycle into the next instruction and is serviced after
**		that instruction has finished executing.
**
**		The registers for the user's program are on the	stack in
**		interrupt order, and thus are restored by the RETN 
*/

static void
step()
{
#asm
;
SSE	EQU	0D3H		;single step enable port
;
	DI
	LD	SP,(_rptr)	;restore user SP
	POP	AF
	LD	I,A
	EX	AF,AF'
	EXX
	POP	AF
	POP	BC
	POP	DE
	POP	HL		;restore entire register set 
	EXX			;from user stack
	EX	AF,AF'
	LD	A,0F1H
	LD	R,A		;set R count value for NMI
	XOR	A
	OUT	(SSE),A		;remove NMI jam
	LD	A,5
	OUT	(SSE),A		;take SSE high so NMI occurs
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	POP	IX
	POP	IY
	EI
	RETI
#endasm
}

/*
** go:
**		Commence execution of the user program from the current
**		user PC value.  Receive interrupts are enabled on the uart
**		so as to allow the user to interrupt the program by typing
**		^C on the host machine.
**
**		The registers stored are on the user program's stack and are
**		restored before execution is commenced via a RETI instruction
*/

static void
go()
{
	INTEN = 4;		/* enable RST B interrupt */
	IMR_A = 2;		/* enable receive interrupt */
#asm
	DI
	LD	SP,(_rptr)	;restore user SP
	POP	AF
	LD	I,A
	EX	AF,AF'
	EXX
	POP	AF
	POP	BC
	POP	DE
	POP	HL		;restore entire register set 
	EXX			;from user stack
	EX	AF,AF'
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	POP	IX
	POP	IY
	EI
	RETI
#endasm
}

/*
** dummy:
**	On initialisation, the user PC is set to point at this routine,
**	thus making sure that a g (GO) command before the user loads a
**	program does not crash the system.
*/

static void
dummy()
{
	asm("rst 0");
}

/*
** intr:
**		intr is the routine which gets control when the user
**		program is interrupted by the timer or a ^C from the host.
**		intr sets rptr to point at the registers on the stack and
**		returns control to the monitor main loop via longjmp
*/

static void interrupt
intr(void)
{
#asm
	EX	(SP),IY		;replace AF saved by entry code with IY
	PUSH	IX
	PUSH	HL
	PUSH	DE		;stuff entire user register set
	PUSH	BC		;on the stack
	PUSH	AF
	EXX
	EX	AF,AF'
	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF
	EX	AF,AF'
	EXX
	LD	A,I
	PUSH	AF
	GLOBAL	_rptr
	LD	(_rptr),SP
	GLOBAL	_sys
	GLOBAL	_longjmp
	LD	HL,1
	PUSH	HL		;return control to main
	LD	HL,_sys		;loop by doing
	PUSH	HL
	CALL	_longjmp	;longjmp(_sys,1)
#endasm
}

main()
{
	register unsigned short	*iptr;
	unsigned char		c, ck, *cp;
	unsigned		addr, stackaddr, cnt;

#ifdef	CPMCALLS
	setup_bdos_trap();
#endif
	di();
	set_vector(RST0, intr);
	set_vector(RSTB, intr);
	set_vector(NMI, intr);
	ei();
	if(!setjmp(sys)) {
		rptr = (struct regs *)(ustack+sizeof ustack - sizeof(struct regs));
		for (iptr = (unsigned short *) rptr; iptr != &rptr->PC; *iptr++ = 0);
		rptr->PC = (unsigned) dummy;
	} else {			/* intr passes control to here */
		IMR_A = 0;		/* disable receive interrupt */
		INTEN = 0;		/* disable all interrupts */
		if (*(cp = (unsigned char *) rptr->PC) == CALL_SUB) {
			putch(STOPCALL);
			putch(*++cp);		/* address low */
			putch(*++cp);		/* address high */
		} else
			putch(STOPPED);
		putch(rptr->PC >> 8);		/* PCH */
		putch(rptr->PC);		/* PCL */
	}
	for(;;) {
		setjmp(loop);
		c = (unsigned char)getch();
		if(c == HELLO) {
			putch(ACK);
			continue;
		}
		if(c == STOP)
			continue;
		if(c == IDENT) {
			putch(IDACK);
			putch(MACHID);
			putch(VERNO);
			putch(sizeof ident - 1);
			cnt = 0;
			while(cnt != sizeof ident - 1)
				putch(ident[cnt++]);
			continue;
		}
		if(c == GREGS) {
			putch((cnt = sizeof(struct regs))+2);
			addr = (unsigned) rptr + sizeof(struct regs);
			putch(ck = addr >> 8);			/* SP high */
			putch(addr);				/* SP low */
			ck += addr;
			for (iptr = (unsigned short *) rptr; cnt; cnt -= 2, iptr++) {
				putch(c = *iptr >> 8);		/* high byte */
				ck += c;
				putch(c = *iptr);		/* low byte */
				ck += c;
			}
			putch(ck);
			continue;
		}
		if(c == SNDREGS) {
			getch();   	/* sizeof register structure */
			stackaddr = (ck = getch()) << 8;	/* SP high */
			ck += c = getch();			/* SP low */
			stackaddr += c;
			rptr = (struct regs *) (stackaddr - sizeof(struct regs));
			cnt = sizeof(struct regs);
			for (iptr = (unsigned short *) rptr; cnt; cnt -= 2, iptr++) {
				ck += c = getch();		/* high byte */
				*iptr = c << 8;
				ck += c = getch();		/* low byte */
				*iptr += c;
			}
			putch(WACK);
			putch(ck);
			continue;
		}
		if(c == READ) {
			c = (unsigned char)getch();
			addr = ((unsigned char)getch() << 8) + c;
			cnt = (unsigned char)getch();
			if(!cnt)
				cnt = 256;
			ck = 0;
			while(cnt--) {
				c = *(unsigned char *)addr++;
				ck += c;
				putch(c);
			}
			putch(ck);
			continue;
		}
		if(c == STEP || c == GO) {
			putch(GACK);
			if(c == STEP)
				step();
			go();
		}
		if(c == WRITE) {
			c = (unsigned char)getch();
			addr = ((unsigned char)getch() << 8) + c;
			cnt = (unsigned char)getch();
			cnt += (unsigned char)getch() << 8;
			ck = 0;
			while(cnt--) {
				c = (unsigned char)getch();
				*(unsigned char *)addr = c;
				ck += *(unsigned char *)addr++;
			}
			putch(WACK);
			putch(ck);
			continue;
		}
	}
}
