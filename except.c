#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <setjmp.h>

#include "except.h"
#include "list.h"

#define unVify(vf, f) \
    void\
    f (const char *fmt, ...) \
    { \
        va_list ap; \
        va_start(ap, fmt); \
        vf(fmt, ap); \
        va_end(ap); \
    }

static List *_excBufs;
unsigned long excPos= 0;
char *excFile= NULL;

void
excInit(void)
{
    _excBufs= listNew();
}

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

void
vDie(const char *fmt, va_list arg)
{
    vWarn(fmt, arg);
    exit(EXIT_FAILURE);
}
unVify(vDie, die)

void
vThrow(const char *fmt, va_list arg)
{
    if (!listEmpty(_excBufs))
        longjmp(*((jmp_buf *)_excBufs->next->item), true);
    vDie(fmt, arg);
}
unVify(vThrow, throw)

jmp_buf *
_excEnv(void)
{
    jmp_buf *env= new(jmp_buf);

    listPrefix(_excBufs, env);
    return env;
}

void
_endTry(void)
{
    if (!listEmpty(_excBufs)) listBehead(_excBufs);
}

void *
excMalloc(size_t size)
{
    void *p= malloc(size);

    if (!p && size) throw("could not allocate memory");
    return p;
}

void *
excCalloc(size_t nobj, size_t size)
{
    void *p= calloc(nobj, size);

    if (!p && nobj && size) throw("could not allocate memory");
    return p;
}

void *
excRealloc(void *p, size_t size)
{
    if (!(p= realloc(p, size)) && size)
        throw("could not reallocate memory");
    return p;
}
