/* Hash tables */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include <stdio.h>

#include "memory.h"
#include "except.h"
#include "hash.h"


/******************************************************************************
* Copyright (C) 1994-2003 Tecgraf, PUC-Rio.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

/* Default definitions for memory macros */
#ifndef mem_new
#define mem_realloc(p, oldn, newn) \
  realloc(p, newn)
#define mem_malloc(n) \
  mem_realloc(NULL, 0, (n))
#define mem_free(b, s) \
  mem_realloc((b), (s), 0)
#define mem_new(t) \
  (t *)mem_malloc(sizeof(t))
#define mem_delete(b) \
  mem_free((b), sizeof(*(b)))
#define mem_cnew(n,t) \
  (t *)mem_malloc((size_t)(n * sizeof(t)))
#define mem_cdelete(b, n, t) \
  mem_realloc((b), (size_t)(n * sizeof(t)), 0)
#define mem_crealloc(v, oldn, n, t) \
  ((v)=(t *)mem_realloc(v,(size_t)(oldn * sizeof(t)), (size_t)(n * sizeof(t))))
#endif



/* The hash tables keep their elements in two parts: an array part and
   a hash part. Non-negative integer keys are candidates to be kept in
   the array part. The actual size of the array is the largest `n'
   such that at least half the slots between 0 and n are in use. The
   hash part is a closed hash table using a mix of chained scatter
   table with Brent's variation. Like open hash tables, collisions
   occur only when two elements have the same hash value. Because of
   that, good performance is still achieved with high loads (up to
   100%).
*/

/* Size of hash pointer array */
#define sizenode(t)	(size_t)(1 << ((t)->lsizenode))

/* max size of array part is 2^MAXBITS */
enum {
  MAXBITS = sizeof(size_t) * CHAR_BIT - 1
};

/* check whether `x' < 2^MAXBITS */
#define toobig(x)	((((x) - 1) >> MAXBITS) != 0)


/* for some types, it is better to avoid modulus by power of 2, as
   they tend to have many 2 factors.
*/
#define hashmod(t, n)	(&t->node[((n) % ((sizenode(t) - 1) | 1))])


/* returns the hash value of an element in a table */
/* This is currently wrong: needs to look at data! */
#define hash(t, key, keysize)    ((void)keysize, hashmod((t), (size_t)(key)))


/* returns the index for `key' if `key' is an appropriate key to live
   in the array part of the table, -1 otherwise.
*/
static size_t
arrayindex(const HashKey *key)
{
  if ((size_t)key >= 1 && !toobig((size_t)key))
    return (size_t)key;
  return 0;  /* `key' did not match some condition */
}



/* Rehash */

static void
computesizes(size_t nums[], size_t ntotal, size_t *narray, size_t *nhash)
{
  size_t i;
  size_t a = nums[0];  /* number of elements smaller than 2^i */
  size_t na = a;  /* number of elements to go to array */
  size_t n = (na == 0) ? MAXBITS + 1 : 0;  /* (log of) best array size */
  for (i = 1; a < *narray && *narray >= (size_t)(1 << (i - 1)); i++) {
    if (nums[i] > 0) {
      a += nums[i];
      if (a >= (size_t)(1 << (i - 1))) {  /* more than half elements in use? */
        n = i;
        na = a;
      }
    }
  }
  assert(na <= *narray && *narray <= ntotal);
  *nhash = ntotal - na;
  *narray = (n == MAXBITS + 1) ? 0 : 1 << n;
  assert(na <= *narray && na >= *narray / 2);
}


static size_t
log2(size_t x)
{
  static const uint8_t log_8[255] = {
    0,
    1,1,
    2,2,2,2,
    3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
  };
  if (x >= 0x00010000) {
    if (x >= 0x01000000)
      return log_8[((x>>24) & 0xff) - 1] + 24;
    else
      return log_8[((x>>16) & 0xff) - 1] + 16;
  }
  else {
    if (x >= 0x00000100)
      return log_8[((x>>8) & 0xff) - 1] + 8;
    else if (x)
      return log_8[(x & 0xff) - 1];
    else
      return MAXBITS + 1;  /* special log for 0 */
  }
}

