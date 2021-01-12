#if	z80
typedef	int	jmp_buf[4];
#endif

#if	i8086
typedef	int	jmp_buf[8];
#endif

#if	i8096
typedef	int	jmp_buf[10];
#endif

#if	m68k
typedef	int	jmp_buf[10];
#endif

#if	m6809
typedef	int	jmp_buf[4];
#endif

#if	m6800
typedef	int	jmp_buf[2];
#endif

#if	i8051
typedef int	jmp_buf[4];
#endif

extern	int	setjmp(jmp_buf);
extern void	longjmp(jmp_buf, int);
