%
% Exception handling
%
% Part of RRTLib C library
%
% Reuben Thomas   12/2/97-16/4/02
%


\documentclass[english]{article}
\usepackage{literate,babel}


\begin{document}

\title{Exception handling}
\author{Reuben Thomas}
\date{16th April 2002}
\maketitle



Simple exception handling. Errors can be warning messages, exceptions
or fatal errors. An error message can be printed, and optionally a
line number appended. Wrappers are provided for |malloc()|, |calloc()|
and |realloc()|.

\begin{history}
  0.00 & 12&Feb&97 &
  Nuweb document created from C source; |EXC_LINENO| added. \\
  0.01 & 01&Apr&97 &
  Added exceptions. \\
  0.02 & 16&Apr&97 &
  Set |exc_fatal| to |TRUE| by default, rather than |FALSE|. \\
  0.03 & 08&May&97 &
  Removed from Mite and added to RRTLib. Added |exc_malloc()|,
  |exc_calloc()| and |exc_realloc()|. \\
  0.04 & 26&May&97 &
  Changed behaviour of |EXC_LINE| so that it only causes the line
  number to be printed if |exc_line| is non-zero. \\
  0.05 & 29&May&97 &
  Added exception handler stack so that exceptions can be nested. \\
  0.06 & 11&Mar&98 &
  Added |exc_file| for printing file name in error messages. \\
  0.07 & 10&Jun&99 &
  Added |exc_vthrow()|. \\
  0.08 & 13&Jun&99 &
  Made |exc_vthrow()| print file name (|exc_file|) if |EXC_LINENO| is
  set but |exc_line| is $0$. \\
  0.09 & 19&Jun&99 &
  Made |exc_throw()| call |va_end()|. \\
  0.10 & 30&Jun&99 &
  Made the allocation functions return |NULL| (or the same pointer in
  the case of |exc_realloc()| if an area of size zero is requested. \\
  0.11 & 25&Oct&99 &
  Removed some typesetting problems. \\
  0.12 & 27&Nov&00 &
  Divided |exc_throw()| into |throw()|, |warn()| and |die()|, and
  removed exception codes. Added |unvify()| macro to make variadic
  versions of exception functions. Renamed |catch| to |try|, added a
  dummy |catch|, and renamed |exc_end()| to |_end_try()|, adding the
  |end_try| macro to match |try| and |catch|. Tweaked the typesetting.
  \\
  0.13 & 30&Jan&01 &
  Changed names to have upper case letters rather than underscores. \\
  0.14 & 10&Feb&01 &
  Removed casts on result of allocation functions, as per K\&R errata.
  Reformatted function definitions to have return type on separate
  line from name, and open brace on a new line. \\
  0.15 & 20&Mar&01 &
  Added |new()| macro. \\
  0.16 & 23&Mar&01 &
  Made error message arguments |const|. Only print |progName| in
  messages if it is non-|NULL|. \\
  0.17 & 19&Jul&01 &
  Corrected declaration of |_endTry| in |except.h| (was |tryEnd|). \\
  0.18 & 22&Aug&01 &
  Used |new()| where appropriate. \\
  0.19 & 24&Oct&01 &
  Defined |catch| as |else|, which makes more sense: the handler
  statement is only executed if an exception is raised. Moved
  definition of |endTry| to |except.h|, where it should be, from
  |except.c|. \\
  0.20 & 16&Apr&02 &
  Renamed |excLine| to |excPos|, as the position may not always be a
  line number. \\
\end{history}



\section{Useful macros}

Definitions for true and false booleans; since many headers define
them, we undefine them first:

@d Truth values
@{
#undef FALSE
#undef TRUE
#define FALSE 0
#define TRUE 1
@| FALSE TRUE @}

A macro to make a variadic wrapper for an exception routine:

@d Make a variadic exception wrapper for an exception routine
@{
#define unVify(vf, f) \
    void\
    f (const char *fmt, ...) \
    { \
        va_list ap; \
        va_start(ap, fmt); \
        vf(fmt, ap); \
        va_end(ap); \
    }
@| unVify @}



\section{Globals}

The following global variables are used:

@d Exception variables
@{
static List *_excBufs;
unsigned long excPos= 0;
char *excFile= NULL;
@| _excBufs excPos excFile @}

\cont Note that |excPos| defaults to 0; it should be set to 1 when
the processing of a file begins if errors are to be printed with line
numbers.

|_excBufs| holds the stack of exception |jmp_buf|s. If |_excBufs| is
non-empty then exceptions are handled by the program: when
|excThrow()| finishes it performs a |longjmp()|.

|_excBufs| is initialised by |excInit()|, which must be called before
any other exception function is used.

@d Initialise globals
@{
void
excInit(void)
{
    _excBufs= listNew();
}
@| excInit @}



\section{Print a warning}

|vWarn()| writes the name of the program (|progName|) to |stderr| if
it is non-|NULL|, followed optionally by the file name (|excFile|), if
non-|NULL|, and line number (|excPos|), if non-zero. The supplied
message is then printed and a newline is appended.

@d Print a warning
@{
void
vWarn(const char *fmt, va_list arg)
{
    if (progName) fprintf(stderr, "%s:", progName);
    if (excFile) fprintf(stderr, "%s:", excFile);
    if (excPos) fprintf(stderr, "%lu:", excPos);
    if (progName || excFile || excPos) putc(' ', stderr);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
    putc('\n', stderr);
}
unVify(vWarn, warn)
@| vWarn warn @}



\section{Die with a fatal error}

|vDie()| prints out the error message, then calls |exit()|.

@d Die with a fatal error
@{
void
vDie(const char *fmt, va_list arg)
{
    vWarn(fmt, arg);
    exit(EXIT_FAILURE);
}
unVify(vDie, die)
@| vDie die @}



\section{Raise an exception}

|vThrow()| raises an exception. If |_excBufs| is non-empty then it
performs |longjmp()| to the next outermost handler; otherwise it exits
the program via |vDie()|.

@d Raise an exception
@{
void
vThrow(const char *fmt, va_list arg)
{
    if (!listEmpty(_excBufs))
        longjmp(*((jmp_buf *)_excBufs->next->item), TRUE);
    vDie(fmt, arg);
}
unVify(vThrow, throw)
@| vThrow throw @}



\section{Exception handler}

An exception handler starts with |try| followed by a statement within
which exceptions may be thrown, then |catch| followed by another
statement which is the handler, and ends with |endTry|.

@d Exception handler
@{
#define try if (setjmp(*_excEnv()) == 0)
#define catch else
#define endTry _endTry()
@| try catch endTry @}

@d Exception handler internals
@{
jmp_buf *
_excEnv(void)
{
    jmp_buf *env= new(jmp_buf);

    listPrefix(_excBufs, env);
    return env;
}
@| _excEnv @}

\cont |_endTry()| is called by |endTry| to decrement the nesting level
and pop the current |jmp_buf|, if any.

@d End of exception handler
@{
void
_endTry(void)
{
    if (!listEmpty(_excBufs)) listBehead(_excBufs);
}
@| _endTry @}



\section{Memory allocation with exception handling}

One of the commonest uses of exceptions is immediately after calling
|malloc()|, |calloc()| or |realloc()|. |excMalloc()|, |excCalloc()|
and |excRealloc()| provide wrappers for |malloc()|, |calloc()| and
|realloc()| which raise an exception if the allocation fails, and
silently do nothing if a block of size zero is requested.

@d {\tt{}malloc()} wrapper
@{
void *
excMalloc(size_t size)
{
    void *p= malloc(size);

    if (!p && size) throw("could not allocate memory");
    return p;
}
@| excMalloc @}

@d {\tt{}calloc()} wrapper
@{
void *
excCalloc(size_t nobj, size_t size)
{
    void *p= calloc(nobj, size);

    if (!p && nobj && size) throw("could not allocate memory");
    return p;
}
@| excCalloc @}

@d {\tt{}realloc()} wrapper
@{
void *
excRealloc(void *p, size_t size)
{
    if (!(p= realloc(p, size)) && size)
        throw("could not reallocate memory");
    return p;
}
@| excRealloc @}

|new()| provides a quick wrapper for perhaps the commonest use of
|malloc()|, that is, to allocate a particular type on the heap:

@d Object creation
@{#define new(T) excMalloc(sizeof(T))
@| new @}


\section{The source files}

\subsection{\tt except.h}

Define the externally visible globals and functions.

@O except.h -d
@{
#ifndef RRTLIB_EXCEPT
#define RRTLIB_EXCEPT

#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

@<Truth values@>
void excInit(void);
void vWarn(const char *fmt, va_list arg);
void warn(const char *fmt, ...);
void vDie(const char *fmt, va_list arg);
void die(const char *fmt, ...);
void vThrow(const char *fmt, va_list arg);
void throw(const char *fmt, ...);
@<Exception handler@>
jmp_buf *_excEnv(void);
void _endTry(void);
void *excMalloc(size_t size);
void *excCalloc(size_t nobj, size_t size);
void *excRealloc(void *p, size_t size);
@<Object creation@>
extern unsigned long excPos;
extern char *excFile;
extern char *progName;
#endif
@}


\subsection{\tt except.c}

First include standard and RRTLib headers.

@o except.c -d
@{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

#include "except.h"
#include "list.h"
@}

\cont Then the code.

@o except.c -d
@{
@<Make a variadic exception wrapper...@>
@<Exception variables@>
@<Initialise globals@>
@<Print a warning@>
@<Die with a fatal error@>
@<Raise an exception@>
@<Exception handler internals@>
@<End of exception handler@>

@<{\tt{}malloc()} wrapper@>
@<{\tt{}calloc()} wrapper@>
@<{\tt{}realloc()} wrapper@>
@}



\begin{nuwebindex}[2]{\section{Index}}
@u
\end{nuwebindex}



\end{document}
