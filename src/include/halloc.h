#ifndef HALLOC

#include "hashmap.h"
#include <stdbool.h>
#include <stdio.h>

#define HEAP_SIZE (64 * (1000000)) // 64MB
#define NOT_FOUND (size_t) - 1

extern char heap[HEAP_SIZE];
extern HashMap heap_allocated;
extern HashMap heap_freed;

void dump_arr(int *arr, size_t sz);
void *halloc(size_t size);
void *min(void *a, void *b);
void merge_free_chunks();
void hfree(void *ptr);

#endif // !HALLOC
