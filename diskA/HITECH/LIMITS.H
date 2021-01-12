/*	Characteristics of integral types */

#define	CHAR_BIT	8		/* bits per char */
#define	CHAR_MAX	127		/* max value of a char */
#define	CHAR_MIN	-128		/* min value */
#define	SCHAR_MAX	CHAR_MAX	/* chars are signed */
#define	SCHAR_MIN	CHAR_MIN
#define	UCHAR_MAX	255		/* for unsigned chars */
#define	SHRT_MAX	32767		/* max value of a short */
#define	SHRT_MIN	-32768
#define	USHRT_MAX	65535		/* unsigned short */
#ifdef	m68k
#define	INT_MAX		2147483647	/* max value of int */
#define	INT_MIN		-2147483648	/* min value */
#define	UINT_MAX	4294967295	/* unsigned int */
#else	m68k
#define	INT_MAX		32767		/* max for int */
#define	INT_MIN		-32768		/* min for int */
#define	UINT_MAX	65535		/* unsigned int */
#endif	m68k
#define	LONG_MAX	2147483647	/* max value of long */
#define	LONG_MIN	-2147483648	/* min value */
#define	ULONG_MAX	4294967295	/* unsigned long */
