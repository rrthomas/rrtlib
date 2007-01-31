/* Stream utilities */

#include <stdio.h>

#include "except.h"
#include "memory.h"
#include "stream.h"


/* Find the length of the given stream, which must be seekable;
   returns -1 if there is an error */
long
flen(FILE *fp)
{
  long pos = ftell(fp);

  if (pos != -1 && fseek(fp, 0, SEEK_END) == 0) {
    long len = ftell(fp);

    if (len != -1 && fseek(fp, pos, SEEK_SET) == 0)
      return len;
  }

  return -1;
}

/* Read up to the first size bytes of file into the buffer pointed to
   by buf, opening it with mode mode. Returns the number of bytes
   actually read. If there is an error opening or reading the file, an
   exception is thrown.*/
long
slurp(const char *file, const char *mode, long size, char *buf)
{
  FILE *fp = fopen(file, mode);

  if (!fp)
    throw("slurp could not open file");
  fread(buf, sizeof(char), size, fp);
  if (ferror(fp))
    throw("error slurping file");
  fclose(fp);

  return size;
}

char *
getln(FILE *fp)
{
  size_t len = 256;
  int c;
  char *l = exc_malloc(len), *s = l;

  for (c = getc(fp); c != '\n' && c != EOF; c = getc(fp)) {
    if (s == l + len)
      l = exc_realloc(l, len *= 2);
    *s++ = c;
  }
  if (s == l + len)
    l = exc_realloc(l, len + 1);
  *s++ = '\0';

  return exc_realloc(l, s - l);
}
