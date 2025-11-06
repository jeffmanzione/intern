#include "intern/internal/hash_set.h"

#include <stdlib.h>

struct StringInternEntry_ {
  const char *value;
  uint32_t hash_value;
  int32_t num_probes;
  StringInternEntry *prev, *next;
};

#define LOOKUP_HASH_POSITION(hval, num_probes, table_sz) \
  (((hval) + ((num_probes) * (num_probes))) % (table_sz))
#define CALCULATE_NEW_RESIZE_THRESHOLD(current_size) (((current_size)*2) + 1)
#define CALCULATE_RESIZE_THRESHOLD(table_size) ((int)((table_size) / 2.f))

void StringInternHashSet_resize_table(StringInternHashSet *set);

StringInternHashSet *StringInternHashSet_create(
    uint32_t start_size, StringInternHashFn hash,
    StringInternComparatorFn compare, AllocateFn allocate, ReleaseFn release) {
  StringInternHashSet *hash_set =
      (StringInternHashSet *)allocate(sizeof(StringInternHashSet), 1);
  StringInternHashSet_init(hash_set, start_size, hash, compare, allocate,
                           release);
  return hash_set;
}

void StringInternHashSet_init(StringInternHashSet *hash_set,
                              uint32_t start_size, StringInternHashFn hash,
                              StringInternComparatorFn compare,
                              AllocateFn allocate, ReleaseFn release) {
  hash_set->hash = hash;
  hash_set->compare = compare;
  hash_set->allocate = allocate;
  hash_set->release = release;
  hash_set->table_size = start_size;
  hash_set->resize_threshold = CALCULATE_RESIZE_THRESHOLD(start_size);
  hash_set->table = NULL;
  hash_set->first = NULL;
  hash_set->last = NULL;
  hash_set->num_entries = 0;
}

void StringInternHashSet_finalize(StringInternHashSet *hash_set) {
  if (NULL == hash_set->table) {
    return;
  }
  hash_set->release(hash_set->table);
}

void StringInternHashSet_delete(StringInternHashSet *hash_set) {
  StringInternHashSet_finalize(hash_set);
  hash_set->release(hash_set);
}

bool StringInternHashSet_insert_helper(
    StringInternHashSet *hash_set, const char *value, uint32_t hval,
    StringInternEntry *table, uint32_t table_sz, StringInternEntry **first,
    StringInternEntry **last, bool *too_many_inserts) {
  int num_probes = 0;
  int num_previously_used = 0;
  StringInternEntry *first_empty = NULL;
  int num_probes_at_first_empty = -1;
  while (true) {
    int table_index = LOOKUP_HASH_POSITION(hval, num_probes, table_sz);
    num_probes++;
    StringInternEntry *entry = table + table_index;
    // LOOKUP_HASH_POSITIONition is vacant.
    if (0 == entry->num_probes) {
      // Use the previously empty slot if we don't find our element.
      if (first_empty != NULL) {
        entry = first_empty;
        num_probes = num_probes_at_first_empty;
      }
      // Take the vacant spot.
      entry->value = value;
      entry->hash_value = hval;
      entry->num_probes = num_probes;
      entry->prev = *last;
      entry->next = NULL;
      if (NULL != *last) {
        entry->prev->next = entry;
      }
      *last = entry;
      if (NULL == *first) {
        *first = entry;
      }
      return true;
    }
    // Spot is vacant but previously used, mark it so we can use it later.
    if (-1 == entry->num_probes) {
      num_previously_used++;
      // Returns early if there is a severe performance bottleneck so the table
      // can be rehashed.
      if (num_previously_used > (table_sz / 2)) {
        *too_many_inserts = true;
        return false;
      }
      if (NULL == first_empty) {
        first_empty = entry;
        num_probes_at_first_empty = num_probes;
      }
      continue;
    }
    // Pair is already present in the table, so the mission is accomplished.
    if (hval == entry->hash_value) {
      if (0 == hash_set->compare(value, entry->value)) {
        entry->value = (void *)value;
        return false;
      }
    }
    // Rob this entry if it did fewer probes.
    if (entry->num_probes < num_probes) {
      StringInternEntry tmp_entry = *entry;
      // Take its spot.
      entry->value = value;
      entry->hash_value = hval;
      entry->num_probes = num_probes;
      // It is the new insertion.
      value = tmp_entry.value;
      hval = tmp_entry.hash_value;
      num_probes = tmp_entry.num_probes;
      first_empty = NULL;
      num_probes_at_first_empty = -1;
    }
  }
}

