/* Test linked lists */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "except.h"
#include "list.h"

char *progname = "list_test";

int main(void)
{
  List *l = list_new();
  List *p;

  /* Check list is empty */
  assert(list_empty(l));

  /* Add lots of items to the list */
  list_suffix(l, "foo");
  list_prefix(l, "bar");
  list_suffix(l, "baz");

  /* Check length */
  assert(list_length(l) == 3);

  /* Check items in list are in the right order, and list is circular;
     N.B. This code is naughty, as it assumes the representation */
  p = l->next;
  assert(strcmp(p->item, "bar") == 0);
  p = p->next;
  assert(strcmp(p->item, "foo") == 0);
  p = p->next;
  assert(strcmp(p->item, "baz") == 0);
  assert(p->next == l);

  /* Check items again, but destructively, and using the API */
  assert(strcmp(list_behead(l), "bar") == 0);
  assert(strcmp(list_betail(l), "baz") == 0);
  assert(strcmp(list_betail(l), "foo") == 0);

  /* Free the list */
  list_free(l);

  warn("succeeded");

  return 0;
}
