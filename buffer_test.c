/* Test auto-extending buffers */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "except.h"
#include "buffer.h"


char *progname = "buffer_test";

int main(void)
{
  Buffer *b = buf_new(16);

  /* Check buffer's size and number of bytes used */
  assert(b->used == 0);
  assert(b->size == 16);

  /* Grow buffer and check size */
  buf_realloc(b, b->size + 1);
  assert(b->size > 16);

  /* Shrink buffer and check size */
  buf_realloc(b, 8);
  assert(b->size == 8);

  /* Add a byte to the buffer and check it and the buffer's size */
  buf_add(b, uint8_t, 27);
  assert(*(uint8_t *)(b->data) == 27);
  assert(b->size == 8);

  /* buf_add a word, having buf_align'ed */

  /* buf_add a block of data */

  /* test size after buf_realloc(b,0) */

  /* Free the buffer */
  buf_free(b);

  warn("succeeded");

  return 0;
}
