#include <stdio.h>

#include "except.h"
#include "memory.h"

/* Call calloc and throw an exception if it fails */
void *
exc_calloc(size_t nobj, size_t size)
{
  void *p = calloc(nobj, size);

  if (!p && nobj && size)
    throw("could not allocate memory");
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
