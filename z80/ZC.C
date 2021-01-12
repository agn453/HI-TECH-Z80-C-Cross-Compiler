/*
 *	Copyright (C) 1984-1988 HI-TECH SOFTWARE
 *
 *	This software remains the property of HI-TECH SOFTWARE and is
 *	supplied under licence only. The use of this software is
 *	permitted under the terms of that licence only. Copying of
 *	this software except for the purpose of making backup or
 *	working copies for the use of the licensee on a single
 *	processor is prohibited.
 */

#if	!DOS && !unix
#define	unix	1
#endif

#include	<stdio.h>
#include	<ctype.h>
#include	<string.h>
#if	HI_TECH_C
#include	<stat.h>
#include	<stdlib.h>
#include	<unixio.h>
#include	<sys.h>
#endif	/* HI_TECH_C */
#include	"hitech.h"
#include	"version.h"

/*
 *	C command - Z80 Cross compiler
 *	MSDOS/Unix version
 *
 *	C [-C] [-O] [-I] [-F] [-U] [-D] [-S] [-X] [-P] [-W] [-M] files {-Llib}
 */

#define	MAXLIST	60		/* max arg list */
#define	BIGLIST	120		/* room for much more */

#define	PROMPT	"C"
#define	HITECH	"HITECH"
#define	TEMP1	"TEMP"
#define	TEMP2	"TMP"
#define	DEFPATH	"A:\\HITECH\\"
#define	DEFTMP	""

#define	LIBSUFF	".lib"		/* library suffix */

#define	LFLAGS		"-z"
#define	STDLIB		"c"
#define	GETARGS		"-U__getargs"

static void	setout(),	/* set output file name */
		print(),	/* print message */
		rm(),		/* remove file */
		doit(),		/* do it */
		addlib(),	/* add library */
		addobj(),	/* add object file */
		compile(),	/* compile C file */
		assemble();	/* assembler .AS file */

static BOOL	doexec();	/* do execute */
static char *	warn_lev;	/* warning level */

static char	keep,		/* retain .obj files, don't link */
		keepas,		/* retain .as files, don't assemble */
		verbose,	/* verbose - echo all commands */
		optimize,	/* invoke optimizer */
		xref,		/* generate cross reference listing */
		profile,	/* execution profiling */
		cpm,		/* produce CP/M com file */
		avocet,		/* produce avocet assembler code */
		symdb,		/* symbolic debugging */
		nolocal;	/* strip local symbols */

static char *	iuds[MAXLIST],	/* -[IUD] args to preprocessor */
	    *	objs[MAXLIST],	/* .obj files and others for linker */
	    *	flgs[BIGLIST],	/* flags etc for linker */
	    *	libs[MAXLIST],	/* .lib files for linker */
	    *	c_as[MAXLIST];	/* .c files to compile or .as to assemble */

static uchar	iud_idx,	/* index into uids[] */
		obj_idx,	/*   "     "  objs[] */
		flg_idx,	/*   "     "  flgs[] */
		lib_idx,	/*   "     "  libs[] */
		c_as_idx;	/*   "     "  c_as[] */

static unsigned	code_adr,	/* start of rom */
		ram_adr,	/* start of ram */
		base_addr,	/* address of base page psect */
		ram_size;	/* size of ram */
static char *	paths[] =
{
#if	unix
	"bin/link",
	"bin/objtohex",
	"bin/cgen",
	"bin/optim",
	"bin/cpp",
	"bin/zas",
	"lib/zlib",
	"bin/p1",
	"lib/zcrtcpm.obj",
	"bin/cref",
	"lib/zcrt.obj",
#else	/* unix */
	"link.exe",
	"objtohex.exe",
	"cgen.exe",
	"optim.exe",
	"cpp.exe",
	"zas.exe",
	"zlib",
	"p1.exe",
	"zcrtcpm.obj",
	"cref.exe",
	"zcrt.obj",
#endif	/* unix */
};

