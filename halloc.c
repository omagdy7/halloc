#include "halloc.h"

char heap[HEAP_SIZE];
HashMap heap_allocated;
HashMap heap_freed;

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
  merge_free_chunks();
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
