/* Circular doubly-linked lists/queues */

#ifndef LIST_H
#define LIST_H

typedef struct List List;
struct List {
  List *prev;
  List *next;
  void *item;
};

List *list_new(void);
void list_free(List *l);
int list_empty(List *l);
unsigned long list_length(List *l);
List *list_prefix(List *l, void *i);
List *list_suffix(List *l, void *i);
void *list_behead(List *l);
void *list_betail(List *l);

#endif