#define	linker	paths[0]
#define	objto	paths[1]
#define	cgen	paths[2]
#define	optim	paths[3]
#define	cpp	paths[4]
#define	assem	paths[5]
#define	libpath	paths[6]
#define	pass1	paths[7]
#define	strtoff	paths[8]
#define	cref	paths[9]
#define	romstrtoff	paths[10]

static char *	temps[] =
{
#if	unix
	"ctmp1XXXXX",
	"ctmp2XXXXX",
	"ctmp3XXXXX",
	"ctmp4XXXXX",
	"l.objXXXXX",
	"crftmXXXXX",
#else	/* unix */
	"$ctmp1.$$$",
	"$ctmp2.$$$",
	"$ctmp3.$$$",
	"$ctmp4.$$$",
	"l.obj",
	"cref.tmp",
#endif	/* unix */
};

#define	tmpf1	temps[0]
#define	tmpf2	temps[1]
#define	tmpf3	temps[2]
#define	redname	temps[3]
#define	l_dot_obj	temps[4]
#define	crtmp	temps[5]

static char *	cppdef[] = { "-DHI_TECH_C", "-Dz80" };
static char *	cpppath = "-I";

static char	tmpbuf[128];	/* gen. purpose buffer */
static char	single[40];	/* single object file to be deleted */
static short	nfiles;		/* number of source or object files seen */
static char *	outfile;	/* output file name for objtohex */
static short	nerrs;		/* errors from passes */
static short	plen;		/* length of path */
static char *	xrname;

extern char **	_getargs();
extern char *	mktemp();
#if	!HI_TECH_C
extern char *	getenv();
extern char *	malloc();
#endif	HI_TECH_C
extern int	errno;

static char *	xalloc();
static void	error();
static void	get_addresses();

