#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

long flen(FILE *fp);
long slurp(const char *file, const char *mode, char **buf, long size);
char *getLn(FILE *fp);

#endif
