/* Test hash tables */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "except.h"
#include "hash.h"


char *progname = "hash_test";

int main(void)
{
  HashTable *t = hash_new();

  /* Key strings; use arrays to prevent string merging by the
     compiler */
  unsigned char s0[] = "a";
  unsigned char s1[] = "abc";
  unsigned char s1a[] = "abc";
  unsigned char s2[] = "def";
  unsigned char s3[] = "ghi";
  unsigned char s4[] = "xyz";


  /* Pointer tests */

  /* Set a hash item and check it can be read back */
  hash_set(t, s0, 0, (unsigned char *)"foo");
  assert(strcmp((char *)hash_get(t, s0, 0), "foo") == 0);

  /* Add lots of items to the hash table, forcing at least one table
     resize */
  hash_set(t, s1, 0, (unsigned char *)"foo");
  hash_set(t, s4, 0, (unsigned char *)"bar");
  hash_set(t, s2, 0, (unsigned char *)"baz");
  hash_set(t, s1a, 0, (unsigned char *)"baz");

  /* Make sure that, because we're using pointers, the item with key
     s1a is distinct from the item with key s1 */
  assert(strcmp((char *)hash_get(t, s1, 0), "foo") == 0);
  assert(strcmp((char *)hash_get(t, s1a, 0), "baz") == 0);

  /* Delete the items, and one that isn't in the table */
  hash_remove(t, s3, 0);
  hash_remove(t, s1, 0);
  hash_remove(t, s1a, 0);
  hash_remove(t, s4, 0);
  hash_remove(t, s2, 0);


  /* Number tests */

  /* Set a hash item and check it can be read back */
  hash_set(t, (HashKey)1, 0, (unsigned char *)"foo");
  assert(strcmp((char *)hash_get(t, (HashKey)1, 0), "foo") == 0);

  /* Add lots of items to the array part, forcing at least one array
     resize */
  hash_set(t, (HashKey)2, 0, (unsigned char *)"foo");
  hash_set(t, (HashKey)3, 0, (unsigned char *)"bar");
  hash_set(t, (HashKey)4, 0, (unsigned char *)"baz");
  hash_set(t, (HashKey)4, 0, (unsigned char *)"biz");
  assert(strcmp((char *)hash_get(t, (HashKey)4, 0), "biz") == 0);
  assert(t->arraysize == 4);

  /* Delete the items, and one that isn't in the table; delete in
     increasing order to force resize which puts some array elements
     into hash part */
  hash_remove(t, (HashKey)1, 0);
  hash_remove(t, (HashKey)2, 0);
  hash_remove(t, (HashKey)3, 0);
  hash_remove(t, (HashKey)4, 0);
  hash_remove(t, (HashKey)5, 0);

  /* Special case for key 0 */
/*   hash_set(t, (HashKey)0, 0, "whiz"); */
/*   assert(strcmp(hash_get(t, (HashKey)0, 0), "whiz") == 0); */


  /* Object tests */

  /* Set a hash item and check it can be read back */
  hash_set(t, s0, 0, (unsigned char *)"foo");
  assert(strcmp((char *)hash_get(t, s0, 0), "foo") == 0);

  /* Add lots of items to the hash table, forcing at least one table
     resize */
  hash_set(t, s1, 0, (unsigned char *)"foo");
  hash_set(t, s4, 0, (unsigned char *)"bar");
  hash_set(t, s2, 0, (unsigned char *)"baz");
  hash_set(t, s1a, 0, (unsigned char *)"baz");

  /* Make sure that, because we're using pointers, the item with key
     s1a is distinct from the item with key s1 */
  assert(strcmp((char *)hash_get(t, s1, 0), "foo") == 0);
  assert(strcmp((char *)hash_get(t, s1a, 0), "baz") == 0);

  /* Delete the items, and one that isn't in the table */
  hash_remove(t, s3, 0);
  hash_remove(t, s1, 0);
  hash_remove(t, s1a, 0);
  hash_remove(t, s4, 0);
  hash_remove(t, s2, 0);


  /* Test hash_next */


  /* Free the hash table */
  hash_free(t);

  warn("succeeded");

  return 0;
}
