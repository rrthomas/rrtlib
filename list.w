%
% Lists
%
% Part of RRTLib C library
%
% (c) Reuben Thomas 1997-2003
%


\documentclass[english]{article}
\usepackage{literate,babel}


\begin{document}
\title{Lists}
\author{Reuben Thomas}
\date{16th August 2003}
\maketitle



This module implements polymorphic doubly-linked circular lists, with functions provided to allow them to act as LIFO and FIFO queues.

\begin{history}
0.00 & 17&Apr&97 &
LIFO lists implemented. \\
0.01 & 18&Apr&97 &
FIFO capability added, and test for empty list. \\
0.02 & 02&Aug&97 &
Length of list function added. \\
0.03 & 06&Nov&97 &
Changed memory allocation to use the exception-throwing version of ¦malloc()¦. \\
0.04 & 09&Mar&98 &
Debugged ¦list_behead()¦ and ¦list_betail()¦. \\
0.05 & 17&Jun&99 &
Changed ¦list_prefix()¦ and ¦list_suffix()¦ to return respectively the new head and tail of the list. \\
0.06 & 27&Nov&00 &
Changed calls to ¦exc_malloc()¦ in accordance with new prototype. Tweaked typesetting. \\
0.07 & 30&Jan&01 &
Changed names to have upper case letters rather than underscores, and made type names start with a capital. \\
0.08 & 10&Feb&01 &
Removed casts on result of allocation functions, as per K\&R errata. Reformatted function definitions to have return type on separate line from name, and open brace on a new line. Changed return type of ¦listLength¦ from ¦unsigned¦ to ¦unsigned long¦. \\
0.09 & 22&Aug&01 &
Used ¦new()¦ where appropriate. \\
0.10 & 16&Aug&03 &
Corrected a formatting typo and improved use of ¦typedef¦. Use ¦new()¦ instead of ¦excMalloc()¦ in one last place.\\
\end{history}



\section{Types}

Lists are constructed from ¦list¦s, which contain links to the previous and next nodes and a pointer to the item itself:

@d List structure
@{
typedef struct List List;
struct List {
    List *prev;
    List *next;
    void *item;
};
@| List @}



\section{Creating and destroying lists}

¦listNew()¦ creates an empty list, returning a pointer to the list.

@d Create a list
@{
List *
listNew(void)
{
    List *l= new(List);

    l->next= l->prev= l; l->item= NULL;
    return l;
}
@| listNew @}

¦listDestroy()¦ destroys a list.

@d Destroy a list
@{
void
listDestroy(List *l)
{
    List *p, *q= l->next;

    for (p= l->next; q != l; p= q) {
        q= p->next;
        free(p->item);
        free(p);
    }
    free(l);
}
@| listDestroy @}



\section{Testing for empty lists}

¦listEmpty()¦ returns 1 if a list is empty and 0 if not.

@d Test for an empty list
@{
int
listEmpty(List *l)
{
    return (l->next == l);
}
@}



\section{Finding the length of lists}

¦listLength()¦ returns the length of a list.

@d Find the length of a list
@{
unsigned long
listLength(List *l)
{
    List *lp;
    unsigned long length= 0;

    for (lp= l->next; lp != l; lp= lp->next) ++length;

    return length;
}
@| listLength @}



\section{Adding items}

¦listPrefix()¦ adds an item to the head of a list, returning the new list head.

@d Prefix an item to a list
@{
List *
listPrefix(List *l, void *i)
{
    List *n= new(List);

    n->next= l->next; n->prev= l; n->item= i;
    l->next->prev= n; l->next= n;

    return n;
}
@| listPrefix @}

¦listSuffix()¦ adds an item to the tail of a list, returning the new list tail.

@d Suffix an item to a list
@{
List *
listSuffix(List *l, void *i)
{
    List *n= new(List);

    n->next= l; n->prev= l->prev; n->item= i;
    l->prev->next= n; l->prev= n;

    return n;
}
@| listSuffix @}



\section{Removing items}

¦listBehead()¦ removes the first item of a list, returning the item, or ¦NULL¦ if the list is empty.

@d Remove the head of a list
@{
void *
listBehead(List *l)
{
    void *i;
    List *d;

    if ((d= l->next) == l) return NULL;
    i= d->item;
    l->next= l->next->next; l->next->prev= l;
    free(d);

    return i;
}
@| listBehead @}

¦listBetail()¦ removes the last item of a list, returning the item, or ¦NULL¦ if the list is empty.

@d Remove the tail of a list
@{
void *
listBetail(List *l)
{
    void *i;
    List *d;

    if ((d= l->prev) == l) return NULL;
    i= d->item;
    l->prev= l->prev->prev; l->prev->next= l;
    free(d);

    return i;
}
@| listBetail @}



\section{Source files}

\subsection{\tt list.h}

Define the externally visible globals and functions.

@o list.h -d
@{
#ifndef RRTLIB_LIST
#define RRTLIB_LIST

@<List structure@>

List *listNew(void);
void listDestroy(List *l);
int listEmpty(List *l);
unsigned long listLength(List *l);
List *listPrefix(List *l, void *i);
List *listSuffix(List *l, void *i);
void *listBehead(List *l);
void *listBetail(List *l);
#endif
@}


\subsection{\tt list.c}

First include standard and RRTLib headers.

@o list.c -d
@{
#include <stdlib.h>
#include "except.h"
#include "list.h"
@}

\cont Then the functions.

@o list.c -d
@{
@<Create a list@>
@<Destroy a list@>
@<Test for an empty list@>
@<Find the length of a list@>
@<Prefix an item to a list@>
@<Suffix an item to a list@>
@<Remove the head of a list@>
@<Remove the tail of a list@>
@}



\begin{nuwebindex}[2]{\section{Index}}
@u
\end{nuwebindex}



\end{document}
