/* Open hash tables */

#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "except.h"
#include "hash.h"


/* Create a hash table of size size with a hashing function hash and
   comparison function compare; a pointer to the table is returned */
HashTable *
hash_new(size_t size, Hasher hash, HashComparer compare)
{
  HashTable *table = new(HashTable);

  table->chain = exc_calloc(size, sizeof(HashNode));
  table->size = size;
  table->hash = hash;
  table->compare = compare;

  return table;
}

/* Free a hash table */
void
hash_free (HashTable *table)
{
  HashNode *c, *p, *q, *end = table->chain + table->size;

  for (c = table->chain; c < end; c++)
    for (p = c->next; p != NULL; p = q) {
      q = p->next;
      free(p);
    }
  free(table->chain);
  free(table);
}

/* Search a hash table for an object with key key, returning a pointer
   to the previous HashNode. If there is no such key, the next field
   of the HashNode returned will be NULL. */
HashNode *
hash_find(HashTable *table, void *key)
{
  HashNode *prev, *curr;

  for (prev = table->chain + (table->hash(key) % table->size),
         curr = prev->next;    /* first link in chain has no data */
       curr != NULL && !table->compare(key, curr->key);
       prev = curr, curr = curr->next);

  return prev;
}

/* Return the object associated with the given key, or NULL if none */
void *
hash_get(HashTable *table, void *key)
{
  HashNode *prev = hash_find(table, key);

  return prev->next ? prev->next->data : NULL;
}

/* Set key key in a hash table to object data. Return the previous
   data for that key. */
void
hash_set(HashTable *table, void *key, void *data)
{
  HashNode *prev = hash_find(table, key);
  HashNode *node = prev->next;

  if (node != NULL) {
    if (data != NULL)
      node->data = data;
    else {
      prev->next = node->next;
      free(node);
    }
  } else if (data) {
    node = new(HashNode);
    prev->next = node;
    node->key = key;
    node->data = data;
  }
}


/* Hasher function for string keys (adapted from the Red Dragon
   book) */
size_t
hash_strhash(void *str)
{
  char *p, *s = (char *)str;
  unsigned long h = 0, g;

  for (p = s; *p != '\0' && (p - s) < 32; p++) {
    h = (h << 4) + *p;
    if ((g = h & 0xf0000000)) {
      h ^= (g >> 24);
      h ^= g;
    }
  }

  return h;
}

/* HashComparer functions for string keys */
bool hash_strcmp(void *s, void *t)
{
  return strcmp((char *)s, (char *)t) == 0;
}
