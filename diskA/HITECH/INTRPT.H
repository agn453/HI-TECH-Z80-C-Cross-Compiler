/* Definitions for interrupt handling */


/*	Macros to enable and disable interrupts */

#if	m6800
#define	ei()	asm("	cli")
#define	di()	asm("	sei")
#endif	/* m6800 */

#if	m6809
/* ei() and di() are for the IRQ, efi() and edi() are for FIRQ */

#define	ei()	asm("	andcc	#$EF")
#define	di()	asm("	orcc	#$10")
#define	efi()	asm("	andcc	#$BF")
#define	dfi()	asm("	orcc	#$40")
#endif	/* m6809 */

#if	i8096
#define	ei()	asm("	ei")
#define	di()	asm("	di")
#define	set_vector(vec, fun)	asm("	psect	vectors,ovrld"); asm("	global	_""fun"); asm("	org	vec"); asm("	dcw	_""fun"); asm("	psect	text")

#endif	/* i8096 */

#if	i8086
#define	ei()	asm("	sti")
#define	di()	asm("	cli")
#endif	/* i8086 */

#if	z80
#define	ei()	asm("	ei")
#define	di()	asm("	di")
#endif	z80


/*	The type of a pointer to an interrupt vector */

#if	i8086
typedef far interrupt void (*isr)(void);
extern isr set_vector(volatile far isr *, isr);
#else	/* i8086 */

typedef interrupt void (*isr)(void);

#ifndef	set_vector
extern isr set_vector(volatile isr *, isr);
#endif

#endif	/* i8086 */
