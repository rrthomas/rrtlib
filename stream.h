/* Stream utilities */

#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

long flen(FILE *fp);
long slurp(const char *file, const char *mode, long size, char *buf);
char *getln(FILE *fp);

#endif
