#ifndef MEMORY_H
#define MEMORY_H

#include "except.h"
#include "list.h"

typedef unsigned char Byte;

typedef struct {
    size_t size;
    Byte *block;
} Chunk;

Chunk *chunkNew(size_t size);
void chunkDestroy(Chunk *ch);
void chunkAdd(List *l, unsigned long w, unsigned n);

#endif
