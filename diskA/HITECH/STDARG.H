/*	Macros for accessing variable arguments */

#ifndef	_STDARG

typedef void *	va_list[1];

#if	i8051 && !EXTERNAL_STACK

#define	va_start(ap, parmn)	*ap = &parmn;
#define	va_arg(ap, type)	(*--(*(type **)ap))

#else

#define	va_start(ap, parmn)	*ap = (char *)&parmn + sizeof parmn
#define	va_arg(ap, type)	(*(*(type **)ap)++)

#endif

#define	va_end(ap)

#define	_STDARG

#endif	/* STDARG */