static void
numuse(const HashTable *t, size_t *narray, size_t *nhash)
{
  size_t nums[MAXBITS + 2];
  size_t i, lg;
  size_t totaluse = 0;  /* only used for consistency checks */
  /* count elements in array part */
  for (i = 0, lg = 0; lg <= MAXBITS; lg++) {  /* for each slice [2^(lg-1) to 2^lg) */
    size_t ttlg = 1 << lg;
    if (ttlg > t->arraysize) {
      ttlg = t->arraysize;
      if (i >= ttlg)
        break;
    }
    nums[lg] = 0;
    for (; i<ttlg; i++) {
      if (t->array[i] != NULL) {
        nums[lg]++;
        totaluse++;
      }
    }
  }
  for (; lg <= MAXBITS; lg++)  /* reset other counts */
    nums[lg] = 0;
  *narray = totaluse;  /* all previous uses were in array part */
  /* count elements in hash part */
  i = sizenode(t);
  while (i--) {
    HashNode *n = &t->node[i];
    if (n->val != NULL) {
      int k = arrayindex(n->key);
      if (k != 0) {  /* is `key' an appropriate array index? */
        nums[log2(k - 1) + 1]++;  /* count as such */
        (*narray)++;
      }
      totaluse++;
    }
  }
  assert(t->nodeitems + t->arrayitems == totaluse);
  assert(t->arrayitems <= *narray);
  computesizes(nums, totaluse, narray, nhash);
}


static void
setarrayvector(HashTable *t, int size)
{
  int i;
  mem_crealloc(t->array, t->sizearray, size, HashValue);
  for (i = t->arraysize; i < size; i++)
     t->array[i] = NULL;
  t->arraysize = size;
}


static void
setnodevector(HashTable *t, int lsize)
{
  int size;
  if (lsize > MAXBITS)
    throw("table too large");
  if (lsize == 0)
    lsize = 1;
  size = 1 << lsize;
  t->node = mem_cnew(size, HashNode);
  memset(t->node, 0, size * sizeof(HashNode));
  t->lsizenode = (uint8_t)lsize;
  t->lastfree = &t->node[size - 1];  /* next free position to be used */
}


static void
resize(HashTable *t, int nasize, int nhsize)
{
  int i;
  int oldasize = t->arraysize;
  int oldhsize = t->lsizenode;
  HashNode *nold;
  nold = t->node;  /* save old hash ... */
  t->arrayitems = t->nodeitems = 0;  /* Zero counts before reinsertion */
  if (nasize > oldasize)  /* array part must grow? */
    setarrayvector(t, nasize);
  /* create new hash part with appropriate size */
  setnodevector(t, nhsize);
  /* re-insert elements */
  if (nasize < oldasize) {  /* array part must shrink? */
    t->arraysize = nasize;
    /* re-insert elements from vanishing slice */
    for (i = nasize; i < oldasize; i++) {
      if (t->array[i] != NULL)
        hash_set(t, (HashKey *)(i + 1), 0, &t->array[i]);
    }
    /* shrink array */
    mem_crealloc(t->array, oldasize, nasize, HashKey);
  }
  /* re-insert elements in hash part */
  for (i = (1 << oldhsize) - 1; i >= 0; i--) {
    HashNode *old = nold + i;
    if (old->val != NULL)
      hash_set(t, old->key, old->keysize, old->val);
  }
  mem_cdelete(nold, 1 << oldhsize, HashNode);  /* free old array */
}


static void
rehash(HashTable *t)
{
  size_t nasize, nhsize;
  numuse(t, &nasize, &nhsize);  /* compute new sizes for array and hash parts */
  resize(t, nasize, log2(nhsize) + 1);
}



/* Get and set */

/* inserts a new key into a hash table; first, check whether key's
   main position is free. If not, check whether colliding node is in
   its main position: if it is, new key goes to an empty position,
   otherwise move colliding node to an empty place and put new key in
   its main position.
*/
static HashValue *
newkey(HashTable *t, HashKey *key, size_t keysize)
{
  HashKey *val;
  HashNode *mp = hash(t, key, keysize);
  if (mp->val != NULL) {  /* main position is not free? */
    HashNode *othern = hash(t, mp->key, mp->keysize);  /* `mp' of colliding node */
    HashNode *n = t->lastfree;  /* get a free place */
    if (othern != mp) {  /* is colliding node out of its main position? */
      /* yes; move colliding node into free position */
      while (othern->next != mp)  /* find previous */
        othern = othern->next;
      othern->next = n;  /* redo the chain with `n' in place of `mp' */
      *n = *mp;  /* copy colliding node into free pos. (mp->next also goes) */
      mp->next = NULL;  /* now `mp' is free */
      mp->val = NULL;
    } else {  /* colliding node is in its own main position */
      /* new node will go into free position */
      n->next = mp->next;  /* chain new position */
      mp->next = n;
      mp = n;
    }
  }
  mp->key = (HashKey *)key;  /* write barrier */
  assert(mp->val == NULL);
  for (;;) {  /* correct lastfree */
    if (t->lastfree->val == NULL && mp != t->lastfree)
      return &mp->val;  /* OK; table still has a free place */
    else if (t->lastfree == t->node)
      break;  /* cannot decrement from here */
    else
      (t->lastfree)--;
  }
  /* no more free places; must create one */
  mp->val = (HashKey)1;  /* avoid new key being removed */
  rehash(t);  /* grow table */
  val = hash_find(t, key, 0);  /* get new position */
  assert(*val == (HashKey)1);
  *val = NULL;
  return val;
}


