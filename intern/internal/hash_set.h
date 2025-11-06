#ifndef COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_HASH_SET_H_
#define COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_HASH_SET_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t (*StringInternHashFn)(const char *);
typedef uint32_t (*StringInternComparatorFn)(const char *, const char *);
typedef void *(*AllocateFn)(uint32_t elt_size, uint32_t num_elts);
typedef void (*ReleaseFn)(void *ptr);

typedef struct StringInternEntry_ StringInternEntry;

typedef struct {
  StringInternHashFn hash;
  StringInternComparatorFn compare;
  AllocateFn allocate;
  ReleaseFn release;
  uint32_t table_size, num_entries, resize_threshold;
  StringInternEntry *table, *first, *last;
} StringInternHashSet;

void StringInternHashSet_init(StringInternHashSet *hash_set, uint32_t size,
                              StringInternHashFn, StringInternComparatorFn,
                              AllocateFn, ReleaseFn);

StringInternHashSet *StringInternHashSe_create(uint32_t size,
                                               StringInternHashFn,
                                               StringInternComparatorFn,
                                               AllocateFn, ReleaseFn);

void StringInternHashSet_finalize(StringInternHashSet *);

void StringInternHashSet_delete(StringInternHashSet *);

bool StringInternHashSet_insert(StringInternHashSet *, const char *value);

char *StringInternHashSet_remove(StringInternHashSet *, const char *value);

bool StringInternHashSet_contains(const StringInternHashSet *hash_set,
                                  const char *value);

uint32_t StringInternHashSet_size(const StringInternHashSet *);

#endif /* COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_HASH_SET_H_ */
