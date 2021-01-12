/*
 *	System-dependent functions.
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

extern int	execl(char *, char *, char *, ...);
extern int	execv(char *, char **);
extern int	execve(char *, char **, char **);
extern int	execle(char *, char *, char *, char *, ...);
extern int	execvp(char *, char **);
extern int	execlp(char *, char *, char *, ...);
#if	unix
extern int	fork(void);
extern int	wait(int *);
extern int	getuid(void);
extern int	setuid(int);
extern int	getpid(void);
extern int	kill(int, int);
extern char *	mktemp(char *);
extern int	alarm(int);
extern int	pause(void);
extern void	sleep(unsigned int);
#else	unix
extern int	spawnl(char *, char *, ...);
extern int	spawnv(char *, char **);
extern int	spawnle(char *, char *, ...);
extern int	spawnve(char *, char **, char **);
#endif	unix
extern int	chdir(char *);
extern int	mkdir(char *);
extern int	rmdir(char *);
extern char *	getcwd(int);
extern char **	_getargs(char *, char *);
extern int	_argc_;
extern int	inp(int);
extern void	outp(int, int);
extern void *	sbrk(size_t);
extern void	brk(char *);
extern int	_pnum(unsigned long, char, char, unsigned char, unsigned char, void (*)(int), unsigned char);
extern int	_fnum(double, short, short, short, void (*)(int), short);
extern char *	ffirst(char *), * fnext(void);