/* generic search function */
static const HashValue *
getblk(HashTable *t, const HashKey *key, size_t keysize)
{
  assert(keysize > 0);
  if (key == NULL)
    return NULL;
  else {
    HashNode *n = hash(t, key, keysize);
    do {  /* check whether `key' is somewhere in the chain */
      if (keysize == n->keysize && memcmp(n->key, key, keysize) == 0)
        return &n->val;  /* that's it */
      n = n->next;
    } while (n);
    return NULL;
  }
}


/* search function for integers */
static const HashValue *
getptr(HashTable *t, size_t key)
{
  if (1 <= key && key <= t->arraysize)
    return &t->array[key - 1];
  else {
    size_t nk = (size_t)key;
    HashNode *n = hashmod(t, nk);
    do {  /* check whether `key' is somewhere in the chain */
      if (n->keysize == 0 && (size_t)n->key == nk)
        return &n->val;  /* that's it */
      else n = n->next;
    } while (n);
    return NULL;
  }
}


/* remove a node */
static void remnode(HashTable *t, HashNode *p) {
  HashNode *n = hash(t, (size_t)p->key, p->keysize);
  HashNode *prev = NULL;
  while (n != (HashNode *)p && n) {  /* Find the previous node in the chain, if any */
    n = n->next;
    prev = n;
  }
  if (prev)
    prev->next = n->next;  /* If there was a previous node, change its next link */
  p->keysize = 0;
  p->key = NULL;
  p->val = NULL;
  p->next = NULL;
  t->nodeitems--;
  if (t->nodeitems < (size_t)(1 << (t->lsizenode - 2)))
    rehash(t);
}


/* API functions */

HashTable *
hash_new(void)
{
  HashTable *t = mem_new(HashTable);
  t->arrayitems = 0;
  t->nodeitems = 0;
  /* temporary values (kept only if some malloc fails) */
  t->array = NULL;
  t->arraysize = 0;
  t->lsizenode = 0;
  t->node = NULL;
  setarrayvector(t, 0);
  setnodevector(t, 0);
  return t;
}

void
hash_free(HashTable *t)
{
  mem_cdelete(t->node, sizenode(t), HashNode);
  mem_cdelete(t->array, t->sizearray, TObject);
  mem_delete(t);
}

HashValue *
hash_find(HashTable *t, const HashKey key, size_t size)
{
  if (size == 0)
    return (HashValue *)getptr(t, (size_t)key);
  else
    return (HashValue *)getblk(t, key, size);
}

void
hash_set(HashTable *t, const HashKey key, size_t size, HashValue val)
{
  assert(val);
  *hash_ensure(t, key, size) = val;
}

HashValue
hash_get(HashTable *t, HashKey key, size_t size)
{
  return *hash_find(t, key, size);
}

HashValue *
hash_ensure(HashTable *t, HashKey key, size_t keysize)
{
  const HashValue *p = hash_find(t, key, keysize);
  if (p != NULL) {
    if (arrayindex(key) != 0 && (HashValue *)p == NULL)
      t->arrayitems++;
    return (HashValue *)p;
  } else {
    t->nodeitems++;
    return newkey(t, key, keysize);
  }
}

HashKey
hash_remove(HashTable *t, HashKey key, size_t keysize)
{
  HashValue *p = hash_find(t, key, keysize);
  if (p) {
    HashKey o = *p;
    if (arrayindex(key) != 0) {
      *p = NULL;
      t->arrayitems--;
      if (t->arrayitems < t->arraysize / 4)
        rehash(t);
    } else
      remnode(t, hashmod(t, (size_t)key));
    return o;
  } else
    return NULL;  /* If object not found, return NULL */
}

int
hash_next(HashTable *t, HashKey *keyp, HashValue *valp, size_t keysize)
{
  const HashKey *v = hash_find(t, *keyp, keysize);  /* find original element */
  size_t i;
  if ((i = arrayindex(*keyp)) != 0) {
    for (; i < t->arraysize; i++) {  /* try first array part */
      if (t->array[i] != NULL) {  /* a non-NULL value? */
        *keyp = (HashKey *)i;
        *valp = &t->array[i];
        return 1;
      }
    }
  }
  i = ((uint8_t *)v - (uint8_t *)&(&t->node[0])->val) / sizeof(HashNode);
  for (i -= t->arraysize; i < sizenode(t); i++) {  /* then hash part */
    if (t->node[i].val != NULL) {  /* a non-NULL value? */
      *keyp = &(t->node[i].key);
      *valp = &(t->node[i].val);
      return 1;
    }
  }
  return 0;  /* no more elements */
}
