/* Vectors (extensible arrays) */

#include <stdint.h>
#include <string.h>

#include "memory.h"
#include "vector.h"


/* Create a buffer with initial size size */
Vector *
vec_new(size_t size)
{
  Vector *b = new(Vector);
  b->size = size;
  b->used = 0;
  b->data = exc_malloc(size);
  return b;
}

/* Free a buffer b */
void
vec_free(Vector *b)
{
  free(b->data);
  free(b);
}

/* Convert a buffer to a byte array */
uint8_t *
vec_toarray(Vector *b) {
  uint8_t *d;
  vec_realloc(b, 0);
  d = b->data;
  free(b);
  return d;
}

/* Resize a buffer b to max(size, b->used) */
Vector *
vec_realloc(Vector *b, size_t size)
{
  b->size = size > b->used ? size : b->used;
  b->data = exc_realloc(b->data, b->size);
  return b;
}

/* Ensure a buffer b is at least size size */
Vector *
vec_grow(Vector *b, size_t size)
{
  if (size > b->size) {
    b->size = size > b->size * 2 ? size : b->size * 2;
    b->data = exc_realloc(b->data, b->size);
  }
  return b;
}

/* Add an n-byte block of data d of to a buffer b */
void
vec_addblk(Vector *b, size_t n, const uint8_t *d)
{
  b = vec_realloc(b, b->used + n);
  memcpy((char *)b->data + b->used, d, n);
  b->used += n;
}
