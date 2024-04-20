#include "vector.h"
#include "halloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to initialize the dynamic array
void darray_init(vector *array, size_t initialCapacity) {
  array->data = (int *)halloc(initialCapacity * sizeof(int));
  if (array->data == NULL) {
    fprintf(stderr, "Failed to allocate memory.\n");
    exit(EXIT_FAILURE);
  }
  array->size = 0;
  array->capacity = initialCapacity;
}

// Function to add an element to the dynamic array
void darray_append(vector *array, int value) {
  // Check if the array needs to grow
  printf("size: %zu, cap: %zu\n", array->size, array->capacity);
  if (array->size == array->capacity) {
    size_t newCapacity = array->capacity * 2;
    int *newData = (int *)halloc(newCapacity * sizeof(int));
    if (newData == NULL) {
      fprintf(stderr, "Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
    }
    // Copy old data to new data
    memcpy(newData, array->data, array->capacity * sizeof(int));
    hfree(array->data); // Free old data
    array->data = newData;
    array->capacity = newCapacity;
  }
  array->data[array->size++] = value;
}

// Function to free the dynamic array
void darray_free(vector *array) {
  hfree(array->data);
  array->data = NULL;
  array->size = 0;
  array->capacity = 0;
}
