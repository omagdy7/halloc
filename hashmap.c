#include "hashmap.h"
#include <stdio.h>

unsigned int int_hash(void *key) { return (size_t)key % TABLE_SIZE; }

// Example comparison function for integer keys
int int_compare(void *a, void *b) { return (*(size_t *)a) == (*(size_t *)b); }

void hashmap_init(HashMap *map, unsigned int (*hash_fn)(void *),
                  int (*comp_fn)(void *, void *)) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    map->heads[i] = -1;
  }
  map->count = 0;
  map->hash_function = hash_fn;
  map->compare_function = comp_fn;
}

int hashmap_insert(HashMap *map, void *key, size_t size) {
  if (map->count == MAX_ENTRIES) {
    return 0; // No space left to insert new entries
  }

  unsigned int slot = map->hash_function(key);
  int current_index = map->count++;

  // Store key and size
  map->entries[current_index].key = key;
  map->entries[current_index].size = size;
  map->entries[current_index].next = map->heads[slot];
  map->heads[slot] = current_index;

  return 1;
}

size_t hashmap_lookup(const HashMap *map, void *key) {
  unsigned int slot = map->hash_function(key);
  int current_index = map->heads[slot];

  while (current_index != -1) {
    if (map->compare_function(map->entries[current_index].key, key)) {
      return map->entries[current_index].size;
    }
    current_index = map->entries[current_index].next;
  }
  return (size_t)-1; // Not found
}

int hashmap_remove(HashMap *map, void *key) {
  unsigned int slot = map->hash_function(key);
  int current_index = map->heads[slot];
  int prev_index = -1;

  while (current_index != -1) {
    if (map->compare_function(map->entries[current_index].key, key)) {
      if (prev_index == -1) {
        map->heads[slot] = map->entries[current_index].next;
      } else {
        map->entries[prev_index].next = map->entries[current_index].next;
      }
      map->entries[current_index].key = NULL; // Clear the entry
      map->entries[current_index].size = 0;
      map->entries[current_index].next = -1;
      return 1; // Success
    }
    prev_index = current_index;
    current_index = map->entries[current_index].next;
  }
  return 0; // Not found
}

void dump_hashmap(HashMap *map) {
  for (size_t i = 0; i < map->count; i++) {
    if (map->entries[i].size != 0) {
      printf("start: %p, size: %zu\n", map->entries[i].key,
             map->entries[i].size);
    }
  }
}
