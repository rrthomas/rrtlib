#include <stddef.h>
#include <string.h>

#include "except.h"
#include "string.h"

int
bstrcmp(const void *s, const void *t)
{
    return strcmp(*(char **)s, *(char **)t);
}

int
bstrcmpp(const void *s, const void *t)
{
    return strncmp(*(const char **)s, *(const char **)t,
        strlen(*(const char **)t));
}

ptrdiff_t
strBsearch(const char *s, const char *base[], size_t n)
{
    const char **p= (const char **)bsearch(&s, base, n, sizeof(char *),
        bstrcmp);

    if (!p) return -1;
    return p - base;
}

ptrdiff_t
strBsearchp(const char *s, const char *base[], size_t n)
{
    const char **p= (const char **)bsearch(&s, base, n, sizeof(char *),
        bstrcmpp);

    if (!p) return -1;
    return p - base;
}
