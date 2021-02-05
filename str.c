/*	$OpenBSD: str.c,v 1.22 2018/09/18 17:48:22 millert Exp $	*/
/*	$NetBSD: str.c,v 1.6 1995/03/21 09:03:24 cgd Exp $	*/

/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define MALLOC_INCR	128

/*
 * tc.str.c: Short string package
 *	     This has been a lesson of how to write buggy code!
 */

#include <sys/types.h>
#include <stdarg.h>
#include <vis.h>

#include "csh.h"
#include "extern.h"

#ifdef SHORT_STRINGS

Char  **
blk2short(char **src)
{
    size_t     n;
    Char **sdst, **dst;

    /*
     * Count
     */
    for (n = 0; src[n] != NULL; n++)
	continue;
    sdst = dst = xreallocarray(NULL, n + 1, sizeof(Char *));

    for (; *src != NULL; src++)
	*dst++ = SAVE(*src);
    *dst = NULL;
    return (sdst);
}

char  **
short2blk(Char *const *src)
{
    size_t     n;
    char **sdst, **dst;

    /*
     * Count
     */
    for (n = 0; src[n] != NULL; n++)
	continue;
    sdst = dst = xreallocarray(NULL, n + 1, sizeof(char *));

    for (; *src != NULL; src++)
	*dst++ = xstrdup(short2str(*src));
    *dst = NULL;
    return (sdst);
}

Char   *
str2short(const char *src)
{
    static Char *sdst;
    static size_t dstsize = 0;
    Char *dst, *edst;

    if (src == NULL)
	return (NULL);

    if (sdst == (NULL)) {
	dstsize = MALLOC_INCR;
	sdst = xreallocarray(NULL, dstsize, sizeof(Char));
    }

    dst = sdst;
    edst = &dst[dstsize];
    while (*src) {
	*dst++ = (Char) ((unsigned char) *src++);
	if (dst == edst) {
	    dstsize += MALLOC_INCR;
	    sdst = xreallocarray(sdst, dstsize, sizeof(Char));
	    edst = &sdst[dstsize];
	    dst = &edst[-MALLOC_INCR];
	}
    }
    *dst = 0;
    return (sdst);
}

char *
short2str(const Char *src)
{
    static char *sdst = NULL;
    static size_t dstsize = 0;
    char *dst, *edst;

    if (src == NULL)
	return (NULL);

    if (sdst == NULL) {
	dstsize = MALLOC_INCR;
	sdst = xreallocarray(NULL, dstsize, sizeof(char));
    }
    dst = sdst;
    edst = &dst[dstsize];
    while (*src) {
	*dst++ = (char) *src++;
	if (dst == edst) {
	    dstsize += MALLOC_INCR;
	    sdst = xreallocarray(sdst, dstsize, sizeof(char));
	    edst = &sdst[dstsize];
	    dst = &edst[-MALLOC_INCR];
	}
    }
    *dst = 0;
    return (sdst);
}

Char *
Strcpy(Char *dst, const Char *src)
{
    Char *d = dst;
    while ((*dst++ = *src++) != '\0')
        ;
    return (d);
}

Char *
Strncpy(Char *dst, const Char *src, size_t n)
{
    Char *sdst;

    if (n == 0)
	return(dst);

    sdst = dst;
    do
	if ((*dst++ = *src++) == '\0') {
	    while (--n != 0)
		*dst++ = '\0';
	    return(sdst);
	}
    while (--n != 0);
    return (sdst);
}

Char *
Strcat(Char *dst, const Char *src)
{
    short *sdst;

    sdst = dst;
    while (*dst++)
	continue;
    --dst;
    while ((*dst++ = *src++) != '\0')
	continue;
    return (sdst);
}

#ifdef NOTUSED
Char *
Strncat(Char *dst, Char *src, size_t n)
{
    Char *sdst;

    if (n == 0)
	return (dst);

    sdst = dst;

    while (*dst++)
	continue;
    --dst;

    do
	if ((*dst++ = *src++) == '\0')
	    return(sdst);
    while (--n != 0)
	continue;

    *dst = '\0';
    return (sdst);
}

#endif

Char   *
Strchr(Char *str, int ch)
{
    do
	if (*str == ch)
	    return (str);
    while (*str++)
	;
    return (NULL);
}

Char   *
Strrchr(Char *str, int ch)
{
    Char *rstr;

    rstr = NULL;
    do
	if (*str == ch)
	    rstr = str;
    while (*str++)
	;
    return (rstr);
}

