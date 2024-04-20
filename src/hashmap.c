#include "hashmap.h"
#include <stdio.h>

unsigned int int_hash(void *key) { return (size_t)key % TABLE_SIZE; }

// Example comparison function for integer keys
int int_compare(void *a, void *b) { return a == b; }

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

int hashmap_set(HashMap *map, void *key, size_t new_size) {
  unsigned int slot = map->hash_function(key);
  int current_index = map->heads[slot];

  while (current_index != -1) {
    if (map->compare_function(map->entries[current_index].key, key)) {
      map->entries[current_index].size = new_size; // Update the size
      return 1; // Success: entry found and updated
    }
    current_index = map->entries[current_index].next;
  }
  return 0; // Key not found
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

void dump_hashmap(const HashMap *map) {
  // Iterate over each slot
  for (int i = 0; i < TABLE_SIZE; i++) {
    int current_index = map->heads[i];

    // Traverse the linked list at this slot
    while (current_index != -1) {
      Entry entry = map->entries[current_index];

      // Assuming key is a pointer to an integer for printing purposes
      printf("Key: %p, Size: %zu\n", entry.key, entry.size);

      // Move to the next entry in the chain
      current_index = entry.next;
    }

    // for (size_t i = 0; i < map->count; i++) {
    //   if (map->entries[i].size != 0) {
    //     printf("start: %p, size: %zu\n", map->entries[i].key,
    //            map->entries[i].size);
    //   }
    // }
  }
}

// Function to deep copy a hashmap
void hashmap_copy(const HashMap *src, HashMap *dest) {
  // Initialize the destination hashmap
  hashmap_init(dest, src->hash_function, src->compare_function);

  // Iterate over each possible slot in the table
  for (int i = 0; i < TABLE_SIZE; i++) {
    int src_index = src->heads[i];
    int prev_dest_index = -1;

    while (src_index != -1) {
      // Create a copy of the entry
      void *key_copy =
          src->entries[src_index].key; // Direct copy of the pointer, modify
                                       // if deep copy of key is needed
      size_t size_copy = src->entries[src_index].size;

      // Insert the copied entry into the destination hashmap
      int new_index = hashmap_insert(dest, key_copy, size_copy);
      if (new_index == -1) {
        fprintf(stderr,
                "Failed to insert in hashmap_deep_copy. Possible overflow.\n");
        return;
      }

      // Manually link the entries since hashmap_insert doesn't manage
      // chains
      if (prev_dest_index != -1) {
        dest->entries[prev_dest_index].next = new_index;
      } else {
        dest->heads[i] = new_index;
      }

      prev_dest_index = new_index;
      src_index = src->entries[src_index].next;
    }
  }
}
