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
  assert(b->items == 0);
  assert(b->itemsize == 16);

  /* Grow vector and check size */
/*   vec_realloc(b, b->itemsize + 1); */
/*   assert(b->itemsize > 16); */

  /* Free the vector */
  vec_free(b);

  warn("succeeded");

  return 0;
}
