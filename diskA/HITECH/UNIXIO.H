/*
 *	Declarations for Unix style low-level I/O functions.
 */

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
#endif	_STDDEF

#ifndef	NULL
#define	NULL	((void *)0)
#endif	NULL

extern int	errno;			/* system error number */

extern int	open(char *, int);
extern int	close(int);
extern int	creat(char *, int);
extern int	dup(int);
extern int	dup2(int, int);
extern long	lseek(int, long, int);
extern size_t	read(int, void *, size_t);
extern int	unlink(char *);
extern size_t	write(int, void *, size_t);
extern int	isatty(int);
extern int	chmod(char *, unsigned);
#if	unix
extern int	umask(int);
extern int	chown(char *, unsigned);
extern int	ioctl(int, int, void *);
extern int	pipe(int *);
#endif
extern int	access(char *, int);
