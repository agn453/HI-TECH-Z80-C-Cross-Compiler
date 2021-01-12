#include	<stdio.h>

static char	buf[50];

void hexbuf()
{
	char	*cp;

	for (cp = buf; *cp; printf("%02X ", *cp++)) ;
	printf("\n");
}

static int
strlen(cp)
register char	*cp;
{
	int	len;

	for (len = 0; *cp++; len++);
	return len;
}

main()
{
	register char	c, n;

	printf("Ascii table for all visible characters\n");
	for (c = 32; c != 127; c++) {
		printf("%02X = %c", c, c);
		if ((c - 1) % 10)
			printf("\t");
		else
			printf("\n");
	}
	printf("\nPress <return> : ");
	gets(buf);
	printf("buf = %s\nlen = %d\n*** = ", buf, strlen(buf));
	for (n = 0; buf[n++]; printf("^")) ;
	printf("\n");
	hexbuf();
	for (c = 1; c != 15; c++) {
		for (n = 20; n != c; n--)
			printf(" ");
		for (n = 1; n != c; n++)
			printf("**");
		printf("\n");
	}
	printf("End of program\n");
	gets(buf);
	printf("buf = %s\n", buf);
	hexbuf();
}
