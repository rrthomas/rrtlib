%
% Open hash tables
%
% Part of RRTLib C library
%
% Reuben Thomas   22/10/96-27/4/02
%


\documentclass[english]{article}
\usepackage{literate,babel}


\begin{document}

\title{Open hash tables}
\author{Reuben Thomas}
\date{27th April 2002}
\maketitle



\cont This module implements polymorphic open hash tables, and provides string hashing functions.

\begin{history}
0.00 & 22&Oct&96 &
Library started. \\
0.01 & 23&Oct&96 &
Wrote type definitions and function shells, and started function definitions. \\
0.02 & 25&Oct&96 &
Added string hashing function (¦hashpjw¦ from \textit{Red Dragon Book}). \\
0.03 & 26&Oct&96 &
Finished function definitions. Separated objects being hashed into key and body. \\
0.04 & 17&Apr&97 &
Converted module into literate form and made ¦hash_destroy()¦ free all the memory associated with the table rather than just the ¦hash_table¦ structure. \\
0.05 & 06&Nov&97 &
Changed memory allocation to use the exception-throwing wrappers. \\
0.06 & 27&Nov&00 &
Changed calls to allocation functions in accordance with new prototypes. Tweaked typesetting. \\
0.07 & 30&Jan&01 &
Changed names to have upper case letters rather than underscores, and made type names start with a capital. \\
0.08 & 10&Feb&01 &
Removed casts on result of allocation functions, as per K\&R errata. Reformatted function definitions to have return type on separate line from name, and open brace on a new line. Renamed ¦hashStrCmp()¦ to ¦hashStrcmp()¦. \\
0.09 & 13&Feb&01 &
Changed ¦hashInsert¦ to return the body of the existing entry if found, and ¦NULL¦ otherwise, rather than just a return code. \\
0.10 & 09&Mar&01 &
Renamed ¦hashCreate()¦ to ¦hashNew()¦. \\
0.11 & 22&Aug&01 &
Used ¦new()¦ where appropriate. \\
0.12 & 27&Apr&02 &
Include ¦stddef.h¦ in ¦hash.h¦ for ¦size_t¦. \\
\end{history}



\section{Types}

Hash tables are constructed from ¦HashNode¦s, which contain a link to the next node, and pointers to the key and body of the object:

@d Hash node structure
@{
struct _HashNode { struct _HashNode *next; void *key; void *body; };
typedef struct _HashNode HashNode;
@| _HashNode HashNode @}

Hash tables require a hashing function and a comparison function. The hashing function hashes a key and returns a ¦size_t¦, while the comparison function compares two keys and returns a flag which is true if the keys are equal, and false otherwise. The functions have the following types:

@d Hash function types
@{
typedef size_t (*Hasher)(void *);
typedef int (*Comparer)(void *, void *);
@| Hasher Comparer @}

A hash table consists of an array of ¦HashNode¦ chains, the size of the array, and a hashing and comparison function:

@d Hash table structure
@{
typedef struct {
    HashNode **thread;
    size_t size;
    Hasher hash;
    Comparer compare;
} HashTable;
@| HashTable @}



\section{Reason codes}

The hashing functions return various reason codes. These are:

@d Reason codes
@{
#define HASH_OK 0
#define HASH_FOUND 1
#define HASH_NOTFOUND 2
@| HASH_OK HASH_FOUND HASH_NOTFOUND @}

\cont The meanings are described in section~\ref{finding}.



\section{Creating and destroying hash tables}

¦hashNew()¦ creates a hash table of a given size with a given hashing and comparison function. A pointer to the table is returned.

@d Create a hash table
@{
HashTable *
hashNew(size_t size, Hasher hash, Comparer compare)
{
    HashTable *table= new(HashTable);

    table->thread= excCalloc(size, sizeof(HashNode *));
    table->size= size; table->hash= hash;  table->compare= compare;
    return table;
}
@| hashNew @}

¦hashDestroy()¦ destroys a hash table.

@d Destroy a hash table
@{
void
hashDestroy (HashTable *table)
{
    size_t i;
    HashNode *p, *q;

    for (i= 0; i < table->size; i++)
        for (p= table->thread[i]; p != NULL; p= q) {
            q= p->next;
            free(p->key);
            free(p->body);
            free(p);
        }
    free(table->thread);
    free(table);
}
@}



\section{Inserting and removing objects}

¦hashInsert()¦ adds an object to a hash table. If it already exists, the object's body is returned; otherwise, ¦NULL¦ is returned.