main(argc, argv)
char **	argv;
{
	register char *	cp, * xp;
	short		i, len;
#if	!unix
	struct stat	statbuf;
#endif

	if(strcmp(argv[1], "-q") == 0) {
		argv++;
		argc--;
	}else {
		fprintf(stderr, "HI-TECH C COMPILER (Z80) V%s\n", VERS);
		fprintf(stderr, "Copyright (C) 1984-1989 HI-TECH SOFTWARE\n");
	}
#if	EDUC
	fprintf(stderr, "Licensed for Educational purposes only\n");
#endif	EDUC
#if	!unix
	if(argc == 1)
		argv = _getargs((char *)0, PROMPT);
#endif	/* unix */
	plen = strchr(strtoff, 'c') - strtoff;
	cp = getenv(HITECH);
#if	unix
	if(!cp)
		cp = "/usr/hitech/";
	plen += strlen(cp);
	cpppath = strcat(strcat(strcpy(xalloc(plen+strlen(cpppath)+9), cpppath), cp), "include/");
#else	/* unix */
	if(!cp)
		if(stat("P1.EXE", &statbuf) >= 0)
			cp = "";
		else {
			strcat(strcpy(tmpbuf, strchr(DEFPATH, '\\')), "P1.EXE");
			if(stat(tmpbuf, &statbuf) >= 0)
				cp = strchr(DEFPATH, '\\');
			else
				cp = DEFPATH;
		}
	if(cp[0] && cp[strlen(cp)-1] != '\\')
		cp = strcat(strcpy(xalloc(strlen(cp)+2), cp), "\\");
	plen += strlen(cp);
	cpppath = strcat(strcpy(xalloc(plen+strlen(cpppath)+1), cpppath), cp);
#endif	/* !unix */
	for(i = 0 ; i < sizeof paths/sizeof paths[0] ; i++)
		paths[i] = strcat(strcpy(xalloc(plen+strlen(paths[i])+1), cp), paths[i]);
#if	unix
	if(!(cp = getenv(TEMP1)))
		cp = "/tmp/";
	for(i = 0 ; i != sizeof temps/sizeof temps[0] ; i++) {
		temps[i] = strcat(strcpy(xalloc(len+strlen(temps[i])+1), cp), temps[i]);
		mktemp(temps[i]);
	}
#else	/* unix */
	if(((cp = getenv(TEMP1)) || (cp = getenv(TEMP2))) && (len = strlen(cp))) {
		if(cp[len-1] != '\\' && cp[len-1] != ':') {
			cp = strcat(strcpy(xalloc(len+2), cp), "\\");
			len++;
		}
		for(i = 0 ; i < sizeof temps/sizeof temps[0] ; i++)
			temps[i] = strcat(strcpy(xalloc(len+strlen(temps[i])+1), cp), temps[i]);
	}
#endif	/* unix */
	objs[0] = romstrtoff;
	obj_idx = 1;
	flgs[0] = LFLAGS;
	flg_idx = 1;
	for(i = 0 ; i < sizeof cppdef/sizeof cppdef[0] ; i++)
		iuds[i] = cppdef[i];
	iud_idx = i;
	while(*++argv) {
		if((argv)[0][0] == '-') {
			if(islower(i = argv[0][1]))
				argv[0][1] = i = toupper(i);
			switch(i) {

			case 'A':
				if(sscanf(argv[0]+2, "%x,%x,%x", &code_adr, &ram_adr, &ram_size) < 3)
					error("Malformed -A option");
				break;

			case 'V':
				verbose = 1;
				break;

			case 'T':
				avocet = 1;
				break;

			case 'S':
				keepas = 1;

			case 'C':
				if(strcmp(argv[0], "-Cpm") == 0 || strcmp(argv[0], "-CPM") == 0) {
					cpm = 1;
					break;
				}
				if(argv[0][2] == 'r' || argv[0][2] == 'R') {
					xref = 1;
					if(argv[0][3]) {
						xrname = &argv[0][1];
						xrname[0] = '-';
						xrname[1] = 'o';
					} else
						xrname = (char *)0;
				} else
					keep = 1;
				break;

			case 'O':
				if(argv[0][2])
					setout(argv[0]+2);
				else
					optimize = 1;
				break;

			case 'I':
			case 'U':
			case 'D':
				iuds[iud_idx++] = argv[0];
				break;

			case 'L':
				addlib(&argv[0][2]);
				break;

			case 'F':
				argv[0][1] = 'D';
				flgs[flg_idx++] = argv[0];
				break;

			case 'G':
				symdb = 1;
				argv[0][1] = 'H';

			case 'H':
				flgs[flg_idx++] = argv[0];
				break;

			case 'P':
				profile = 1;
				break;

			case 'W':
				warn_lev = argv[0];
				break;

			case 'X':
				nolocal = 1;

			case 'M':
				flgs[flg_idx++] = argv[0];
				break;

			default:
				fprintf(stderr, "Unknown flag %s\n", argv[0]);
				exit(1);
			}
			continue;
		}
		nfiles++;
		cp = argv[0];
#if	DOS
		while(*cp) {
			if(isupper(*cp))
				*cp = tolower(*cp);
			cp++;
		}
#endif	/* DOS */
		cp = strrchr(argv[0], '.');
		if(cp && (strcmp(cp, ".c") == 0 || strcmp(cp, ".as") == 0)) {
			c_as[c_as_idx++] = argv[0];
			if((xp = strrchr(argv[0], '\\')) || (xp = strrchr(argv[0], '/')) ||
				(xp = strrchr(argv[0], ':')))
				xp++;
			else
				xp = argv[0];
			*cp = 0;
			strcat(strcpy(tmpbuf, xp), ".obj");
			addobj(tmpbuf);
			strcpy(single, tmpbuf);
			*cp = '.';
		} else
			addobj(argv[0]);
	}
	if(cpm) {
		objs[0] = strtoff;
		iuds[iud_idx++] = "-DCPM";
		iuds[iud_idx++] = "-D_HOSTED";
	}
	doit();
	exit(nerrs != 0);
}

