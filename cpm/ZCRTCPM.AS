	psect	text,global,pure
	psect	data,global
	psect	bss,global

	psect	text
	defs	100h		;Base of CP/M's TPA

	global	start,_main,_exit,__Hbss, __Lbss, __argc_, startup

start:	
	defb	0b8h
	jr	1f
	defb	0b8h,00,09h,0bah	;this code prevents attempted
	defw	message			;execution under MS-DOS
	defb	0cdh,21h,0b8h,01h,4ch,0cdh,21h,0,0,0
1:
	ld	hl,(6)		;base address of fdos
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
	ld	hl,nularg
	push	hl
	ld	hl,80h		;argument buffer
	ld	c,(hl)
	inc	hl
	ld	b,0
	add	hl,bc
	ld	(hl),0		;zero terminate it
	ld	hl,81h
	push	hl
	call	startup
	pop	bc		;unjunk stack
	pop	bc
	push	hl
	ld	hl,(__argc_)
	push	hl
	call	_main
	push	hl
	call	_exit
	jp	0

	psect	data
nularg:	defb	0
message:
	defm	'Z80 .COM file'
	defb	0dh,0ah,'$',0
	end	start
