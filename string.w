%
% String utilities
%
% Part of RRTLib C library
%
% Reuben Thomas   25/5/97-23/3/01
%


\documentclass[english]{article}
\usepackage{literate,babel}


\begin{document}

\title{String Utilities}
\author{Reuben Thomas}
\date{23rd March 2001}
\maketitle



String utilities, including string array searching and a safe ¦gets()¦.

\begin{history}
0.00 & 25&May&97 &
Added ¦str_bsearch()¦ and ¦str_bsearchp()¦. \\
0.01 & 16&Oct&97 &
Removed ¦bstrcmp()¦ and ¦bstrcmpp()¦ from the header. \\
0.02 & 25&Nov&97 &
Added ¦str_eq()¦ macro. \\
0.03 & 22&Jan&99 &
Made ¦str_eq()¦ work with ¦NULL¦ arguments. \\
0.04 & 27&Nov&00 &
Changed ¦strutil¦ to ¦string¦. Added ¦getln()¦. \\
0.05 & 30&Jan&01 &
Changed names to have upper case letters rather than underscores. \\
0.06 & 10&Feb&01 &
Removed casts on result of allocation functions, as per K\&R errata, and added missing ¦#include¦ of ¦except.h¦ thereby revealed. Reformatted function definitions to have return type on separate line from name, and open brace on a new line. \\
0.07 & 22&Mar&01 &
Removed ¦getLn()¦ to streams library. \\
0.08 & 23&Mar&01 &
Tidied up the types and return values of ¦strBsearch()¦ and ¦strBsearchp()¦. \\
\end{history}



\section{String equality macro}

A macro for testing string equality which works with ¦NULL¦ arguments.

@d String equality test
@{#define strEq(s,t) (((s) && (t)) ? !strcmp((s), (t)) : (s) == (t))
@| strEq @}



\section{String array searching}

¦bstrcmp()¦ is just a veneer to ¦strcmp()¦ for use with ¦bsearch()¦.

@d String compare for {\tt bsearch()}
@{
int
bstrcmp(const void *s, const void *t)
{
    return strcmp(*(char **)s, *(char **)t);
}
@| bstrcmp @}

¦bstrcmpp()¦ is a veneer to ¦strncmp()¦ for use with ¦bsearch()¦. The match is performed up to the length of the datum string, which is the second parameter.

@d Prefix string compare for {\tt bsearch}
@{
int
bstrcmpp(const void *s, const void *t)
{
    return strncmp(*(const char **)s, *(const char **)t,
        strlen(*(const char **)t));
}
@}

Two functions are provided to search for strings in arrays. Both return the index of the token, or $-1$ if it is not found. ¦strBsearch()¦ requires an exact match; ¦strBsearchp()¦ will match a prefix of the string supplied. Both functions must be passed the size of the token table.

@d Find a string in a string array
@{
ptrdiff_t
strBsearch(const char *s, const char *base[], size_t n)
{
    const char **p= (const char **)bsearch(&s, base, n, sizeof(char *),
        bstrcmp);

    if (!p) return -1;
    return p - base;
}
@| strBsearch @}

@d Find a prefix in a string array
@{
ptrdiff_t
strBsearchp(const char *s, const char *base[], size_t n)
{
    const char **p= (const char **)bsearch(&s, base, n, sizeof(char *),
        bstrcmpp);

    if (!p) return -1;
    return p - base;
}
@| strBsearchp @}



\section{The source files}

\subsection{\tt string.h}

Define the externally visible globals and functions.

@o string.h -d
@{
#ifndef RRTLIB_STRING
#define RRTLIB_STRING

#include <stddef.h>
#include <string.h>

@<String equality test@>
ptrdiff_t strBsearch(const char *s, const char *base[], size_t n);
ptrdiff_t strBsearchp(const char *s, const char *base[], size_t n);
#endif
@}


\subsection{\tt string.c}

First include standard and RRTLib headers.

@o string.c -d
@{
#include <stddef.h>
#include <string.h>

#include "except.h"
#include "string.h"
@}

\cont Then the code.

@o string.c -d
@{
@<String compare for {\tt bsearch()}@>
@<Prefix string compare for {\tt bsearch}@>
@<Find a string in a string array@>
@<Find a prefix in a string array@>
@}



\begin{nuwebindex}[2]{\section{Index}}
@u
\end{nuwebindex}



\end{document}
