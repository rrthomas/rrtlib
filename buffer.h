/* Auto-extending buffers */

#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
  size_t size;
  size_t used;
  void *data;
} Buffer;

/* Create a buffer with initial size size */
Buffer *buf_new(size_t size);
/* Ensure a buffer's size is at least size */
Buffer *buf_resize(Buffer *b, size_t size);
/* Make a buffer exactly the size of the data it contains */
Buffer *buf_fit(Buffer *b);

#endif
