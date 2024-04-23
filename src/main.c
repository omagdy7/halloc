#include "darray.h"
#include "halloc.h"

int main() {
  hashmap_init(&heap_allocated, int_hash, int_compare);
  hashmap_init(&heap_freed, int_hash, int_compare);
  hashmap_insert(&heap_freed, heap, HEAP_SIZE);

  vector array;
  darray_init(&array, 4);

  for (size_t i = 0; i < 10; i++) {
    darray_append(&array, i + 1);
  }
  printf("size: %zu \n", array.size);

  for (size_t i = 0; i < array.size; i++) {
    printf("%d ", array.data[i]);
  }
  printf("\n");

  printf("allocated: \n");
  dump_hashmap(&heap_allocated);

  printf("Before: \n");
  dump_hashmap(&heap_freed);

  darray_free(&array);
  printf("After: \n");
  merge_free_chunks();
  dump_hashmap(&heap_freed);
  return 0;
}
