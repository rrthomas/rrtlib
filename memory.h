/* Memory management */

#ifndef MEMORY_H
#define MEMORY_H

#include "except.h"

void *exc_calloc(size_t nobj, size_t size);
void *exc_realloc(void *p, size_t size);

/* Allocate and zero size bytes, raising an exception if it fails */
#define exc_malloc(size) exc_calloc(1, size)

/* Allocate and zero space for an object of type T and raise an
   exception if it fails  */
#define new(T) exc_calloc(1, sizeof(T))

#endif
