/*******************************************************
 *
 *  a56 - a DSP56001 assembler
 *
 *  Written by Quinn C. Jensen
 *  July 1990
 *
 *******************************************************/

/*
 * Copyright (C) 1990-1994 Quinn C. Jensen
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no representations
 * about the suitability of this software for any purpose.  It is
 * provided "as is" without express or implied warranty.
 *
 */
static char *Copyright = "Copyright (C) 1990-1994 Quinn C. Jensen";

/*
 *  subs.c - Some subroutines for the assembler.
 *
 */

#include <stdlib.h>
#include <string.h>

#include "a56.h"

#define MAX 1024

char *alloc();

FILE *
open_read(char *file)
{
	FILE *fp;

	if(strcmp(file, "-") == 0)
		fp = stdin;
	else if ((fp = fopen(file, "r")) == NULL) {
		perror(file);
		exit(1);
	}	
	return fp;
}

FILE *
open_write(char *file)
{
	FILE *fp;
	if ((fp = fopen(file, "w")) == NULL) {
		perror(file);
		exit(1);
	}	
	return fp;
}

FILE *
open_append(char *file)
{
	FILE *fp;
	if ((fp = fopen(file, "a")) == NULL) {
		perror(file);
		exit(1);
	}	
	return fp;
}

void
fatal(char *c, char *a1, char *a2, char *a3, char *a4, char *a5, char *a6, char *a7, char *a8)
{
	fprintf(stderr, c, a1, a2, a3, a4, a5, a6, a7, a8);
	exit(1);
}

#define TABS 8
#define MAX_BUF 256

char tabbuf[MAX_BUF], *untabn();
char *untab(s)	/* expand tabs in s */
register char *s;
{
	return untabn(s, TABS);
}

char *untabn(s, stops)	/* expand tabs in s */
register char *s;
register int stops;
{
	char *o = s;

	/* copy input string into buffer to scan while input string is modified */

	register char *b = tabbuf;
	
	strncpy(b, s, MAX_BUF);

	/* iterate until the copy of the input string is depleted */

	while(*b) {
		if(*b == '\t') {
			do
				*s = ' ';
			while ((++s - o) % stops);
		} else {
			*s = *b; s++;
		}
		b++;
	}

	/* null terminate the resultant string */

	*s = '\0';

	return o;
}

char *
alloc(int size)
{
	char *p = (char *)malloc(size);
	if (!p)
		fprintf(stderr, "alloc:  insufficient virtual memory to allocate %d bytes\n", size);
	return p;
}

#define ascii2n(c)  \
	((c) >= 'a' ? (c) - 'a' + 10 : ((c) >= 'A' ? (c) - 'A' + 10 : (c) - '0'))

#define valid(c) ((c) >= '0' && (c) <= '9' || \
	(c) >= 'A' && (c) <= 'Z' || \
	(c) >= 'a' && (c) <= 'z')

