/* Circular doubly-linked lists/queues */

#include <stdlib.h>

#include "except.h"
#include "memory.h"
#include "list.h"


/* Create an empty list, returning a pointer to the list */
List *
list_new(void)
{
  List *l = new(List);

  l->next = l->prev = l;
  l->item = NULL;

  return l;
}

/* Free a list */
void
list_free(List *l)
{
  List *p = l, *q;

  do {
    q = p;
    p = p->next;
    free(q);
  } while (p != l);
}

/* Test whether a list is empty */
int
list_empty(List *l)
{
    return l->next == l;
}

/* Return the length of a list */
unsigned long
list_length(List *l)
{
  List *p;
  unsigned long length = 0;

  for (p = l->next; p != l; p = p->next)
    ++length;

  return length;
}

/* Add an item to the head of a list, returning the new list head */
List *
list_prefix(List *l, const void *i)
{
  List *n = new(List);

  n->next = l->next;
  n->prev = l;
  n->item = i;
  l->next = l->next->prev = n;

  return n;
}

/* Add an item to the tail of a list, returning the new list tail */
List *
list_suffix(List *l, const void *i)
{
  List *n = new(List);

  n->next = l;
  n->prev = l->prev;
  n->item = i;
  l->prev = l->prev->next = n;

  return n;
}

/* Return the first item of a list, or NULL if the list is empty */
void *
list_head(List *l)
{
  List *d = l->next;

  if (d == l)
    return NULL;

  return d->item;
}

/* Remove the first item of a list, returning the item, or NULL if the
   list is empty */
void *
list_behead(List *l)
{
  void *i;
  List *d = l->next;

  if (d == l)
    return NULL;
  i = d->item;
  l->next = l->next->next;
  l->next->prev = l;
  free(d);

  return i;
}

/* Remove the last item of a list, returning the item, or NULL if the
   list is empty */
void *
list_betail(List *l)
{
  void *i;
  List *d = l->prev;

  if (d == l)
    return NULL;
  i = d->item;
  l->prev = l->prev->prev;
  l->prev->next = l;
  free(d);

  return i;
}
