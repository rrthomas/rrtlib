/* Exceptions */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <setjmp.h>

#include "except.h"
#include "memory.h"
#include "list.h"

/* Make a variadic wrapper for an exception routine */
#define unvify(vf, f) \
  void\
  f (const char *fmt, ...) \
  { \
    va_list ap; \
    va_start(ap, fmt); \
    vf(fmt, ap); \
    va_end(ap); \
  }

/* The stack of exception jmp_bufs */
List *_exc_bufs;

/* Position in file for error messages; typically a line number */
unsigned long exc_pos = 0;

/* The name of the file being processed */
char *exc_file = NULL;

/* Initialise exceptions; exc_init() must be called before any other
   exception function */
void
exc_init(void)
{
  _exc_bufs = list_new();
}

/* Raise an exception. If _exc_bufs is non-empty then longjmp() to the
   next handler; otherwise exit the program via vdie(). */
void
vthrow(const char *fmt, va_list arg)
{
  if (!list_empty(_exc_bufs))
    longjmp(*((jmp_buf *)_exc_bufs->next->item), true);
  vdie(fmt, arg);
}
unvify(vthrow, throw)

/* Set up a new exception handler. Called by the try macro. */
jmp_buf *
_try(void)
{
  jmp_buf *env = new(jmp_buf);

  list_prefix(_exc_bufs, env);

  return env;
}

/* Write a warning to stderr of the form:

   progname:exc_file:exc_pos other arguments\n

   progname and exc_file are not displayed if they are NULL, and
   exc_pos isn't displayed if it is 0. */
void
vwarn(const char *fmt, va_list arg)
{
  if (progname)
    fprintf(stderr, "%s:", progname);
  if (exc_file)
    fprintf(stderr, "%s:", exc_file);
  if (exc_pos)
    fprintf(stderr, "%lu:", exc_pos);
  if (progname || exc_file || exc_pos)
    putc(' ', stderr);
  vfprintf(stderr, fmt, arg);
  va_end(arg);
  putc('\n', stderr);
}
unvify(vwarn, warn)

/* Like vwarn, but calls exit() after the warning message has been
   written. */
void
vdie(const char *fmt, va_list arg)
{
  vwarn(fmt, arg);
  exit(EXIT_FAILURE);
}
unvify(vdie, die)
