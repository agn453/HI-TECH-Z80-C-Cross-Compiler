/*
 *	fclose - for HI-TECH C stdio
 */

#include	<stdio.h>
#include	<unixio.h>

fclose(f)
register FILE *	f;
{
	register short	i;
	register unsigned	of;

	if(!(f->_flag & (_IOREAD|_IOWRT)))
		return(EOF);
	fflush(f);
	of = f->_flag;
	f->_flag &= ~(_IOREAD|_IOWRT|_IONBF|_IOLBF);
	if(f->_base && !(f->_flag & _IOMYBUF)) {
		_buffree(f->_base);
		f->_base = (char *)0;
	}
	if(close(fileno(f)) == -1 || f->_flag & _IOERR)
		return EOF;
	if(of & _IOTMPFILE)
		for(i = 0 ; i != _MAXTFILE ; i++)
			if(_tfiles[i].tfp == f) {
				remove(_tfiles[i].tname);
				_tfiles[i].tfp = 0;
				break;
			}
	return 0;
}
