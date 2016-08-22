#include <stdio.h>
#include <dlfcn.h>

int main() {
  printf("Launching test\n");
  void * handle = dlopen("libtestlib.so", RTLD_LAZY);
  void (*foo)() = dlsym(handle, "foo");
  (*foo)();
  dlclose(handle);
}