/* Open hash tables */

#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdbool.h>

/* Hash tables are constructed from chains of HashNodes, which contain
   a link to the next node, and pointers to the key and body of the
   object. */
typedef struct HashNode HashNode;
struct HashNode {
  HashNode *next;
  void *key;
  void *data;
};

/* The hashing function type */
typedef size_t (*Hasher)(void *);

/* The comparison function type */
typedef bool (*HashComparer)(void *, void *);

/* A hash table consists of a pointer to an array of HashNodes, the
   size of the array, a hashing function and a comparison function. */
typedef struct {
  HashNode *chain;
  size_t size;
  Hasher hash;
  HashComparer compare;
} HashTable;

HashTable *hash_new(size_t size, Hasher hash, HashComparer compare);
void hash_free(HashTable *table);
HashNode *hash_find(HashTable *table, void *key);
void *hash_get(HashTable *table, void *key);
void hash_set(HashTable *table, void *key, void *body);

size_t hash_strhash(void *);
bool hash_strcmp(void *, void *);

#endif
