#include <stdio.h>

#include "except.h"
#include "memory.h"

/* Call malloc and throw an exception if it fails */
void *
exc_malloc(size_t size)
{
  void *p = malloc(size);

  if (!p && size)
    throw("could not allocate memory in malloc");
  return p;
}

/* Call calloc and throw an exception if it fails */
void *
exc_calloc(size_t nobj, size_t size)
{
  void *p = calloc(nobj, size);

  if (!p && nobj && size)
    throw("could not allocate memory in calloc");
  return p;
}

/* Call realloc and throw an exception if it fails */
void *
exc_realloc(void *p, size_t size)
{
  if (!(p = realloc(p, size)) && size)
    throw("could not reallocate memory");
  return p;
}
