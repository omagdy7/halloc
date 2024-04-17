#include "hashmap.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define HEAP_SIZE (64 * (1000))   // 64KB
#define CHUNKS_SIZE (64 * (1000)) // 64KB
//

#define NOT_FOUND (size_t) - 1

typedef struct {
  void *ptr;
  size_t size;
} Heap_Chunk;

char heap[HEAP_SIZE];
Heap_Chunk heap_chunks[CHUNKS_SIZE];

HashMap heap_allocated;
HashMap heap_freed;

uint64_t alloced = 0;
uint64_t chunks = 0;

void dump_chunk(Heap_Chunk chunk) {
  printf("Allocatd %p with size %zu bytes\n", chunk.ptr, chunk.size);
}

void dump_chunks() {
  for (size_t i = 0; i < chunks; i++) {
    dump_chunk(heap_chunks[i]);
  }
}

void dump_arr(int *arr, size_t sz) {
  printf("{ ");
  for (size_t i = 0; i < sz; i++) {
    printf("%d ", arr[i]);
  }
  printf("}\n");
}

void *halloc(size_t size) {
  if (size == 0)
    return NULL;

  for (size_t i = 0; i < heap_freed.count; i++) {
    size_t cur_size = heap_freed.entries[i].size;
    if (cur_size >= size) {
      void *cur_key = heap_freed.entries[i].key;
      // printf("cur_size: %zu, size: %zu\n", cur_size, size);
      hashmap_remove(&heap_freed, cur_key);
      hashmap_insert(&heap_allocated, cur_key, size);
      printf("Allocating %p with size %zu\n", cur_key, size);
      hashmap_insert(&heap_freed, cur_key + size, cur_size - size);
      return cur_key;
    }
  }

  return NULL;
}

void merge_free_chunks() {
  for (size_t i = 0; i < heap_freed.count; i++) {
  }
}

void hfree(void *ptr) {
  size_t size = hashmap_lookup(&heap_allocated, ptr);
  if (size == NOT_FOUND) {
    return;
  }

  // if the ptr is found in allocatd chunks we remove it and add it to freed
  // chunks
  printf("deallocating %p with size %zu\n", ptr, size);
  hashmap_remove(&heap_allocated, ptr);
  // if the ptr already exists update its size
  if (hashmap_lookup(&heap_freed, ptr) != NOT_FOUND) {
    hashmap_set(&heap_freed, ptr, size);
  } else {
    hashmap_insert(&heap_freed, ptr, size);
  }
}

int main() {
  hashmap_init(&heap_allocated, int_hash, int_compare);
  hashmap_init(&heap_freed, int_hash, int_compare);
  hashmap_insert(&heap_freed, heap, HEAP_SIZE);

  size_t size = 8;
  for (size_t t = 0; t < 5; t++) {
    int *arr = (int *)halloc(size * sizeof(int));
    for (size_t i = 0; i < t; i++) {
      arr[i] = i + 1;
    }
    printf("t: %zu\n", t);
    printf("heap_allocated: \n");
    dump_hashmap(&heap_allocated);
    // if (t % 2 == 0) {
    // }
    printf("heap_freed: \n");
    dump_hashmap(&heap_freed);
    hfree(arr);
    printf("--------------------------------------------------\n");
  }
  int *arr = (int *)halloc(size * 2 * sizeof(int));
  printf("heap_allocated: \n");
  dump_hashmap(&heap_allocated);
  printf("heap_freed: \n");
  dump_hashmap(&heap_freed);
  return 0;
}
