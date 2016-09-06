#include <stdio.h>

extern __attribute__((unused)) void foo() {
  printf("in library symbol\n");
}