%
% Memory utilities
%
% Part of RRTLib C library
%
% Reuben Thomas   4/3/98-22/8/01
%


\documentclass[english,a4]{article}
\usepackage{literate,babel}


\begin{document}

\title{Memory utilities}
\author{Reuben Thomas}
\date{22nd August 2001}
\maketitle



Auto-extending buffers and linked lists of small memory chunks.

\begin{history}
  0.00 & 04&Mar&98 &
  Extracted from Mite code. \\
  0.01 & 05&Mar&98 &
  Changed ¦BYTE¦ to ¦byte¦ to avoid conflict with Mite's definition,
  and debugged ¦chunk_add()¦. \\
  0.02 & 09&Mar&98 &
  Tidied up a little. \\
  0.03 & 18&Mar&98 &
  Changed include of ¦<list.h>¦ to ¦"list.h"¦. \\
  0.04 & 20&Nov&98 &
  Allowed zero-length chunks. \\
  0.05 & 23&Jan&99 &
  Stopped ¦chunk_destroy()¦ trying to free the chunk's block when it
  does not exist (because it is a size zero chunk). \\
  0.06 & 16&Jun&99 &
  Corrected a typo in the documentation. \\
  0.07 & 27&Nov&00 &
  Tweaked typsetting. \\
  0.08 & 07&Dec&00 &
  Changed to new exception API. \\
  0.09 & 30&Jan&01 &
  Changed names to have upper case letters rather than underscores,
  and made type names start with a capital. \\
  0.10 & 10&Feb&01 &
  Removed casts on result of allocation functions, as per K\&R errata.
  Reformatted function definitions to have return type on separate
  line from name, and open brace on a new line. \\
  0.11 & 15&Mar&01 &
  Remove cast to ¦void *¦ as it is unnecessary. \\
  0.12 & 22&Mar&01 &
  Changed name to ¦memory¦ and added auto-extending buffers. \\
  0.13 & 19&Jul&01 &
  Added ¦#include¦ of ¦except.h¦ to ¦memory.h¦. \\
  0.14 & 24&Jul&01 &
  Changed ¦RRTLIB_CHUNK¦ to ¦RRTLIB_MEMORY¦, and tidied up some typos.
  \\
  0.15 & 22&Aug&01 &
  Used ¦new()¦ where appropriate. \\
\end{history}



\section{Auto-extending buffers}

¦bufNew(s, i)¦ creates a buffer with initial size ¦i¦, storing the size in
lvalue ¦s¦. When the buffer needs to be extended, ¦bufExt()¦ is called to
do the job. It can also be called prophylactically, so that for example, it
could be called every time new data is written to the buffer.

Finally, ¦bufShrink()¦ shrinks a buffer to the size of the data it
contains. This is typically used when a quantity being built up in a buffer
has finished being constructed.

@d Auto-extending buffers
@{
#define max(a, b) ((a) > (b) ? (a) : (b))
#define bufNew(size, init) excMalloc(((size)= (init)))
#define bufExt(buf, size, need) \
    if ((size) < (need)) \
        (buf)= excRealloc((buf), (size)= max((size) * 2, (need)))
#define bufShrink(buf, used) excRealloc((buf), (used))
@| max bufNew bufExt bufShrink @}



\section{Chunk structure}

A chunk consists of a size and a block of bytes. The definition of
¦Byte¦ used here should work on most systems; a configuration header
file for RRTLib would be needed to make it completely portable.


@d Chunk structure
@{
typedef unsigned char Byte;
typedef struct {
    size_t size;
    Byte *block;
} Chunk;
@}



\section{Create a chunk}

¦chunkNew()¦ returns a new chunk of size ¦size¦.

@d Create a chunk
@{
Chunk *
chunkNew(size_t size)
{
    Chunk *ch= new(Chunk);

    ch->size= size;
    if (size)
        ch->block= excCalloc(size, sizeof(Byte));

    return ch;
}
@| chunkNew @}



\section{Destroy a chunk}

¦chunkDestroy()¦ destroys a chunk, releasing its storage.

@d Destroy a chunk
@{
void
chunkDestroy(Chunk *ch)
{
    if (ch->size) free(ch->block);
    free(ch);
}
@| chunkDestroy @}



\section{Add a chunk to a list}

¦chunkAdd()¦ adds a chunk of 1--4 bytes to a list:

@d Add a chunk to a list
@{
void
chunkAdd(List *l, unsigned long w, unsigned n)
{
    size_t i;
    Chunk *ch;

    if (n > 4) throw("word to add to list too large");

    ch= chunkNew(n);
    for (i= 0; i < n; i++) {
        (ch->block)[i]= w & 0xff;
        w >>= 8;
    }

    listSuffix(l, ch);
}
@| chunkAdd @}

\cont The ¦n¦ least-significant bytes of ¦w¦ are stored in consecutive
bytes in a new chunk which is then added to the list ¦l¦. If ¦n¦ is
greater than 4 an exception is raised.




\section{The source files}

\subsection{\tt memory.h}

Define the externally visible globals and functions.

@o memory.h -d
@{
#ifndef RRTLIB_MEMORY
#define RRTLIB_MEMORY

#include "except.h"
#include "list.h"

@<Auto-extending buffers@>
@<Chunk structure@>
Chunk *chunkNew(size_t size);
void chunkDestroy(Chunk *ch);
void chunkAdd(List *l, unsigned long w, unsigned n);
#endif
@}


\subsection{\tt memory.c}

First include standard and RRTLib headers.

@o memory.c -d
@{
#include <stdio.h>

#include "except.h"
#include "list.h"
#include "memory.h"
@}

\cont Then the code.

@o memory.c -d
@{
@<Create a chunk@>
@<Destroy a chunk@>
@<Add a chunk to a list@>
@}



\begin{nuwebindex}[2]{\section{Index}}
@u
\end{nuwebindex}



\end{document}
