/* Hash tables */

#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

/* Parameter type: key in the tables; must be a pointer type */
typedef void *HashKey;

/* Parameter type: objects in the tables; needn't be a pointer type */
typedef void *HashValue;

/* Hash tables are constructed from chains of HashNodes */
typedef struct HashNode HashNode;
struct HashNode {
  HashNode *next;  /* next in chain */
  HashKey key;
  size_t keysize;  /* size of the key object: if 0, key equality test
                      uses value of key field, otherwise it uses
                      keysize bytes at address pointed to by key
                      field*/
  HashValue val;
};

typedef struct HashTable {
  uint8_t lsizenode;  /* log2 of size of hash table */
  HashValue *array;  /* array (array of pointers to values) */
  size_t arraysize;  /* size of array */
  HashNode *node;  /* hash table (array of hash nodes) */
  HashNode *lastfree;  /* this position is free; all positions after
                          it are full (assuming no deletions) */
  size_t arrayitems;  /* number of items in array */
  size_t nodeitems;  /* number of items in hash table */
} HashTable;

HashTable *hash_new(void);
void hash_free(HashTable *t);
HashValue *hash_find(HashTable *t, HashKey key, size_t size);
HashValue *hash_ensure(HashTable *t, HashKey key, size_t size);
inline void hash_set(HashTable *t, const HashKey key, size_t size, HashValue val);
HashValue hash_get(HashTable *t, HashKey key, size_t size);
HashValue hash_remove(HashTable *t, HashKey key, size_t size);
int hash_next(HashTable *t, HashKey *keyp, HashValue *valp, size_t size);

#endif
