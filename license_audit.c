#include <link.h>
#include <stdio.h>

#define used __attribute__((unused))
#define ignored __attribute__((unused))

extern used unsigned la_version(unsigned version) {
  return version;
}

extern used void la_preinit(ignored uintptr_t * cookie) {
  printf("Initial loading finished\n");
}

extern used char * la_objsearch(const char * name, ignored uintptr_t * cookie, unsigned flag) {
  if (flag == LA_SER_ORIG) printf("rtld is searching for symbol in library %s\n", name);
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
