#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "types.h"

#define HEAP_SIZE (64 * (1000000)) // 64MB

// the last 3 bits are used for flags and need to be masked out when actually using the size
// +----------------------------------+
// |000000000000000000000000000000|000|
// |---------------size-----------|A--|
// +----------------------------------+
typedef struct {
  size_t size;
} meta_data;

typedef struct {
  meta_data header;
  ptr start;
  meta_data footer;
} chunk_t;

extern char heap[HEAP_SIZE];
extern chunk_t free_list[(1 << 20)];

void dump_arr(int *arr, size_t sz);
void dump_free_list();
void init_free_list();
void *halloc(size_t size);
void hfree(void *ptr);
