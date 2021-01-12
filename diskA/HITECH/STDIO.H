#if	z80
#define	BUFSIZ		512
#define	_NFILE		8
#else	z80
#define	BUFSIZ		1024
#define	_NFILE		20
#endif	z80

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
#endif	_STDDEF

#ifndef	_STDARG
#include	<stdarg.h>
#endif

#ifndef	NULL
#define	NULL	((void *)0)
#endif	NULL

extern int	errno;			/* system error number */

#ifndef FILE
#define	uchar	unsigned char

extern	struct	_iobuf {
	char *		_ptr;
	int		_cnt;
	char *		_base;
	unsigned short	_flag;
	short		_file;
	size_t		_size;
} _iob[_NFILE];

#define	FILE		struct _iobuf

#define	L_tmpnam	34		/* max length of temporary names */
#define	_MAXTFILE	8		/* max number of temporary files */

extern struct _tfiles {
	char	tname[L_tmpnam];
	FILE *	tfp;
}	_tfiles[_MAXTFILE];

#endif	FILE

#define	_IOFBF		0
#define	_IOREAD		01
#define	_IOWRT		02
#define	_IORW		03
#define	_IONBF		04
#define	_IOMYBUF	010
#define	_IOEOF		020
#define	_IOERR		040
#define	_IOSTRG		0100
#define	_IOBINARY	0200
#define	_IOLBF		0400
#define	_IODIRN		01000	/* true when file is in write mode */
#define	_IOAPPEND	02000	/* file was opened in append mode */
#define	_IOSEEKED	04000	/* a seek has occured since last write */
#define	_IOTMPFILE	010000	/* this file is a temporary */

#define	EOF		(-1)
#define	_IOSTRING	(-67)

#define	stdin		(&_iob[0])
#define	stdout		(&_iob[1])
#define	stderr		(&_iob[2])
#define	getchar()	getc(stdin)
#define	putchar(x)	putc(x,stdout)

/*	getc() and putc() must be functions for CP/M to allow the special
 *	handling of '\r', '\n' and '\032'. The same for MSDOS except that
 *	it at least knows the length of a file.
 */

#define	getc(p)		fgetc(p)
#define	putc(x,p)	fputc(x,p)

#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((uchar)p->_file)
#define	clrerr(p)	p->_flag &= ~_IOERR
#define	clreof(p)	p->_flag &= ~_IOEOF
#define	clearerr(p)	p->_flag &= ~(_IOERR|_IOEOF)


extern int	_flsbuf(char, FILE *);
extern int	_filbuf(FILE *);
extern int	fclose(FILE *);
extern int	fflush(FILE *);
extern int	fgetc(FILE *);
extern int	ungetc(int, FILE *);
extern int	fputc(int, FILE *);
extern int	getw(FILE *);
extern int	putw(int, FILE *);
extern char *	gets(char *);
extern int	puts(char *);
extern int	fputs(char *, FILE *);
extern int	fread(void *, size_t, size_t, FILE *);
extern int	fwrite(void *, size_t, size_t, FILE *);
extern int	fseek(FILE *, long, int);
extern int	rewind(FILE *);
extern void	setbuf(FILE *, char *);
extern int	setvbuf(FILE *, char *, int, size_t);
extern int	printf(char *, ...);
extern int	fprintf(FILE *, char *, ...);
extern int	sprintf(char *, char *, ...);
extern int	scanf(char *, ...);
extern int	fscanf(FILE *, char *, ...);
extern int	sscanf(char *, char *, ...);
extern int	vfprintf(FILE *, char *, va_list);
extern int	vprintf(char *, va_list);
extern int	vsprintf(char *, char *, va_list);
extern int	vscanf(char *, va_list ap);
extern int	vfscanf(FILE *, char *, va_list);
extern int	vsscanf(char *, char *, va_list);
extern int	remove(char *);
extern int	rename(char *, char *);
extern FILE *	fopen(char *, char *);
extern FILE *	freopen(char *, char *, FILE *);
extern FILE *	fdopen(int, char *);
extern long	ftell(FILE *);
extern char *	fgets(char *, int, FILE *);
extern void	perror(const char *);
extern char *	_bufallo(void);
extern void	_buffree(char *);
extern char *	tmpnam(char *);
extern FILE *	tmpfile(void);
#if	unix
extern FILE *	popen(char *, char *);
extern int	pclose(FILE *);
#endif
