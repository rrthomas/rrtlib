/* Auto-extending buffer */

#include "memory.h"
#include "buffer.h"


/* Create a buffer with initial size size */
Buffer *
buf_new(size_t size)
{
  Buffer *b = new(Buffer);

  b->size = size;
  b->data = exc_malloc(size);

  return b;
}

/* Ensure a buffer's size is at least size */
Buffer *
buf_resize(Buffer *b, size_t size)
{
  if (b->size < size) {
    b->size = size > b->size * 2 ? size : b->size * 2;
    b->data = exc_realloc(b->data, b->size);
  }

  return b;
}

/* Make a buffer exactly the size of the data it contains */
Buffer *
buf_fit(Buffer *b)
{
  b->data = exc_realloc(b->data, b->used);

  return b;
}
