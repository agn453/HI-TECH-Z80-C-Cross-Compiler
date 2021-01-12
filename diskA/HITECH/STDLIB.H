/*	Standard utility functions */

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */

#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))

#define	_STDDEF

#ifndef	NULL
#define	NULL	((void *)0)
#endif	NULL

extern int	errno;			/* system error number */
#endif	_STDDEF

#define	RAND_MAX	32767		/* max value returned by rand() */

extern double	atof(char *);
extern int	atoi(char *);
extern unsigned	xtoi(char *);
extern long	atol(char *);
extern int	rand(void);
extern void	srand(unsigned int);
extern void *	calloc(size_t, size_t);
extern void	free(void *);
extern void *	malloc(size_t);

#if	i8086
#if	SMALL_MODEL || MEDIUM_MODEL
extern far void *	farmalloc(size_t);
extern far void *	farmemcpy(far void *, far void *, size_t);
extern far void *	farmemset(far  void *, int, size_t);
extern void		farfree(far void *);
#else
extern void *	farmalloc(size_t);
extern void *	farmemcpy(void *, void *, size_t);
extern void *	farmemset(void *, int, size_t);
extern void	farfree(void *);
#endif
#endif

extern void *	realloc(void *, size_t);
extern void	abort(void);
extern void	exit(int);
extern char *	getenv(char *);
extern char **	environ;
extern int	system(char *);
extern void	qsort(void *, size_t, size_t, int (*)(void *, void *));
extern int	abs(int);
extern long	labs(long);
