#include <stdlib.h>
#include "except.h"
#include "list.h"

List *
listNew(void)
{
    List *l= new(List);

    l->next= l->prev= l; l->item= NULL;
    return l;
}

void
listDestroy(List *l)
{
    List *p, *q= l->next;

    for (p= l->next; q != l; p= q) {
        q= p->next;
        free(p->item);
        free(p);
    }
    free(l);
}

int
listEmpty(List *l)
{
    return (l->next == l);
}

unsigned long
listLength(List *l)
{
    List *lp;
    unsigned long length= 0;

    for (lp= l->next; lp != l; lp= lp->next) ++length;

    return length;
}

List *
listPrefix(List *l, void *i)
{
    List *n= new(List);

    n->next= l->next; n->prev= l; n->item= i;
    l->next->prev= n; l->next= n;

    return n;
}

List *
listSuffix(List *l, void *i)
{
    List *n= new(List);

    n->next= l; n->prev= l->prev; n->item= i;
    l->prev->next= n; l->prev= n;

    return n;
}

void *
listBehead(List *l)
{
    void *i;
    List *d;

    if ((d= l->next) == l) return NULL;
    i= d->item;
    l->next= l->next->next; l->next->prev= l;
    free(d);

    return i;
}

void *
listBetail(List *l)
{
    void *i;
    List *d;

    if ((d= l->prev) == l) return NULL;
    i= d->item;
    l->prev= l->prev->prev; l->prev->next= l;
    free(d);

    return i;
}
