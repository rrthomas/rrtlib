/* Open hash tables */

#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "except.h"
#include "hash.h"


/* Create a hash table of size 1<<size */
HashTable *
hash_new(size_t size)
{
  HashTable *t = new(HashTable);

  if (size > HASH_MAX_SIZE)
    throw("hash too big");
  t->size = (1 << size) - 1;
  t->chain = exc_calloc(t->size, sizeof(HashNode));

  return t;
}

/* Free a hash table */
void
hash_free (HashTable *t)
{
  HashNode *c, *p, *q, *end = t->chain + t->size;

  for (c = t->chain; c < end; c++)
    for (p = c->next; p != NULL; p = q) {
      q = p->next;
      free(p);
    }
  free(t->chain);
  free(t);
}

/* Search a hash table for an object with key key, returning a pointer
   to the previous HashNode. If there is no such key, the next field
   of the HashNode returned will be NULL. */
HashNode *
hash_find(HashTable *t, void *key)
{
  HashNode *p, *curr;

  for (p = t->chain + ((size_t)key % t->size),
         curr = p->next;        /* first link in chain has no data */
       curr != NULL && key != curr->key;
       p = curr, curr = curr->next);

  return p;
}

/* Return the object associated with the given key, or NULL if none */
void *
hash_get(HashTable *table, void *key)
{
  HashNode *p = hash_find(table, key);

  return p->next ? p->next->data : NULL;
}

/* Set key key in a hash table to object data. Return the previous
   data for that key. */
void
hash_set(HashTable *t, void *key, void *data)
{
  HashNode *p = hash_find(t, key);
  HashNode *n = p->next;

  if (n != NULL) {
    if (data != NULL)
      n->data = data;
    else {
      p->next = n->next;
      free(n);
    }
  } else if (data) {
    n = new(HashNode);
    p->next = n;
    n->key = key;
    n->data = data;
  }
}
