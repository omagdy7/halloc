#include "hashmap.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define HEAP_SIZE (64 * (1000))   // 64KB
#define CHUNKS_SIZE (64 * (1000)) // 64KB
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
    if (cur_size > size) {
      void *cur_key = heap_freed.entries[i].key;
      hashmap_remove(&heap_freed, cur_key);
      hashmap_insert(&heap_allocated, cur_key, size);
      hashmap_insert(&heap_freed, cur_key + size, cur_size - size);
      return cur_key;
    }
  }

  return NULL;
}

void *min(void *a, void *b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

void merge_free_chunks() {
  bool segments_found = true;
  while (segments_found) {
    for (size_t i = 0; i < heap_freed.count; i++) {
      void *segments[10] = {0};
      size_t cnt = 0;
      if (heap_freed.entries[i].size != 0) {
        char *cur_ptr = heap_freed.entries[i].key;
        void *min_address = cur_ptr;
        size_t total_size = 0;
        size_t next = hashmap_lookup(&heap_freed, cur_ptr);
        while (next != NOT_FOUND && next != 0) {
          segments[cnt++] = cur_ptr;
          cur_ptr = cur_ptr + next;
          min_address = min(min_address, cur_ptr);
          total_size += next;
          if (cur_ptr <= heap + HEAP_SIZE) {
            next = hashmap_lookup(&heap_freed, cur_ptr);
          }
        }
        if (cnt == 1) {
          segments_found = false;
        }
        for (size_t s = 0; s < cnt; s++) {
          if (segments[s] != min_address) {
            hashmap_remove(&heap_freed, segments[s]);
          }
        }
        hashmap_set(&heap_freed, min_address, total_size);
      }
    }
  }
}

void hfree(void *ptr) {
  size_t size = hashmap_lookup(&heap_allocated, ptr);
  if (size == NOT_FOUND) {
    return;
  }

  // if the ptr is found in allocatd chunks we remove it and add it to freed chunks
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
  for (size_t t = 0; t < 3; t++) {
    int *arr = (int *)halloc(size * sizeof(int));
    for (size_t i = 0; i < t; i++) {
      arr[i] = i + 1;
    }
    printf("t: %zu\n", t);
    printf("heap_allocated: \n");
    dump_hashmap(&heap_allocated);
    printf("heap_freed: \n");
    dump_hashmap(&heap_freed);
    if (t % 2 == 0) {
      hfree(arr);
    }

    printf("--------------------------------------------------\n");
  }
  // int *arr = (int *)halloc(size * 2 * sizeof(int));
  // printf("heap_allocated: \n");
  // dump_hashmap(&heap_allocated);
  printf("before merging heap_freed: \n");
  dump_hashmap(&heap_freed);
  merge_free_chunks();
  printf("after merging heap_freed: \n");
  dump_hashmap(&heap_freed);
  return 0;
}
