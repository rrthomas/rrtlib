/* Auto-extending buffers */

#include <string.h>

#include "memory.h"
#include "buffer.h"


/* Create a buffer with initial size size */
Buffer *
buf_new(size_t size)
{
  Buffer *b = new(Buffer);
  b->size = size;
  b->used = 0;
  b->data = exc_malloc(size);
  return b;
}

/* Free a buffer b */
void
buf_free(Buffer *b)
{
  free(b->data);
  free(b);
}

/* Resize a buffer b to max(size, b->used) */
Buffer *
buf_realloc(Buffer *b, size_t size)
{
  b->size = size > b->used ? size : b->used;
  b->data = exc_realloc(b->data, b->size);
  return b;
}

/* Ensure a buffer b is at least size size */
Buffer *
buf_grow(Buffer *b, size_t size)
{
  if (size > b->size) {
    b->size = size > b->size * 2 ? size : b->size * 2;
    b->data = exc_realloc(b->data, b->size);
  }
  return b;
}

/* Add a block of data d of size n to a buffer b */
void
buf_addblk(Buffer *b, size_t n, const void *d)
{
  b = buf_realloc(b, b->used + n);
  b->used += n;
  memcpy((char *)b->data + b->used, d, n);
}
