/* Circular doubly-linked list/queue */

#include <stdlib.h>
#include "except.h"
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

/* Destroy a list */
void
list_destroy(List *l)
{
    List *p, *q = l->next;

    for (p = l->next; q != l; p = q) {
        q = p->next;
        free(p->item);
        free(p);
    }
    free(l);
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
    List *lp;
    unsigned long length = 0;

    for (lp = l->next; lp != l; lp = lp->next)
      ++length;

    return length;
}

/* Add an item to the head of a list, returning the new list head */
List *
list_prefix(List *l, void *i)
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
list_suffix(List *l, void *i)
{
    List *n = new(List);

    n->next = l;
    n->prev = l->prev;
    n->item = i;
    l->prev = l->prev->next = n;

    return n;
}

/* Remove the first item of a list, returning the item, or NULL if the
   list is empty. */
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
   list is empty. */
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
