/*
 * $Id: proto.h,v 1.1 89/08/23 13:58:12 colin Exp $
 *
 * Protocol definitions for Lucifer Z80 version
 *
 * $Log:	proto.h,v $
 * Revision 1.1  89/08/23  13:58:12  colin
 * Initial revision
 * 
 */

#define	HELLO	0xE0
#define	ACK	0xE1

#define	READ	0xE2

#define	WRITE	0xE3
#define	WACK	0xE4

#define	GO	0xE5
#define	GACK	0xE6
#define	GREGS	0xE7
#define	SNDREGS	0xE8
#define	STOP	0xE9
#define	STEP	0xEA
#define	IDENT	0xEB

#define	IDACK	0xEC

#define	OUTPUT	0xED
#define	OACK	0xEE

#define	INPUT	0xEF
#define	IACK	0xF0
#define	STOPPED	0xF1
#define STOPCALL 0xF2
