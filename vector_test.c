/* Test vectors */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "except.h"
#include "vector.h"


char *progname = "vector_test";

int main(void)
{
  Vector *b = vec_new(16);

  /* Check vector's size and number of bytes used */
  assert(b->used == 0);
  assert(b->size == 16);

  /* Grow vector and check size */
  vec_realloc(b, b->size + 1);
  assert(b->size > 16);

  /* Shrink vector and check size */
  vec_realloc(b, 8);
  assert(b->size == 8);

  /* Add a byte to the vector and check it and the vector's size */
  vec_add(b, uint8_t, 27);
  assert(*(uint8_t *)(b->data) == 27);
  assert(b->size == 8);

  /* vec_add a word, having vec_align'ed */

  /* vec_add a block of data */

  /* test size after vec_realloc(b,0) */

  /* Free the vector */
  vec_free(b);

  warn("succeeded");

  return 0;
}
