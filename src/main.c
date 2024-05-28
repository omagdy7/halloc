#include "include/halloc.h"
#include <stdio.h>

int main() {
  init_free_list();
  void *p = halloc(37);
  printf("p: %p\n", p);
  dump_free_list();
}
