/* Memory management */


#ifndef MEMORY_H
#define MEMORY_H

#include "except.h"

void *exc_malloc(size_t size);
void *exc_calloc(size_t nobj, size_t size);
void *exc_realloc(void *p, size_t size);

/* Malloc space for an object of type T and raise an exception if it
   fails  */
#define new(T) exc_malloc(sizeof(T))

#endif
