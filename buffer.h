/* Auto-extending buffer */

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

typedef struct {
  size_t size;
  size_t used;
  void *data;
} Buffer;

Buffer *buf_new(size_t size);
Buffer *buf_resize(Buffer *b, size_t size);
Buffer *buf_fit(Buffer *b);

#endif
