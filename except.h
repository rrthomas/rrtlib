/* Exceptions */

#ifndef EXCEPT_H
#define EXCEPT_H

#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

void exc_init(void);
void vthrow(const char *fmt, va_list arg);
void throw(const char *fmt, ...);

jmp_buf *_try(void);
typedef struct ExcList {
  jmp_buf *env;
  struct ExcList *next;
} ExcList;
extern ExcList *_excBufs;

/* An exception handler has the form:

   try <statement>;   (exceptions may be thrown here)
   catch <statement>; (this is the exception handler)
   endtry;
*/

/* Set up a new exception handler for the next statement */
#define try if (setjmp(*_try()) == 0)

/* Convenient name to introduce the handler statement */
#define catch else

/* Pop the current exception handler */
#define endtry list_behead(_exc_bufs)

extern unsigned long exc_pos;
extern char *exc_file;
extern char *progname;

void vwarn(const char *fmt, va_list arg);
void warn(const char *fmt, ...);
void vdie(const char *fmt, va_list arg);
void die(const char *fmt, ...);

#endif
