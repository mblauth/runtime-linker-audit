#include <link.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <stdbool.h>

#include "pipes.h"

#define used __attribute__((unused))
#define ignored __attribute__((unused))

int a2lFD = -1;
int l2aFD = -1;

extern used unsigned la_version(unsigned version) {
  return version;
}

extern used void la_preinit(ignored uintptr_t * cookie) {
  printf("Initial loading finished\n");
  printf("opening pipes\n");
  a2lFD = open(PIPE_A2L, O_RDWR);
  l2aFD = open(PIPE_L2A, O_RDWR);
}

extern used char * la_objsearch(const char * name, ignored uintptr_t * cookie, unsigned flag) {
  if (flag == LA_SER_ORIG) printf("rtld is searching for symbol in library %s\n", name);
  if (l2aFD > -1) {
    write(l2aFD, name, strlen(name));
    bool auditResult = false;

    read(a2lFD, &auditResult, sizeof(bool));
    if (auditResult) printf("loading of %s accepted\n", name);
  }
  return (char *) name;
}

extern used void la_activity(uintptr_t * cookie, unsigned flag) {
  if (flag == LA_ACT_ADD) printf("adding element to link-map\n");
}

extern used unsigned la_objopen(struct link_map * map, Lmid_t lmid, uintptr_t * cookie) {
  printf("new object was loaded\n");
  return LA_FLG_BINDTO | LA_FLG_BINDFROM;
}

extern used uintptr_t la_symbind32(Elf32_Sym * symbol, unsigned index, uintptr_t * refcook, uintptr_t * defcook,
                                   unsigned * flags, const char * symbolName) {
  printf("binding symbol %s\n", symbolName);
  return symbol->st_value;
}

extern used uintptr_t la_symbind64(Elf64_Sym * symbol, unsigned index, uintptr_t * refcook, uintptr_t * defcook,
                                   unsigned * flags, const char * symbolName) {
  printf("binding symbol %s\n", symbolName);
  return symbol->st_value;
}

extern used void la_pltenter() {
  printf("pltenter");
}