static void
doit()
{
	register char *	cp;
	register uchar	i;
	char		psbuf[80];

	if(symdb && optimize)
		fprintf(stderr, "-g used with -O: partial optimization only performed\n");
	if(xref)
		close(creat(crtmp, 0600));
	close(2);
	dup(1);
	iuds[iud_idx++] = cpppath;
	for(i = 0 ; i < c_as_idx ; i++) {
		cp = strrchr(c_as[i], '.');
		if(strcmp(cp, ".c") == 0)
			compile(c_as[i]);
		else if(keepas)
			fprintf(stderr, "-S option used: %s ignored\n", c_as[i]);
		else
			assemble(c_as[i]);
	}
	rm(tmpf1);
	rm(tmpf2);
	rm(tmpf3);
	if(!(keep || nerrs)) {
		if(cpm)
			sprintf(psbuf, "-ptext=0,data,bss");
		else {
			if(code_adr == 0 && ram_adr == 0)
				get_addresses();
			if(ram_size == 0)
				ram_size = 2048;
			sprintf(psbuf, "-ptext=0%xh,data,bss=0%xh,stack=0%xh", code_adr, ram_adr, ram_adr+ram_size);
		}
		flgs[flg_idx++] = psbuf;
		flgs[flg_idx++] = strcat(strcpy(xalloc(strlen(l_dot_obj)+3), "-o"), l_dot_obj);
		for(i = 0 ; i < obj_idx ; i++)
			flgs[flg_idx++] = objs[i];
		addlib(STDLIB);
		for(i = 0 ; i < lib_idx ; i++)
			flgs[flg_idx++] = libs[i];
		flgs[flg_idx] = 0;
		if(doexec(linker, flgs)) {
			flg_idx = 0;
			if(cpm || ((cp = strrchr(outfile, '.')) && (strcmp(cp, ".bin") == 0 || strcmp(cp, ".BIN") == 0))) {
				flgs[flg_idx++] = "-B";
				flgs[flg_idx++] = "-F";
			}
			flgs[flg_idx++] = l_dot_obj;
			flgs[flg_idx++] = outfile;
			flgs[flg_idx] = 0;
			doexec(objto, flgs);
			if(c_as_idx == 1 && nfiles == 1)
				rm(single);
		}
		rm(l_dot_obj);
	}
	if(xref)
		if(xrname) {
			flgs[0] = xrname;
			strcat(strcpy(tmpbuf, "-h"), outfile);
			if(cp = strrchr(tmpbuf, '.'))
				strcpy(cp, ".crf");
			else
				strcat(tmpbuf, ".crf");
			flgs[1] = tmpbuf;
			flgs[2] = crtmp;
			flgs[3] = 0;
			doexec(cref, flgs);
			rm(crtmp);
		} else
			fprintf(stderr, "Cross reference info left in %s: run cref to produce listing\n", crtmp);
}

static void
rm(file)
char *	file;
{
	char	buf[40];

	if(verbose) {
		strcat(strcpy(buf, "del "), file);
		print(buf);
	}
	unlink(file);
}

static void
print(s)
char *	s;
{
	fprintf(stderr, "%s\n", s);
}

static void
setout(s)
register char *	s;
{
	register char *	cp, * xp;
	
	cp = xalloc(strlen(s) + strlen("-O.hex") + 1);
	strcat(strcpy(cp, "-O"), s);
	if(!(xp = strrchr(cp, '.')) || strcmp(xp, ".obj") == 0) {
		*xp = 0;
		strcat(cp, cpm ? ".com" : ".hex");
	}
	outfile = cp+2;
	if(xp)
		*xp = '.';
}

static void
addobj(s)
char *	s;
{
	char *	cp;

	if(!outfile)
		setout(s);
	cp = xalloc(strlen(s)+1);
	strcpy(cp, s);
	objs[obj_idx++] = cp;
}

static void
addlib(s)
char *	s;
{
	char *	cp;

	strcpy(tmpbuf, libpath);
	strcat(strcat(tmpbuf, s), LIBSUFF);
	cp = xalloc(strlen(tmpbuf)+1);
	strcpy(cp, tmpbuf);
	libs[lib_idx++] = cp;
}

