;
; $Id: zcrti.as,v 1.1 89/10/02 16:03:08 colin Exp $
;
; Runtime startoff module Z80 compiler.  ROM version
; no interrupt vectors.
;
; $Log:	zcrti.as,v $
; Revision 1.1  89/10/02  16:03:08  colin
; Initial revision
; 
;
	psect	text,global,pure
	psect	data,global
	psect	bss,global
	psect	stack,global

	psect	text

	global	start,_main,_exit,__Hstack,__Hbss, __Lbss

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
	call	_main
_exit:
	jp	start
;
	psect	data
;
nularg:
	defb	0
;
	psect	bss
;
	defs	2		;force non zero BSS size
;
	end	start

