#include "halloc.h"
#include "vector.h"

int main() {
  hashmap_init(&heap_allocated, int_hash, int_compare);
  hashmap_init(&heap_freed, int_hash, int_compare);
  hashmap_insert(&heap_freed, heap, HEAP_SIZE);

  // size_t size = 8;
  // for (size_t t = 0; t < 3; t++) {
  //   int *arr = (int *)halloc(size * sizeof(int));
  //   for (size_t i = 0; i < t; i++) {
  //     arr[i] = i + 1;
  //   }
  //   printf("t: %zu\n", t);
  //   printf("heap_allocated: \n");
  //   dump_hashmap(&heap_allocated);
  //   printf("heap_freed: \n");
  //   dump_hashmap(&heap_freed);
  //   if (t % 2 == 0) {
  //     hfree(arr);
  //   }
  //
  //   printf("--------------------------------------------------\n");
  // }
  // // printf("heap_allocated: \n");
  // // dump_hashmap(&heap_allocated);
  // printf("before merging heap_freed: \n");
  // dump_hashmap(&heap_freed);
  // merge_free_chunks();
  // int *arr = (int *)halloc(32);
  // printf("after merging heap_freed: \n");
  // dump_hashmap(&heap_freed);

  {
    vector array;
    darray_init(&array, 4); // Initialize with a capacity of 4

    // Add some elements
    for (size_t i = 0; i < 10; i++) {
      darray_append(&array, i + 1); // This should trigger a resize
    }
    printf("size: %zu \n", array.size);

    // Print the elements
    for (size_t i = 0; i < array.size; i++) {
      printf("%d ", array.data[i]);
    }
    printf("\n");

    printf("allocated: \n");
    dump_hashmap(&heap_allocated);

    printf("Before: \n");
    dump_hashmap(&heap_freed);
    // Cleanup
    darray_free(&array);
    printf("After: \n");
    merge_free_chunks();
    dump_hashmap(&heap_freed);
  }
  return 0;
}
