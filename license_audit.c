#include <link.h>
#include <stdio.h>

#define unused __attribute__((unused))

extern unused unsigned la_version(unsigned version) {
  return version;
}

extern unused void la_preinit(unused uintptr_t * cookie) {
  printf("Initial loading finished\n");
}

extern unused char * la_objsearch(const char * name, unused uintptr_t * cookie, unused unsigned flag) {
  if (flag == LA_SER_ORIG)
    printf("rtld is searching for symbol %s\n", name);
  return (char *) name;
}
