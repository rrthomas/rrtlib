/* Test auto-extending buffers */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "except.h"
#include "buffer.h"


char *progname = "buffer_test";

int main(void)
{
  Buffer *b = buf_new();

  /* Check buffer's size and number of bytes used */
  assert(b->items == 0);
  assert(b->size == 0);

  /* Add a byte to the buffer and check it and the buffer's size */
  buf_add(b, uint8_t, 27);
  assert(*(uint8_t *)(b->array) == 27);
  assert(b->items == 1);

  /* buf_add a word, having buf_align'ed */

  /* buf_add a block of data */

  /* Free the buffer */
  buf_free(b);

  warn("succeeded");

  return 0;
}
