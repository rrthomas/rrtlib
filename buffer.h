/* Auto-extending buffers */

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  size_t size;                  /* number of bytes reserved in data */
  size_t used;                  /* number of bytes used in data */
  uint8_t *data;
} Buffer;

Buffer *buf_new(size_t size);
void buf_free(Buffer *b);
uint8_t *buf_toarray(Buffer *b);
Buffer *buf_realloc(Buffer *b, size_t size);
Buffer *buf_grow(Buffer *b, size_t size);
void buf_addblk(Buffer *b, size_t n, const uint8_t *d);

#define buf_size(b) (b)->size
#define buf_used(b) (b)->used
#define buf_data(b) (b)->data

/* Align buffer b's b->used to the nearest n bytes (n a power of 2) */
#define buf_align(b, n) \
  (b)->used = (size_t)((b)->used + n - 1) & ~(n - 1)

/* Add an object d of type t to a buffer b */
#define buf_add(b, t, d) \
  (b) = buf_grow((b), (b)->used + sizeof(t)), \
  *(t *)((uint8_t *)(b)->data + b->used) = (d), \
  (b)->used += sizeof(t)


#endif