bool StringInternHashSet_insert(StringInternHashSet *hash_set,
                                const char *value) {
  if (NULL == hash_set->table) {
    hash_set->table =
        hash_set->allocate(sizeof(StringInternEntry), hash_set->table_size);
  } else if (hash_set->num_entries > hash_set->resize_threshold) {
    StringInternHashSet_resize_table(hash_set);
  }
  bool too_many_inserts = false;
  bool was_inserted = StringInternHashSet_insert_helper(
      hash_set, value, hash_set->hash(value), hash_set->table,
      hash_set->table_size, &hash_set->first, &hash_set->last,
      &too_many_inserts);
  // Maps may have a lot of removed spots. If this causes a performance
  // slowdown, then it is better to rehash the map.
  if (too_many_inserts) {
    StringInternHashSet_resize_table(hash_set);
    too_many_inserts = false;
    was_inserted = StringInternHashSet_insert_helper(
        hash_set, value, hash_set->hash(value), hash_set->table,
        hash_set->table_size, &hash_set->first, &hash_set->last,
        &too_many_inserts);
    if (too_many_inserts) {
      // This should never happen.
    }
  }
  if (was_inserted) {
    hash_set->num_entries++;
  }
  return was_inserted;
}

StringInternEntry *StringInternHashSet_lookup_entry(
    const StringInternHashSet *hash_set, const void *value,
    StringInternEntry *table, uint32_t table_sz) {
  uint32_t hval = hash_set->hash(value);
  int num_probes = 0;
  while (true) {
    int table_index = LOOKUP_HASH_POSITION(hval, num_probes, table_sz);
    ++num_probes;
    StringInternEntry *entry = table + table_index;
    if (0 == entry->num_probes) {
      // Found empty.
      return NULL;
    }
    if (-1 == entry->num_probes) {
      // Found previously used.
      continue;
    }
    if (hval == entry->hash_value) {
      if (0 == hash_set->compare(value, entry->value)) {
        return entry;
      }
    }
  }
}

char *StringInternHashSet_remove(StringInternHashSet *hash_set,
                                 const char *value) {
  if (NULL == hash_set->table) {
    return NULL;
  }
  StringInternEntry *entry = StringInternHashSet_lookup_entry(
      hash_set, value, hash_set->table, hash_set->table_size);
  if (NULL == entry) {
    return NULL;
  }
  if (hash_set->last == entry) {
    hash_set->last = entry->prev;
  } else {
    entry->next->prev = entry->prev;
  }
  if (hash_set->first == entry) {
    hash_set->first = entry->next;
  } else {
    entry->prev->next = entry->next;
  }
  entry->num_probes = -1;
  hash_set->num_entries--;
  return (char *)entry->value;
}

bool StringInternHashSet_contains(const StringInternHashSet *hash_set,
                                  const char *value) {
  if (NULL == hash_set->table) {
    return false;
  }
  StringInternEntry *entry = StringInternHashSet_lookup_entry(
      hash_set, value, hash_set->table, hash_set->table_size);
  if (NULL == entry) {
    return false;
  }
  return true;
}

uint32_t StringInternHashSet_size(const StringInternHashSet *hash_set) {
  return hash_set->num_entries;
}

void StringInternHashSet_resize_table(StringInternHashSet *hash_set) {
  uint32_t new_table_size =
      CALCULATE_NEW_RESIZE_THRESHOLD(hash_set->table_size);
  StringInternEntry *new_table =
      hash_set->allocate(sizeof(StringInternEntry), new_table_size);
  StringInternEntry *new_first = NULL;
  StringInternEntry *new_last = NULL;

  for (StringInternEntry *entry = hash_set->first; entry != NULL;
       entry = entry->next) {
    bool too_many_inserts = false;
    StringInternHashSet_insert_helper(hash_set, entry->value, entry->hash_value,
                                      new_table, new_table_size, &new_first,
                                      &new_last, &too_many_inserts);
    if (too_many_inserts) {
      // Should never happen
    }
  }

  hash_set->release(hash_set->table);
  hash_set->table = new_table;
  hash_set->table_size = new_table_size;
  hash_set->first = new_first;
  hash_set->last = new_last;
  hash_set->resize_threshold = CALCULATE_RESIZE_THRESHOLD(new_table_size);
}