size_t
Strlen(const Char *str)
{
    size_t n;

    for (n = 0; *str++; n++)
	continue;
    return (n);
}

int
Strcmp(const Char *str1, const Char *str2)
{
    for (; *str1 && *str1 == *str2; str1++, str2++)
	continue;
    /*
     * The following case analysis is necessary so that characters which look
     * negative collate low against normal characters but high against the
     * end-of-string NUL.
     */
    if (*str1 == '\0' && *str2 == '\0')
	return (0);
    else if (*str1 == '\0')
	return (-1);
    else if (*str2 == '\0')
	return (1);
    else
	return (*str1 - *str2);
}

int
Strncmp(const Char *str1, const Char *str2, size_t n)
{
    if (n == 0)
	return (0);
    do {
	if (*str1 != *str2) {
	    /*
	     * The following case analysis is necessary so that characters
	     * which look negative collate low against normal characters
	     * but high against the end-of-string NUL.
	     */
	    if (*str1 == '\0')
		return (-1);
	    else if (*str2 == '\0')
		return (1);
	    else
		return (*str1 - *str2);
	}
	if (*str1 == '\0')
	    return(0);
	str1++, str2++;
    } while (--n != 0);
    return(0);
}

Char   *
Strsave(Char *s)
{
    Char   *n;
    Char *p;

    if (s == 0)
	s = STRNULL;
    for (p = s; *p++;)
	continue;
    n = p = xreallocarray(NULL, p - s, sizeof(Char));
    while ((*p++ = *s++) != '\0')
	continue;
    return (n);
}

Char   *
Strspl(const Char *cp, const Char *dp)
{
    Char   *ep, *d;
    const Char *p, *q;

    if (!cp)
	cp = STRNULL;
    if (!dp)
	dp = STRNULL;
    for (p = cp; *p++;)
	continue;
    for (q = dp; *q++;)
	continue;
    ep = xreallocarray(NULL, (p - cp) + (q - dp) - 1, sizeof(*ep));
    for (d = ep, q = cp; (*d++ = *q++) != '\0';)
	continue;
    for (d--, q = dp; (*d++ = *q++) != '\0';)
	continue;
    return (ep);
}

Char   *
Strend(Char *cp)
{
    if (!cp)
	return (cp);
    while (*cp)
	cp++;
    return (cp);
}

Char   *
Strstr(Char *s, const Char *t)
{
    do {
	Char *ss = s;
	const Char *tt = t;

	do
	    if (*tt == '\0')
		return (s);
	while (*ss++ == *tt++);
    } while (*s++ != '\0');
    return (NULL);
}
#endif				/* SHORT_STRINGS */

char   *
short2qstr(const Char *src)
{
    static char *sdst = NULL;
    static size_t dstsize = 0;
    char *dst, *edst;

    if (src == NULL)
	return (NULL);

    if (sdst == NULL) {
	dstsize = MALLOC_INCR;
	sdst = xreallocarray(NULL, dstsize, sizeof(*sdst));
    }
    dst = sdst;
    edst = &dst[dstsize];
    while (*src) {
	if (*src & QUOTE) {
	    *dst++ = '\\';
	    if (dst == edst) {
		dstsize += MALLOC_INCR;
		sdst = xreallocarray(sdst, dstsize, sizeof(*sdst));
		edst = &sdst[dstsize];
		dst = &edst[-MALLOC_INCR];
	    }
	}
	*dst++ = (char) *src++;
	if (dst == edst) {
	    dstsize += MALLOC_INCR;
	    sdst = xreallocarray(sdst, dstsize, sizeof(*sdst));
	    edst = &sdst[dstsize];
	    dst = &edst[-MALLOC_INCR];
	}
    }
    *dst = 0;
    return (sdst);
}

/*
 * XXX: Should we worry about QUOTE'd chars?
 */
char *
vis_str(Char *cp)
{
    static char *sdst = NULL;
    static size_t dstsize = 0;
    size_t n;
    Char *dp;

    if (cp == NULL)
	return (NULL);

    for (dp = cp; *dp++;)
	continue;
    n = ((dp - cp) << 2) + 1; /* 4 times + NUL */
    if (dstsize < n) {
	sdst = xreallocarray(sdst, n, sizeof(*sdst));
	dstsize = n;
    }
    (void) strvis(sdst, short2str(cp), VIS_NOSLASH);
    return (sdst);
}
