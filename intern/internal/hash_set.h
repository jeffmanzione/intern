#ifndef COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_HASH_SET_H_
#define COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_HASH_SET_H_

#include <stdbool.h>
#include <stdint.h>

// A decent small prime number to use as the starting size for the hashtable
#define DEFAULT_TABLE_SIZE 31

// Find the table position of a hash value.
#define LOOKUP_HASH_POSITION(hval, num_probes, table_sz) \
  (((hval) + ((num_probes) * (num_probes))) % (table_sz))

// Calculates a new reasonable size of the hash table given a current size.
#define CALCULATE_NEW_TABLE_SIZE(current_size) (((current_size)*2) + 1)

// Calculates the threshold for number of entries in the given hash table size
// before it efficieny starts to diminish and the table should be
// resized/rehashed.
#define CALCULATE_RESIZE_THRESHOLD(table_size) ((int)((table_size) / 2.f))

// Expands to the header definitions for a hash set with the given name and
// value type.
//
// Generates the following for name=CatHashSet and value_type=Cat:
//
//   typedef struct {} CatHashSet;
//   typedef uint32_t (*CatHashSetHashFn)(const Cat, uint32_t size);
//   typedef int32_t (*CatHashSetCompareFn)(const Cat, const Cat);
//   void CatHashSet_init(CatHashSet*, uint32_t start_size, CatHashSetHashFn,
//                        CatHashSetCompareFn);
//   CatHashSet* CatHashSet_create(uint32_t start_size, CatHashSetHashFn,
//                                 CatHashSetCompareFn);
//   void CatHashSet_finalize(CatHashSet*);
//   void CatHashSet_delete(CatHashSet*);
//   bool CatHashSet_insert(CatHashSet*, const Cat, uint32_t);
//   Cat CatHashSet_remove(CatHashSet*, const Cat, uint32_t);
//   bool CatHashSet_contains(CatHashSet*, const Cat, uint32_t);
//   uint32_t CatHashSet_size(CatHashSet*);
#define DEFINE_HASH_SET(name, value_type)                                  \
                                                                           \
  typedef uint32_t (*name##HashFn)(const value_type, uint32_t size);       \
  typedef int32_t (*name##CompareFn)(const value_type, uint32_t,           \
                                     const value_type, uint32_t);          \
                                                                           \
  typedef struct name##Entry_ name##Entry;                                 \
                                                                           \
  typedef struct {                                                         \
    name##HashFn hash;                                                     \
    name##CompareFn compare;                                               \
    uint32_t table_size, num_entries, resize_threshold;                    \
    name##Entry *table, *first, *last;                                     \
  } name;                                                                  \
                                                                           \
  void name##_init(name *hash_set, uint32_t start_size, name##HashFn,      \
                   name##CompareFn);                                       \
                                                                           \
  name *name##_create(uint32_t size, name##HashFn, name##CompareFn);       \
                                                                           \
  void name##_finalize(name *);                                            \
                                                                           \
  void name##_delete(name *);                                              \
                                                                           \
  bool name##_insert(name *, const value_type value, uint32_t value_size); \
                                                                           \
  bool name##_remove(name *, const value_type value, uint32_t value_size); \
                                                                           \
  bool name##_contains(const name *hash_set, const value_type value,       \
                       uint32_t value_size);                               \
                                                                           \
  value_type name##_find(const name *hash_set, const value_type value,     \
                         uint32_t value_size, value_type default_value);   \
                                                                           \
  uint32_t name##_size(const name *)

