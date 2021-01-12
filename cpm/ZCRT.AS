;
; $Id: zcrt.as,v 1.2 89/10/02 16:02:46 colin Exp $
;
; Runtime startoff module Z80 compiler.  ROM version
;
; $Log:	zcrt.as,v $
; Revision 1.2  89/10/02  16:02:46  colin
; Clobbered redundant LD HL,0 before calling main()
; 
; Revision 1.1  89/09/29  13:09:40  colin
; Initial revision
; 
;
	psect	text,global,pure
	psect	data,global
	psect	bss,global
	psect	stack,global

	psect	text

	global	start,_main,_exit,__Hstack,__Hbss, __Lbss

	jp	start		; RST 0 & reset
	defb	0,0
	jp	bdosvec		; jump to BDOS handler
	jp	r08vec		; jump to RST 08 handler
	defb	0,0,0,0,0
	jp	r10vec		; jump to RST 10 handler
	defb	0,0,0,0,0
	jp	r18vec		; jump to RST 18 handler
	defb	0,0,0,0,0
	jp	r20vec		; jump to RST 20 handler
	defb	0,0,0,0,0
	jp	r28vec		; jump to RST 28 handler
	defb	0
	jp	r2Cvec		; jump to RST 0C handler (NSC800)
	defb	0
	jp	r30vec		; jump to RST 30 handler
	defb	0
	jp	r34vec		; jump to RST 0B handler (NSC800)
	defb	0
	jp	r38vec		; jump to RST 38 handler
	defb	0
	jp	r3Cvec		; jump to RST 0A handler (NSC800)
	defb	0,0
	defm	"COPYRIGHT (C) 1989 HI-TECH SOFTWARE"
	defb	0,0
	jp	nmivec		; jump to NMI handler
start:
	ld	hl,__Hstack	;top of stack area
	ld	sp,hl		;stack grows downwards
	ld	de,__Lbss	;Start of BSS segment
	or	a		;clear carry
	ld	hl,__Hbss
	sbc	hl,de		;size of uninitialized data area
	ld	c,l
	ld	b,h
	dec	bc	
	ld	l,e
	ld	h,d
	inc	de
	ld	(hl),0
 	ldir			;clear memory
	ld	a,0c3h		;JP opcode
	ld	hl,__vectors	;start of vector table
	ld	b,12		;number of JP opcodes to install
1:	ld	(hl),a
	inc	hl
	inc	hl
	inc	hl
	djnz	1b
	ld	hl,def_rst	;load address of default RST handler
	ld	(bdosvec+1),hl	;set BDOS handler to default
	ld	(r08vec+1),hl
	ld	(r10vec+1),hl
	ld	(r18vec+1),hl
	ld	(r20vec+1),hl	;set all RSTs to point at default handler
	ld	(r28vec+1),hl
	ld	(r30vec+1),hl
	ld	(r38vec+1),hl
	ld	hl,def_int	;load address of default INT handler
	ld	(r2Cvec+1),hl
	ld	(r34vec+1),hl	;set NSC800 interrupt vectors
	ld	(r3Cvec+1),hl
	ld	hl,def_nmi	;load address of default NMI handler
	ld	(nmivec+1),hl
	call	_main
_exit:
	jp	start
def_rst:			;default RST handler
	ret		
def_int:			;default interrupt handler
	ei
	reti
def_nmi:			;default NMI handler
	ei
	retn
;
	psect	data
nularg:
	defb	0
;
	psect	bss
;
;	Vector table used by "set_vector()"
;
__vectors:
bdosvec:	defs	3
r08vec:		defs	3
r10vec:		defs	3
r18vec:		defs	3
r20vec:		defs	3
r28vec:		defs	3
r2Cvec:		defs	3
r30vec:		defs	3
r34vec:		defs	3
r38vec:		defs	3
r3Cvec:		defs	3
nmivec:		defs	3
;
	end	start

