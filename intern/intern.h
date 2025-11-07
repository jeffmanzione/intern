#ifndef COM_GITHUB_JEFFMANZIONE_INTERN_INTERN_H_
#define COM_GITHUB_JEFFMANZIONE_INTERN_INTERN_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "intern/internal/hash_set.h"
#include "intern/internal/intern_helpers.h"

#define DEFAULT_MAX_VALUES_PER_CHUNK 64

#define MAX_VALUE(a, b) ((a) > (b)) ? (a) : (b)

#define DEFINE_INTERN(name, value_type)                            \
  DEFINE_HASH_SET(name##HashSet, value_type *);                    \
                                                                   \
  typedef struct name##Chunk_ name##Chunk;                         \
                                                                   \
  typedef struct {                                                 \
    char *tail, *end;                                              \
    name##Chunk *chunk, *last;                                     \
    name##HashSet hash_set;                                        \
  } name;                                                          \
                                                                   \
  void name##_init(name *intern, name##HashSetHashFn hash,         \
                   name##HashSetCompareFn compare);                \
  void name##_finalize(name *intern);                              \
  value_type *name##_intern(name *intern, const value_type *value, \
                            uint32_t value_size);

#define IMPL_INTERN(name, value_type)                                         \
  IMPL_HASH_SET(name##HashSet, value_type *);                                 \
                                                                              \
  struct name##Chunk_ {                                                       \
    char *block;                                                              \
    name##Chunk *next;                                                        \
    uint32_t sz;                                                              \
  };                                                                          \
                                                                              \
  name##Chunk *name##Chunk_create(uint32_t chunk_size) {                      \
    name##Chunk *chunk = (name##Chunk *)malloc(sizeof(name##Chunk));          \
    chunk->sz = chunk_size;                                                   \
    chunk->block = (char *)malloc(sizeof(value_type) * chunk->sz);            \
    chunk->next = NULL;                                                       \
    return chunk;                                                             \
  }                                                                           \
                                                                              \
  void name##Chunk_delete(name##Chunk *chunk) {                               \
    if (NULL != chunk->next) {                                                \
      name##Chunk_delete(chunk->next);                                        \
    }                                                                         \
    free(chunk->block);                                                       \
    free(chunk);                                                              \
  }                                                                           \
                                                                              \
  void name##_init(name *intern, name##HashSetHashFn hash,                    \
                   name##HashSetCompareFn compare) {                          \
    intern->chunk = intern->last =                                            \
        name##Chunk_create(DEFAULT_MAX_VALUES_PER_CHUNK);                     \
    intern->tail = intern->chunk->block;                                      \
    intern->end = intern->tail + intern->chunk->sz;                           \
    name##HashSet_init(&intern->hash_set, DEFAULT_TABLE_SIZE, hash, compare); \
  }                                                                           \
                                                                              \
  void name##_finalize(name *intern) {                                        \
    name##HashSet_finalize(&intern->hash_set);                                \
    name##Chunk_delete(intern->chunk);                                        \
  }                                                                           \
                                                                              \
  value_type *name##_intern(name *intern, const value_type *value,            \
                            uint32_t value_size) {                            \
    value_type *value_lookup =                                                \
        name##HashSet_find(&intern->hash_set, value, value_size, NULL);       \
    if (NULL != value_lookup) {                                               \
      return value_lookup;                                                    \
    }                                                                         \
                                                                              \
    if (intern->tail + value_size >= intern->end) {                           \
      intern->last->next = name##Chunk_create(                                \
          MAX_VALUE(DEFAULT_MAX_VALUES_PER_CHUNK * sizeof(value_type),        \
                    compute_nearest_pow2_gte(value_size * 16)));              \
      intern->last = intern->last->next;                                      \
      intern->tail = intern->last->block;                                     \
      intern->end = intern->tail + intern->last->sz;                          \
    }                                                                         \
    value_type *to_return = (value_type *)intern->tail;                       \
    memmove(intern->tail, value, value_size);                                 \
    intern->tail += value_size;                                               \
    name##HashSet_insert(&intern->hash_set, to_return, value_size);           \
    return to_return;                                                         \
  }

#endif /* COM_GITHUB_JEFFMANZIONE_INTERN_INTERN_H_ */
