#ifndef LIST_H
#define LIST_H

typedef struct List List;
struct List {
    List *prev;
    List *next;
    void *item;
};

List *listNew(void);
void listDestroy(List *l);
int listEmpty(List *l);
unsigned long listLength(List *l);
List *listPrefix(List *l, void *i);
List *listSuffix(List *l, void *i);
void *listBehead(List *l);
void *listBetail(List *l);

#endif
