#include <stdio.h>

#include "except.h"
#include "list.h"
#include "memory.h"

Chunk *
chunkNew(size_t size)
{
    Chunk *ch= new(Chunk);

    ch->size= size;
    if (size)
        ch->block= excCalloc(size, sizeof(Byte));

    return ch;
}

void
chunkDestroy(Chunk *ch)
{
    if (ch->size) free(ch->block);
    free(ch);
}

void
chunkAdd(List *l, unsigned long w, unsigned n)
{
    size_t i;
    Chunk *ch;

    if (n > 4) throw("word to add to list too large");

    ch= chunkNew(n);
    for (i= 0; i < n; i++) {
        (ch->block)[i]= w & 0xff;
        w >>= 8;
    }

    listSuffix(l, ch);
}
