/* Open hash tables */

#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdbool.h>

/* Hash tables are constructed from chains of HashNodes, which contain
   a link to the next node, and pointers to the key and data
   objects. */
typedef struct HashNode HashNode;
struct HashNode {
  HashNode *next;
  void *key;
  void *data;
};

/* A hash table consists of a pointer to an array of HashNodes and the
   size of the array. */
typedef struct {
  size_t size;
  HashNode *chain;
} HashTable;

HashTable *hash_new(size_t size);
void hash_free(HashTable *table);
HashNode *hash_find(HashTable *table, void *key);
void *hash_get(HashTable *table, void *key);
void hash_set(HashTable *table, void *key, void *body);

#endif