// Expands to the impleemtation for a hash set with the given name and value
// type.
//
// Generates the following for name=CatHashSet and value_type=Cat:
//
//   void CatHashSet_init(CatHashSet*, uint32_t start_size, CatHashSetHashFn,
//                        CatHashSetCompareFn) { ... }
//   CatHashSet* CatHashSet_create(uint32_t start_size, CatHashSetHashFn,
//                                 CatHashSetCompareFn) { ... }
//   void CatHashSet_finalize(CatHashSet*) { ... }
//   void CatHashSet_delete(CatHashSet*) { ... }
//   bool CatHashSet_insert(CatHashSet*, const Cat, uint32_t value_size) { ... }
//   bool CatHashSet_remove(CatHashSet*, const Cat, uint32_t value_size) { ... }
//   bool CatHashSet_contains(CatHashSet*, const Cat, uint32_t) { ... }
//   Cat CatHashSet_find(CatHashSet*, const Cat, uint32_t value_size) { ... }
//   uint32_t CatHashSet_size(CatHashSet*) { ... }
#define IMPL_HASH_SET(name, value_type)                                        \
  struct name##Entry_ {                                                        \
    value_type value;                                                          \
    uint32_t value_size;                                                       \
    uint32_t hash_value;                                                       \
    int32_t num_probes;                                                        \
    name##Entry *prev, *next;                                                  \
  };                                                                           \
  void name##_resize_table(name *set);                                         \
                                                                               \
  name *name##_create(uint32_t start_size, name##HashFn hash,                  \
                      name##CompareFn compare) {                               \
    name *hash_set = (name *)calloc(sizeof(name), 1);                          \
    name##_init(hash_set, start_size, hash, compare);                          \
    return hash_set;                                                           \
  }                                                                            \
                                                                               \
  void name##_init(name *hash_set, uint32_t start_size, name##HashFn hash,     \
                   name##CompareFn compare) {                                  \
    hash_set->hash = hash;                                                     \
    hash_set->compare = compare;                                               \
    hash_set->table_size = start_size;                                         \
    hash_set->resize_threshold = CALCULATE_RESIZE_THRESHOLD(start_size);       \
    hash_set->table = NULL;                                                    \
    hash_set->first = NULL;                                                    \
    hash_set->last = NULL;                                                     \
    hash_set->num_entries = 0;                                                 \
  }                                                                            \
                                                                               \
  void name##_finalize(name *hash_set) {                                       \
    if (NULL == hash_set->table) {                                             \
      return;                                                                  \
    }                                                                          \
    free(hash_set->table);                                                     \
  }                                                                            \
                                                                               \
  void name##_delete(name *hash_set) {                                         \
    name##_finalize(hash_set);                                                 \
    free(hash_set);                                                            \
  }                                                                            \
                                                                               \
  bool name##_insert_helper(                                                   \
      name *hash_set, value_type value, uint32_t value_size, uint32_t hval,    \
      name##Entry *table, uint32_t table_sz, name##Entry **first,              \
      name##Entry **last, bool *too_many_inserts) {                            \
    int num_probes = 0;                                                        \
    int num_previously_used = 0;                                               \
    name##Entry *first_empty = NULL;                                           \
    int num_probes_at_first_empty = -1;                                        \
    while (true) {                                                             \
      int table_index = LOOKUP_HASH_POSITION(hval, num_probes, table_sz);      \
      num_probes++;                                                            \
      name##Entry *entry = table + table_index;                                \
      /* Position is vacant. */                                                \
      if (0 == entry->num_probes) {                                            \
        /* Use the previously empty slot if we don't find our element. */      \
        if (first_empty != NULL) {                                             \
          entry = first_empty;                                                 \
          num_probes = num_probes_at_first_empty;                              \
        }                                                                      \
        /* Take the vacant spot. */                                            \
        entry->value = (value_type)value;                                      \
        entry->value_size = value_size;                                        \
        entry->hash_value = hval;                                              \
        entry->num_probes = num_probes;                                        \
        entry->prev = *last;                                                   \
        entry->next = NULL;                                                    \
        if (NULL != *last) {                                                   \
          entry->prev->next = entry;                                           \
        }                                                                      \
        *last = entry;                                                         \
        if (NULL == *first) {                                                  \
          *first = entry;                                                      \
        }                                                                      \
        return true;                                                           \
      }                                                                        \
      /* Spot is vacant but previously used, mark it so we can use it later.   \
       */                                                                      \
      if (-1 == entry->num_probes) {                                           \
        num_previously_used++;                                                 \
        /* Returns early if there is a severe performance bottleneck so the    \
         * table can be rehashed. */                                           \
        if (num_previously_used > (int)(table_sz / 2)) {                       \
          *too_many_inserts = true;                                            \
          return false;                                                        \
        }                                                                      \
        if (NULL == first_empty) {                                             \
          first_empty = entry;                                                 \
          num_probes_at_first_empty = num_probes;                              \
        }                                                                      \
        continue;                                                              \
      }                                                                        \
      /* Pair is already present in the table, so the mission is accomplished. \
       */                                                                      \
      if (hval == entry->hash_value) {                                         \
        if (0 == hash_set->compare(value, value_size, entry->value,            \
                                   entry->value_size)) {                       \
          entry->value = (value_type)value;                                    \
          return false;                                                        \
        }                                                                      \
      }                                                                        \
      /* Rob this entry if it did fewer probes. */                             \
      if (entry->num_probes < num_probes) {                                    \
        name##Entry tmp_entry = *entry;                                        \
        /* Take its spot. */                                                   \
        entry->value = (value_type)value;                                      \
        entry->value_size = value_size;                                        \
        entry->hash_value = hval;                                              \
        entry->num_probes = num_probes;                                        \
        /* It is the new insertion. */                                         \
        value = tmp_entry.value;                                               \
        value_size = tmp_entry.value_size;                                     \
        hval = tmp_entry.hash_value;                                           \
        num_probes = tmp_entry.num_probes;                                     \
        first_empty = NULL;                                                    \
        num_probes_at_first_empty = -1;                                        \
      }                                                                        \
    }                                                                          \
  }                                                                            \
                                                                               \
  bool name##_insert(name *hash_set, const value_type value,                   \
                     uint32_t value_size) {                                    \
    if (NULL == hash_set->table) {                                             \
      hash_set->table =                                                        \
          (name##Entry *)calloc(sizeof(name##Entry), hash_set->table_size);    \
    } else if (hash_set->num_entries > hash_set->resize_threshold) {           \
      name##_resize_table(hash_set);                                           \
    }                                                                          \
    bool too_many_inserts = false;                                             \
    bool was_inserted = name##_insert_helper(                                  \
        hash_set, (value_type)value, value_size,                               \
        hash_set->hash(value, value_size), hash_set->table,                    \
        hash_set->table_size, &hash_set->first, &hash_set->last,               \
        &too_many_inserts);                                                    \
    /* Maps may have a lot of removed spots. If this causes a performance      \
     * slowdown, then it is better to rehash the map. */                       \
    if (too_many_inserts) {                                                    \
      name##_resize_table(hash_set);                                           \
      too_many_inserts = false;                                                \
      was_inserted = name##_insert_helper(                                     \
          hash_set, (value_type)value, value_size,                             \
          hash_set->hash(value, value_size), hash_set->table,                  \
          hash_set->table_size, &hash_set->first, &hash_set->last,             \
          &too_many_inserts);                                                  \
      if (too_many_inserts) {                                                  \
        /* This should never happen. */                                        \
      }                                                                        \
    }                                                                          \
    if (was_inserted) {                                                        \
      hash_set->num_entries++;                                                 \
    }                                                                          \
    return was_inserted;                                                       \
  }                                                                            \
                                                                               \
  name##Entry *name##_lookup_entry(                                            \
      const name *hash_set, const value_type value, uint32_t value_size,       \
      name##Entry *table, uint32_t table_sz) {                                 \
    uint32_t hval = hash_set->hash(value, value_size);                         \
    int num_probes = 0;                                                        \
    while (true) {                                                             \
      int table_index = LOOKUP_HASH_POSITION(hval, num_probes, table_sz);      \
      ++num_probes;                                                            \
      name##Entry *entry = table + table_index;                                \
      if (0 == entry->num_probes) {                                            \
        /* Found empty. */                                                     \
        return NULL;                                                           \
      }                                                                        \
      if (-1 == entry->num_probes) {                                           \
        /* Found previously used. */                                           \
        continue;                                                              \
      }                                                                        \
      if (hval == entry->hash_value) {                                         \
        if (0 == hash_set->compare(value, value_size, entry->value,            \
                                   entry->value_size)) {                       \
          return entry;                                                        \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }                                                                            \
                                                                               \
  bool name##_remove(name *hash_set, const value_type value,                   \
                     uint32_t value_size) {                                    \
    if (NULL == hash_set->table) {                                             \
      return false;                                                            \
    }                                                                          \
    name##Entry *entry = name##_lookup_entry(                                  \
        hash_set, value, value_size, hash_set->table, hash_set->table_size);   \
    if (NULL == entry) {                                                       \
      return false;                                                            \
    }                                                                          \
    if (hash_set->last == entry) {                                             \
      hash_set->last = entry->prev;                                            \
    } else {                                                                   \
      entry->next->prev = entry->prev;                                         \
    }                                                                          \
    if (hash_set->first == entry) {                                            \
      hash_set->first = entry->next;                                           \
    } else {                                                                   \
      entry->prev->next = entry->next;                                         \
    }                                                                          \
    entry->num_probes = -1;                                                    \
    hash_set->num_entries--;                                                   \
    return true;                                                               \
  }                                                                            \
                                                                               \
  bool name##_contains(const name *hash_set, const value_type value,           \
                       uint32_t value_size) {                                  \
    if (NULL == hash_set->table) {                                             \
      return false;                                                            \
    }                                                                          \
    name##Entry *entry = name##_lookup_entry(                                  \
        hash_set, value, value_size, hash_set->table, hash_set->table_size);   \
    if (NULL == entry) {                                                       \
      return false;                                                            \
    }                                                                          \
    return true;                                                               \
  }                                                                            \
  value_type name##_find(const name *hash_set, const value_type value,         \
                         uint32_t value_size, value_type default_value) {      \
    if (NULL == hash_set->table) {                                             \
      return default_value;                                                    \
    }                                                                          \
    name##Entry *entry = name##_lookup_entry(                                  \
        hash_set, value, value_size, hash_set->table, hash_set->table_size);   \
    if (NULL == entry) {                                                       \
      return default_value;                                                    \
    }                                                                          \
    return entry->value;                                                       \
  }                                                                            \
                                                                               \
  uint32_t name##_size(const name *hash_set) { return hash_set->num_entries; } \
                                                                               \
  void name##_resize_table(name *hash_set) {                                   \
    uint32_t new_table_size = CALCULATE_NEW_TABLE_SIZE(hash_set->table_size);  \
    name##Entry *new_table =                                                   \
        (name##Entry *)calloc(sizeof(name##Entry), new_table_size);            \
    name##Entry *new_first = NULL;                                             \
    name##Entry *new_last = NULL;                                              \
                                                                               \
    for (name##Entry *entry = hash_set->first; entry != NULL;                  \
         entry = entry->next) {                                                \
      bool too_many_inserts = false;                                           \
      name##_insert_helper(hash_set, entry->value, entry->value_size,          \
                           entry->hash_value, new_table, new_table_size,       \
                           &new_first, &new_last, &too_many_inserts);          \
      if (too_many_inserts) {                                                  \
        /* Should never happen */                                              \
      }                                                                        \
    }                                                                          \
                                                                               \
    free(hash_set->table);                                                     \
    hash_set->table = new_table;                                               \
    hash_set->table_size = new_table_size;                                     \
    hash_set->first = new_first;                                               \
    hash_set->last = new_last;                                                 \
    hash_set->resize_threshold = CALCULATE_RESIZE_THRESHOLD(new_table_size);   \
  }

#endif /* COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_HASH_SET_H_ */
