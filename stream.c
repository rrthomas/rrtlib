#include <stdio.h>

#include "except.h"
#include "stream.h"

long
flen(FILE *fp)
{
    long pos= ftell(fp);

    if (pos != -1 && fseek(fp, 0, SEEK_END) == 0) {
        long len= ftell(fp);

        if (len != -1 && fseek(fp, pos, SEEK_SET) == 0)
            return len;
    }
    return -1;
}

long
slurp(const char *file, const char *mode, char **buf, long size)
{
    FILE *fp= fopen(excFile, mode);

    if (!fp) throw("could not open file");
    if (*buf == NULL) *buf= excMalloc(size);
    else if ((size= flen(fp)) < 0) throw("error getting length of file");
    if (size == 0) return 0;
    fread(*buf, sizeof(char), size, fp);
    if (ferror(fp)) throw("error reading file");
    fclose(fp);
    return size;
}

char *
getLn(FILE *fp)
{
    size_t len= 256;
    int c;
    char *l= excMalloc(len), *s= l;

    for (c= getc(fp); c != '\n' && c != EOF; c= getc(fp)) {
        if (s == l + len) excRealloc(l, len *= 2);
        *s++= c;
    }
    *s= '\0';
    return l;
}
