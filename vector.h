/* Vectors (extensible arrays) */

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  size_t size;                  /* number of bytes reserved in data */
  size_t used;                  /* number of bytes used in data */
  uint8_t *data;
} Vector;

Vector *vec_new(size_t size);
void vec_free(Vector *b);
uint8_t *vec_toarray(Vector *b);
Vector *vec_realloc(Vector *b, size_t size);
Vector *vec_grow(Vector *b, size_t size);
void vec_addblk(Vector *b, size_t n, const uint8_t *d);

#define vec_size(b) (b)->size
#define vec_used(b) (b)->used
#define vec_data(b) (b)->data

/* Align buffer b's b->used to the nearest n bytes (n a power of 2) */
#define vec_align(b, n) \
  (b)->used = (size_t)((b)->used + n - 1) & ~(n - 1)

/* Add an object d of type t to a buffer b */
#define vec_add(b, t, d) \
  (b) = vec_grow((b), (b)->used + sizeof(t)), \
  *(t *)((uint8_t *)(b)->data + b->used) = (d), \
  (b)->used += sizeof(t)


#endif
