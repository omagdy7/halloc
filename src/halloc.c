#include "halloc.h"
#include "assert.h"

// TODO: Implement boundary checks for memory access - Ensure all memory accesses, especially within `halloc`, `hfree`, and `merge_free_chunks`, are within the bounds of the `heap` array to prevent undefined behavior.
// TODO: Improve error handling and robustness - Add checks for null pointers, and verify return values from hash map operations to ensure they are handled correctly and do not lead to crashes or incorrect behavior.
// TODO: Optimize the `merge_free_chunks` function - Simplify and optimize the merging logic to reduce complexity and avoid unnecessary performance overhead. Consider only merging adjacent free blocks to make the process more efficient.
// TODO: Implement alignment handling in `halloc` - Modify the allocation logic to align memory addresses to hardware requirements, which is essential for performance optimizations and compatibility with low-level operations.
// TODO: Refactor the data structures for better performance - Evaluate and potentially replace the hashmaps with more suitable data structures that offer faster access times and better support for operations commonly used in memory management, such as AVL trees or binary heaps.
// TODO: Add comprehensive comments and documentation - Document each function and critical sections of the code to improve readability and maintainability, making it easier for others to understand and modify the code base.
// TODO: Introduce a mechanism to handle memory fragmentation - Develop strategies to combat memory fragmentation, possibly through smarter allocation strategies or periodic compaction of memory.
// TODO: Implement unit tests for memory management functions - Create a suite of unit tests that cover various scenarios of allocation and deallocation to ensure that the memory management functions are working as expected under different conditions.
// TODO: Profile and optimize memory management operations - Use profiling tools to identify bottlenecks in the memory management implementation and address these with targeted optimizations.
// TODO: Ensure thread safety for allocator functions - If the code is used in a multi-threaded environment, add mechanisms to ensure that allocation and deallocation are thread-safe, such as mutexes or other synchronization primitives.

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

  // If you don't find a chunk with enough size try merging freechunks and try again
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
  assert(ptr >= heap && ptr < heap + HEAP_SIZE);
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