static void
error(s, a)
char *	s;
{
	fprintf(stderr, s, a);
	fputc('\n', stderr);
	exit(1);
}

static void
get_addresses()
{
	char	buf[80];

	fprintf(stderr, "\n\aInvalid or no memory addresses specified:\n");
	fprintf(stderr, "Use a -Arom,ram,ramsize option, e.g.\n");
	fprintf(stderr, "	-A8000,6000,2000\n");
	fprintf(stderr, "specifies ROM at 8000(hex) and 8K bytes of RAM at 6000(hex)\n");
	fprintf(stderr, "Rom address: ");
	gets(buf);
	sscanf(buf, "%x", &code_adr);
	fprintf(stderr, "RAM address: ");
	gets(buf);
	sscanf(buf, "%x", &ram_adr);
	fprintf(stderr, "RAM size(default 800 hex): ");
	gets(buf);
	ram_size = 0x800;
	sscanf(buf, "%x", &ram_size);
	fprintf(stderr, "Next time use the option -A%x,%x,%x\n", code_adr, ram_adr, ram_size);
}


static char *
xalloc(s)
short	s;
{
	register char *	cp;

	if(!(cp = malloc(s)))
		error("Out of memory");
	return cp;
}

upcase(s)
register char *	s;
{
	while(*s) {
		if(*s >= 'a' && *s <= 'z')
			*s -= 'a'-'A';
		s++;
	}
}

#if	unix
unx_exec(name, vec)
char *	name;
char **	vec;
{
	short	pid;
	int	status;

	if(pid = fork()) {
		if(pid == -1)
			return -1;
		while(wait(&status) != pid)
			continue;
		return status;
	}
	execv(name, vec);
	perror(name);
	exit(1);
}
#endif	/* unix */

static BOOL
doexec(name, vec)
char *	name;
char **	vec;
{
	register char *	cp;
	short	len;
	char **	pvec, ** pvec1;
	char *	redir[2];
	FILE *	cfile;
	char	redflag;
	int	i;
	int	newin, newstd;
	char	xbuf[30];
	char *	xvec[40];
	char	abuf[128];
	char	nbuf[128];

	pvec = vec;
	if((cp = strrchr(name, '/')) || (cp = strrchr(name, '\\')) || (cp = strrchr(name, ':')))
		xvec[0] = cp+1;
	else
		xvec[0] = name;
	pvec1 = xvec+1;
	len = 0;
	redflag = 0;
	for(;;) {
		*pvec1++ = *pvec;
		if(*pvec)
			len += strlen(*pvec++)+1;
		else
			break;
	}
	vec = xvec;
	if(verbose) {
		for(pvec = vec ;;) {
			fprintf(stderr, "%s", *pvec);
			if(*++pvec)
				putc(' ', stderr);
			else
				break;
		}
		putc('\n', stderr);
	}
#if	unix
	if((i = unx_exec(name, vec))) {
		if(i == -1) {
			perror(name);
			error("Exec failed");
		}
		nerrs++;
		return 0;
	}
	return 1;
#else	/* unix */
	if(len > 124) {
		sprintf(xbuf, redname);
		if(!(cfile = fopen(xbuf, "w")))
			error("Can't create %s", xbuf);
		len = 0;
		while(*++vec) {
			len += strlen(*vec);
			fprintf(cfile, "%s ", *vec);
			if(len > 126) {
				len = 0;
				fprintf(cfile, "\\\n");
			}
		}
		putc('\n', cfile);
		fclose(cfile);
		redir[0] = name;
		redir[1] = (char *)0;
		vec = redir;
		newin = open(xbuf, 0);
		if(newin < 0)
			error("Can't re-open %s for input", xbuf);
		if(fileno(stdin) == 0 && (newstd = dup(fileno(stdin))) < 0)
				error("Can't dup(stdin)");
		stdin->_file = newstd;
		if(newin != 0) {
			close(0);
			if(dup(newin) != 0)
				error("dup(newin) failed");
			close(newin);
		}
		redflag = 1;
	}
	while((i = spawnv(name, vec))) {
		if(i == -1) {
			perror(name);
			if(errno == 2 || errno == 3) {
				fprintf(stderr, "Enter new file name or change disks and press RETURN: ");
				gets(abuf);
				if(abuf[0]) {
					strcpy(nbuf, abuf);
					name = nbuf;
				}
				continue;
			}
			error("Exec failed");
		}
		nerrs++;
		break;
	}
	if(redflag) {
		close(0);
		rm(redname);
		if(dup(fileno(stdin)) != 0)
			error("dup(fileno(stdin) failed");
	}
	return i == 0;
#endif	/* unix */
}

