#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 32000
#define MAX_ENTRIES 32000

typedef struct {
  void *key;   // Key is a void pointer
  size_t size; // Value is a size_t
  int next;    // Index of the next entry in the chain
} Entry;

typedef struct {
  Entry entries[MAX_ENTRIES];
  int heads[TABLE_SIZE];
  size_t count;
  unsigned int (*hash_function)(void *);
  int (*compare_function)(void *, void *);
} HashMap;

unsigned int int_hash(void *key);
int int_compare(void *a, void *b);
void hashmap_init(HashMap *map, unsigned int (*hash_fn)(void *),
                  int (*comp_fn)(void *, void *));
int hashmap_insert(HashMap *map, void *key, size_t value);
size_t hashmap_lookup(const HashMap *map, void *key);
int hashmap_set(HashMap *map, void *key, size_t new_size);
int hashmap_remove(HashMap *map, void *key);
void dump_hashmap(HashMap *map);
