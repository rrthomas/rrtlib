%
% Stream utilities
%
% Part of RRTLib C library
%
% Reuben Thomas   27/11/00-14/7/01
%


\documentclass[english,a4]{article}
\usepackage{literate,babel}


\begin{document}

\title{Stream utilities}
\author{Reuben Thomas}
\date{14th July 2001}
\maketitle



Various utilities for stream and file I/O.

\begin{history}
  0.00 & 27&Nov&00 &
  Adapted from ¦flen.w¦, which was extracted from Mite code. \\
  0.01 & 30&Jan&01 &
  Changed some names to match changes in the rest of the library. \\
  0.02 & 10&Feb&01 & Added ¦mode¦ argument to ¦slurp()¦, removed casts
  on result of allocation functions, as per K\&R errata, and tided up
  code. Reformatted function definitions to have return type on
  separate line from name, and open brace on a new line. \\
  0.03 & 15&Mar&01 &
  Added ¦size¦ parameter to ¦slurp()¦, and allowed ¦buf¦ to be
  pre-allocated. Make ¦stream.h¦ ¦#include¦ ¦stdio.h¦, which it needs.
  \\
  0.04 & 22&Mar&01 &
  Added ¦getLn()¦ from string library, and changed name of this
  library to ¦stream¦. \\
  0.05 & 14&Jul&01 &
  Removed spurious ¦#include¦ from just before ¦getLn()¦. \\
\end{history}



\section{Stream length}

¦flen()¦ returns the length of the given stream, which it finds by seeking
to the end, or $-1$ if there is an error.

@d Find length of stream
@{
long
flen(FILE *fp)
{
    long pos= ftell(fp);

    if (pos != -1 && fseek(fp, 0, SEEK_END) == 0) {
        long len= ftell(fp);

        if (len != -1 && fseek(fp, pos, SEEK_SET) == 0)
            return len;
    }
    return -1;
}
@| flen @}



\section{Slurp a file}

¦slurp()¦ reads a file into memory, using the given mode to open it,
and returns the file and the number of characters read. If ¦*buf¦ is
non-¦NULL¦, it is assumed to point to a buffer of length at least
¦size¦ into which the first ¦size¦ characters of file are read.
Otherwise, a buffer big enough to hold the file is allocated, and the
entire file is read into it.

@d Read a file into memory
@{
long
slurp(const char *file, const char *mode, char **buf, long size)
{
    FILE *fp= fopen(excFile, mode);

    if (!fp) throw("could not open file");
    if (*buf == NULL) *buf= excMalloc(size);
    else if ((size= flen(fp)) < 0) throw("error getting length of file");
    if (size == 0) return 0;
    fread(*buf, sizeof(char), size, fp);
    if (ferror(fp)) throw("error reading file");
    fclose(fp);
    return size;
}
@| slurp @}



\section{Read a line}

¦getLn()¦ is a safe ¦gets()¦, or alternatively a disadvantaged
person's ¦readline()¦. It reads a line of text from the given stream,
which it returns as a ¦malloc¦ed block. The block is dynamically
extended as needed.

@d Read a line
@{
char *
getLn(FILE *fp)
{
    size_t len= 256;
    int c;
    char *l= excMalloc(len), *s= l;

    for (c= getc(fp); c != '\n' && c != EOF; c= getc(fp)) {
        if (s == l + len) excRealloc(l, len *= 2);
        *s++= c;
    }
    *s= '\0';
    return l;
}
@| getLn @}



\section{The source files}

\subsection{\tt stream.h}

Define the externally visible globals and functions.

@o stream.h -d
@{
#ifndef RRTLIB_STREAM
#define RRTLIB_STREAM

#include <stdio.h>

long flen(FILE *fp);
long slurp(const char *file, const char *mode, char **buf, long size);
char *getLn(FILE *fp);
#endif
@}


\subsection{\tt stream.c}

First include standard and RRTLib headers.

@o stream.c -d
@{
#include <stdio.h>

#include "except.h"
#include "stream.h"
@}

\cont Then the code.

@o stream.c -d
@{
@<Find length of stream@>
@<Read a file into memory@>
@<Read a line@>
@}



\begin{nuwebindex}[2]{\section{Index}}
@u
\end{nuwebindex}



\end{document}