@d Insert an object into a hash table
@{
void *
hashInsert(HashTable *table, void *key, void *body)
{
    HashLink l= hashSearch(table, key);
    HashNode *n;

    if (l.found == HASH_FOUND) return l.curr->body;

    n= new(HashNode);

    if (l.prev == NULL) table->thread[l.entry]= n;
    else l.prev->next= n;

    n->next= l.curr;  n->key= key; n->body= body;

    return NULL;
}
@| hashInsert @}

¦hashInsert()¦ calls ¦hashSearch()¦ (see section~\ref{finding}) to discover if an object with the given key already exists, and where it should go if not.

¦hashRemove()¦ removes an object from a hash table, returning
¦HASH_OK¦ or ¦HASH_NOTFOUND¦ if an object with the given key does not exist.

@d Remove an object from a hash table
@{
int
hashRemove(HashTable *table, void *key)
{
    HashLink l= hashSearch(table, key);

    if (l.found == HASH_NOTFOUND) return HASH_NOTFOUND;

    if (l.prev == NULL) table->thread[l.entry]= l.curr->next;
    else l.prev->next= l.curr->next;

    free(l.curr->key);
    free(l.curr->body);
    free(l.curr);

    return HASH_OK;
}
@| hashRemove @}



\section{Finding objects} \label{finding}

¦hashFind()¦ returns the body of the object associated with the given key, or ¦NULL¦ if no object with that key exists.

@d Find an object in a hash table
@{
void *
hashFind(HashTable *table, void *key)
{
    HashLink l= hashSearch(table, key);

    if (l.found == HASH_NOTFOUND) return NULL;

    return l.curr->body;
}
@| hashFind @}

The auxiliary function ¦hashSearch()¦ looks for an object with a given key and returns a structure containing the number of the thread on which it does or should occur, pointers to the object before and at the position on the thread where it is or should be, the object itself, and a code indicating whether it was found or not.

@d Full location of an object in hash table
@{
typedef struct {
    size_t entry;
    HashNode *prev;
    HashNode *curr;
    int found;
} HashLink;
@| HashLink @}

@d Find an object in a hash table, or the place it ought to go
@{
static HashLink
hashSearch(HashTable *table, void *key)
{
    size_t entry= table->hash(key) % table->size;
    HashLink ret;

    ret.entry= entry;
    ret.prev= NULL;
    ret.curr= table->thread[entry];
    ret.found= HASH_NOTFOUND;

    while (ret.curr != NULL) {
        if (table->compare(key, ret.curr->key)) {
            ret.found= HASH_FOUND;
            return ret;
        }
        ret.prev= ret.curr;
        ret.curr= ret.curr->next;
    }

    return ret;
}
@| hashSearch @}



\section{String hashing functions}

Comparison and hashing functions are provided for strings.

@d Hash function for strings
@{
size_t
hashStrHash(void *str)
{
    char *p, *s= (char *)str;
    unsigned long h= 0, g;

    for (p= s; *p != '\0'; p++) {
        h= (h << 4) + *p;
        if ((g= h & 0xf0000000)) {
            h ^= (g >> 24);
            h ^= g;
        }
    }

    return h;
}
@| hashStrHash @}

\cont The algorithm is taken from \textit{Compilers: Principles, Techniques and Tools} by Aho, Sethi and Ullman, p. 435.

@d Comparison function for strings
@{
int hashStrcmp(void *s, void *t)
{
    return (strcmp((char *)s, (char *)t) == 0);
}
@| hashStrcmp @}



\section{Source files}

\subsection{\tt hash.h}

Define the externally visible globals and functions.

@O hash.h -d
@{
#ifndef RRTLIB_HASH
#define RRTLIB_HASH

#include <stddef.h>


@<Hash node structure@>
@<Hash function types@>
@<Hash table structure@>

HashTable *hashNew(size_t size, Hasher hash, Comparer compare);
void hashDestroy(HashTable *table);
void *hashFind(HashTable *table, void *key);
void *hashInsert(HashTable *table, void *key, void *body);
int hashRemove(HashTable *table, void *key);

@<Reason codes@>

size_t hashStrHash(void *);
int hashStrcmp(void *, void *);
#endif
@}


\subsection{\tt hash.c}

First include standard and RRTLib headers.

@o hash.c -d
@{
#include <stdlib.h>
#include <string.h>
#include "except.h"
#include "hash.h"
@}

\cont Then the functions.

@o hash.c -d
@{
@<Create a hash table@>
@<Destroy a hash table@>
@<Full location of an object in hash table@>
@<Find an object in a hash table, or the place it ought to go@>
@<Find an object in a hash table@>
@<Insert an object into a hash table@>
@<Remove an object from a hash table@>

@<Hash function for strings@>
@<Comparison function for strings@>
@}



\begin{nuwebindex}[2]{\section{Index}}
@u
\end{nuwebindex}



\end{document}
