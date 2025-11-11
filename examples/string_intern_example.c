#include <stdio.h>
#include <string.h>

#include "intern/intern.h"

#define MAX_VALUE(a, b) ((a) > (b)) ? (a) : (b)

// Define an intern pool for strings (char arrays)
DEFINE_INTERN_POOL(StringInternPool, char)
IMPL_INTERN_POOL(StringInternPool, char)

// Simple string hash and compare
uint32_t str_hash(const char *s, uint32_t size) {
  uint32_t hash = 5381;
  for (uint32_t i = 0; i < size; ++i) hash = ((hash << 5) + hash) + s[i];
  return hash;
}

int str_compare(const char *a, uint32_t a_size, const char *b,
                uint32_t b_size) {
  return memcmp(a, b, MAX_VALUE(a_size, b_size));
}

int main(void) {
  StringInternPool intern_pool;
  StringInternPool_init(&intern_pool, /*threadsafe=*/false, str_hash,
                        str_compare);

  const char *a = "hello";
  const char *b = "hello";

  const char *interned_a =
      StringInternPool_intern(&intern_pool, a, strlen(a) + 1);
  const char *interned_b =
      StringInternPool_intern(&intern_pool, b, strlen(b) + 1);

  printf("Pointers equal? %s\n", (interned_a == interned_b) ? "yes" : "no");

  StringInternPool_finalize(&intern_pool);
  return 0;
}