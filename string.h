#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <string.h>

#define strEq(s,t) (((s) && (t)) ? !strcmp((s), (t)) : (s) == (t))

ptrdiff_t strBsearch(const char *s, const char *base[], size_t n);
ptrdiff_t strBsearchp(const char *s, const char *base[], size_t n);

#endif
