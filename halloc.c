#include "hashmap.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define HEAP_SIZE (64 * (1000))   // 64KB
#define CHUNKS_SIZE (64 * (1000)) // 64KB

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

  }
  return NULL;
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
      hashmap_insert(&heap_freed, cur_key + size, cur_size - size);
      return cur_key;
    }
  }

  return NULL;

  // void *free_chunk = best_fit_free_chunk(size);
  //
  // if (free_chunk != NULL) {
  //   hashmap_insert(&heap_allocated, free_chunk, size);
  //   hashmap_remove(&heap_freed, free_chunk);
  //   return free_chunk;
  // }
  //
  // assert(size + alloced <= HEAP_SIZE);
  // void *ptr = (heap + alloced);
  // Heap_Chunk chunk = {
  //     .ptr = ptr,
  //     .size = size,
  // };
  // alloced += size;
  // heap_chunks[chunks++] = chunk;
  // hashmap_insert(&heap_allocated, ptr, size);
  // return ptr;
}

void merge_free_chunks() {
  for (size_t i = 0; i < heap_freed.count; i++) {
  }
}

void hfree(void *ptr) {
  size_t size = hashmap_lookup(&heap_allocated, ptr);
  // if the ptr is found in allocatd chunks we remove it
  if (size == (size_t)-1) {
    return;
  }

  hashmap_remove(&heap_allocated, ptr);
  hashmap_insert(&heap_freed, ptr, size);
}

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

int main() {
  hashmap_init(&heap_allocated, int_hash, int_compare);
  hashmap_init(&heap_freed, int_hash, int_compare);
  hashmap_insert(&heap_freed, heap, HEAP_SIZE);

  size_t size = 10;
  for (size_t t = 0; t < 10; t++) {
    int *arr = (int *)halloc(t * sizeof(int));
    for (size_t i = 0; i < t; i++) {
      arr[i] = i + 1;
    }
    if (t % 2 == 0) {
      hfree(arr);
    }
    printf("t: %zu\n", t);
    printf("heap_allocated: \n");
    dump_hashmap(&heap_allocated);
    printf("heap_freed: \n");
    dump_hashmap(&heap_freed);
    printf("--------------------------------------------------\n");
  }
  return 0;
}
