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
      free(p->key);
      free(p->data);
      free(p);
    }
  free(table->chain);
  free(table);
}

/* Search a hash table for an object with key key, returning a pointer
   to the HashNode, or the start of the chain in which it would go if
   there is no such key. */
HashNode *
hash_find(HashTable *table, void *key)
{
  HashNode *start = table->chain + table->hash(key) % table->size;
  HashNode *curr;

  for (curr = start;
       curr != NULL && !table->compare(key, curr->key);
       curr = curr->next);

  return curr != NULL ? curr : start;
}

/* Set key key in a hash table to object data. Return the previous
   data for that key. */
void
hash_set(HashTable *table, void *key, void *data)
{
  HashNode *node = hash_find(table, key);

  if (node->data != NULL) {
    free(node->data);
    if (data != NULL)
      node->data = data;
    else {
      free(node->key);
      free(node);
    }
  } else if (data) {
    HashNode *new = new(HashNode);
    node->next = new;
    new->key = key;
    new->data = data;
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
