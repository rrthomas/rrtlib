/* Test hash tables */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "except.h"
#include "hash.h"


char *progname = "hash_test";

int main(void)
{
  HashTable *t = hash_new(1);
  char *s0 = "a";
  char *s1 = "abc";
  char *s2 = "def";
  char *s3 = "ghi";
  char *s4 = "xyz";

  /* Set a hash item and check it can be read back */
  hash_set(t, s0, "foo");
  assert(strcmp(hash_get(t, s0), "foo") == 0);

  /* Set the item to something else and check it no longer exists */
  hash_set(t, s0, NULL);
  assert(hash_get(t, s0) == NULL);

  /* Add lots of strings to the hash table, forcing at least one chain */
  hash_set(t, s1, "foo");
  hash_set(t, s4, "bar");
  hash_set(t, s2, "baz");

  /* Delete the strings, and one that isn't in the table */
  hash_set(t, s3, NULL);
  hash_set(t, s1, NULL);
  hash_set(t, s4, NULL);
  hash_set(t, s2, NULL);

  /* Free the hash table */
  hash_free(t);

  warn("succeeded");

  return 0;
}
