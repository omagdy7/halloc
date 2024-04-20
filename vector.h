#ifndef VECTOR

#include <stdlib.h>

typedef struct {
  int *data;       // Pointer to the array of integers
  size_t size;     // Number of elements currently in the array
  size_t capacity; // Total available capacity
} vector;

void darray_init(vector *array, size_t initialCapacity);
void darray_append(vector *array, int value);
void darray_free(vector *array);

#endif // !VECTOR
