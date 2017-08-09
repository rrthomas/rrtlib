#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <string.h>

/* Test string equality; NULL-pointer safe */
#define strEq(s,t) (((s) && (t)) ? !strcmp((s), (t)) : (s) == (t))

/* A veneer to strcmp() for use with bsearch().
   Also suitable for use with qsort. */
int bstrcmp(const void *s, const void *t);

/* A veneer to strncmp() for use with bsearch().
   The match is performed up to the length of the datum string, which is the
   second parameter. */
int bstrcmp_prefix(const void *s, const void *t);

/* Search for a string s in a sorted array base of size n.
   Returns the index of the string, or -1 if not found. */
ptrdiff_t strBsearch(const char *s, const char *base[], size_t n);

/* Search for a prefix of a string s in a sorted array base of size n.
   Returns the index of the match, or -1 if not found. */
ptrdiff_t strBsearch_prefix(const char *s, const char *base[], size_t n);

#endif
