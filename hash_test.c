/* Test hash tables */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "except.h"
#include "hash.h"

char *progname = "hash_test";

int main(void)
{
  HashTable *t = hash_new(2, hash_strhash, hash_strcmp);

  /* Set a hash item and check it can be read back */
  hash_set(t, "a", "foo");
  assert(strcmp(hash_get(t, "a"), "foo") == 0);

  /* Set the item to something else and check it no longer exists */
  hash_set(t, "a", NULL);
  assert(hash_get(t, "a") == NULL);

  /* Add lots of strings to the hash table, forcing at least one chain */
  hash_set(t, "abc", "foo");
  hash_set(t, "xyz", "bar");
  hash_set(t, "def", "baz");

  /* Delete the strings, and one or two that aren't in the table */
  hash_set(t, "ghi", NULL);
  hash_set(t, "abc", NULL);
  hash_set(t, "xyz", NULL);
  hash_set(t, "def", NULL);

  /* Free the hash table */
  hash_free(t);

  warn("succeeded");

  return 0;
}
