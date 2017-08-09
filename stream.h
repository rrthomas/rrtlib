/* Stream utilities */

#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

/* Find the length of the given stream, which must be seekable;
   returns -1 if there is an error */
long flen(FILE *fp);

#endif
