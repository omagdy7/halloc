#include "include/halloc.h"
#include <stdio.h>

#define ALIGN(ptr) ((void *)(((uintptr_t)(ptr) + 7) & ~((uintptr_t)0x7)))
// Define the alignment (8 bytes)
#define ALIGNMENT 8
#define ALIGN_MASK (ALIGNMENT - 1)

// Define flags (up to 3 bits)
#define ALLOCED 0x1 // Binary: 001

// Macro to align a size to the nearest 8-byte boundary
#define ALIGN_SIZE(size) (((size) + ALIGN_MASK) & ~ALIGN_MASK)

// Macro to set flags in the size variable
#define SET_FLAGS(size, flags) ((size) |= (flags))

// Macro to extract flags from the size variable
#define GET_FLAGS(size) ((size) & ALIGN_MASK)

// Macro to clear flags from the size variable and get the original size
#define CLEAR_FLAGS(size) ((size) & ~ALIGN_MASK)

char heap[HEAP_SIZE];

ptr cur_heap = heap;
usize cur_size = HEAP_SIZE;
u32 cur_idx = 0;
chunk_t free_list[(1 << 20)];

void dump_free_list() {
  ptr start = free_list;
  usize i = 0;
  while (i < cur_idx) {
    printf("start: %p\n", start);
    size_t size = ((chunk_t *)start)->header.size;
    printf("size: %zu\n", CLEAR_FLAGS(size));
    printf("Alloc flag: %zu\n", GET_FLAGS(size));
    start += 24;
    i++;
  }
}

void add_chunk(size_t size, bool is_alloced) {
  if (size == 0)
    return;
  chunk_t *cur_chunk = &free_list[cur_idx];
  cur_chunk->header.size = ALIGN_SIZE(size);
  // printf("size: %zu\n", cur_chunk->header.size);
  if (is_alloced) {
    SET_FLAGS(cur_chunk->header.size, ALLOCED);
  }
  cur_chunk->start = cur_heap;
  cur_chunk->footer.size = cur_chunk->header.size;
  cur_idx++;
  cur_heap += CLEAR_FLAGS(cur_chunk->header.size);
}

void init_free_list() {
  size_t size = sizeof(chunk_t);
  add_chunk(HEAP_SIZE, false);
}

void dump_arr(int *arr, size_t sz) {
  printf("{ ");
  for (size_t i = 0; i < sz; i++) {
    printf("%d ", arr[i]);
  }
  printf("}\n");
}

// Policy: first_fit
// Finds the first chunk that satisfies the size and marks it as allocated and splits the chunk
// into 2 other chunks if the size of the chunk is bigger than the specifed size
void *halloc(size_t size) {
  if (size == 0) {
    return NULL;
  }
  size = ALIGN_SIZE(size);
  cur_size -= size;
  ptr start = free_list;
  usize i = 0;
  usize tmp_idx = cur_idx;
  while (i < tmp_idx) {
    size_t size_with_flags = CLEAR_FLAGS(((chunk_t *)start)->header.size);
    size_t chunk_size = CLEAR_FLAGS(((chunk_t *)start)->header.size);
    size_t is_alloced = GET_FLAGS(size_with_flags);
    // printf("size_with_flags: %zu\n", size_with_flags);
    // printf("chunk_size: %zu\n", chunk_size);
    // printf("is_alloced: %zu\n", is_alloced);
    if (!is_alloced && size <= chunk_size) {
      ((chunk_t *)start)->header.size = size;
      ((chunk_t *)start)->footer.size = size;
      SET_FLAGS(((chunk_t *)start)->header.size, ALLOCED);
      SET_FLAGS(((chunk_t *)start)->footer.size, ALLOCED);
      add_chunk(cur_size, false);
      return (void *)((chunk_t *)start)->start;
    }
    start += sizeof(chunk_t);
    i++;
  }
  return NULL;
};

void hfree(void *ptr){
    //
};
