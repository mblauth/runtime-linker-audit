#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main() {
  printf("Launching test\n");
  void * handle = dlopen("./libtestlib.so", RTLD_LAZY);
  if (handle == NULL) {
    perror("could not load library\n");
    exit(1);
  }
  void (*foo)() = dlsym(handle, "foo");
  if (foo == NULL) {
    perror("could not load symbol\n");
    exit(2);
  }
  (*foo)();
  dlclose(handle);
}