static void
assemble(s)
char *	s;
{
	char *	vec[10];
	char	buf[80];
	char *	cp;
	uchar	i;

	if(c_as_idx > 1)
		print(s);
	i = 0;
	if(optimize)
		vec[i++] = "-j";
	if(nolocal)
		vec[i++] = "-x";
	if((cp = strrchr(s, '\\')) || (cp = strrchr(s, '/')) || (cp = strrchr(s, ':')))
		cp++;
	else
		cp = s;
	strcat(strcpy(buf, "-O"), cp);
	if(strrchr(buf, '.'))
		*strrchr(buf, '.') = 0;
	strcat(buf, ".obj");
	vec[i++] = buf;
	vec[i++] = s;
	vec[i] = (char *)0;
	doexec(assem, vec);
}

static void
compile(s)
char *	s;
{
	char *	cp;
	char *	vec[MAXLIST];
	uchar	i, j;
	char	cbuf[50];

	if(c_as_idx > 1)
		print(s);
	for(j = 0; j < iud_idx ; j++)
		vec[j] = iuds[j];
	vec[j++] = s;
	vec[j++] = tmpf1;
	vec[j] = (char *)0;
	if(!doexec(cpp, vec))
		return;
	if((cp = strrchr(s, '\\')) || (cp = strrchr(s, '/')) || (cp = strrchr(s, ':')))
		s = cp+1;
	*strrchr(s, '.') = 0;
	i = 0;
	vec[i++] = "-QP,port";		/* for port variables */
	if(warn_lev)
		vec[i++] = warn_lev;
	if(keepas)
		vec[i++] = "-S";
	if(xref)
		vec[i++] = strcat(strcpy(cbuf, "-c"), crtmp);
	vec[i++] = tmpf1;
	vec[i++] = tmpf2;
	vec[i++] = tmpf3;
	vec[i++] = (char *)0;
	if(!doexec(pass1, vec))
		return;
	i = 0;
	if(warn_lev)
		vec[i++] = warn_lev;
	if(avocet)
		vec[i++] = "-M2";
	if(symdb)
		vec[i++] = "-D";
	vec[i++] = tmpf2;
	vec[i++] = keepas && (!optimize || symdb) ? strcat(strcpy(tmpbuf, s), ".as") : tmpf1;
	vec[i++] = (char *)0;
	if(!doexec(cgen, vec))
		return;
	if(keepas && (!optimize || symdb))
		return;
	cp = tmpf1;
	if(optimize && !symdb) {
		i = 0;
		vec[i++] = tmpf1;
		if(keepas)
			vec[i++] = strcat(strcpy(tmpbuf, s), ".as");
		else
			vec[i++] = tmpf2;
		vec[i] = (char *)0;
		if(!doexec(optim, vec))
			return;
		if(keepas)
			return;
		cp = tmpf2;
	}
	i = 0;
	if(nolocal)
		vec[i++] = "-x";
	if(optimize)
		vec[i++] = "-j";
	vec[i++] = "-n";
	vec[i++] = strcat(strcat(strcpy(tmpbuf, "-o"), s), ".obj");
	vec[i++] = cp;
	vec[i] = (char *)0;
	doexec(assem, vec);
}
