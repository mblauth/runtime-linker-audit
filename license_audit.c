#include <link.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#include "pipes.h"

#define used __attribute__((unused))
#define ignored __attribute__((unused))

volatile int a2lFD = -1;
volatile int l2aFD = -1;
pthread_mutex_t * mutex;

static __attribute__((constructor)) void startup() {
  printf("initializing audit library...");
  l2aFD = open(PIPE_L2A, O_WRONLY);
  fcntl(l2aFD, F_SETPIPE_SZ, 1024);
  a2lFD = open(PIPE_A2L, O_RDONLY);
  pthread_mutex_init(mutex, NULL);
  printf("done\n");
}

extern used unsigned la_version(unsigned version) {
  return version;
}

extern used void la_preinit(ignored uintptr_t * cookie) {
  printf("Initial library loading finished\n");
}

static __attribute__((destructor)) void shutdown() {
  printf("audit library shutdown\n");
  pthread_mutex_lock(mutex);
  close(a2lFD); a2lFD = -1;
  close(l2aFD); l2aFD = -1;
  pthread_mutex_unlock(mutex);
}

extern used char * la_objsearch(const char * name, ignored uintptr_t * cookie, unsigned flag) {
  printf("rtld is searching for symbol in library %s\n", name);
  if(strcmp("libdl.so.2", name) == 0) return (char *) name;
  if(strcmp("/lib64/libdl.so.2", name) == 0) return (char *) name;
  if(strcmp("libc.so.6", name) == 0) return (char *) name;
  if(strcmp("/lib64/libc.so.6", name) == 0) return (char *) name;
  pthread_mutex_lock(mutex);
  if (l2aFD > -1) {
    write(l2aFD, name, strlen(name)+1);
    char auditResult = 0;

    printf("retrieving from %d\n", a2lFD);
    ssize_t size = read(a2lFD, &auditResult, sizeof(char));
    if(size == -1) {
      perror("Encountered an error retrieving the audit result");
      close(l2aFD);
      char buf = 0;
      fcntl(a2lFD, F_SETFL, O_RDONLY|O_NONBLOCK);
      read(a2lFD, &buf, sizeof(char));
      close(a2lFD);
      exit(1);
    }
    if (auditResult) {
      printf("loading of %s accepted\n", name);
      pthread_mutex_unlock(mutex);
      return (char *) name;
    } else {
      printf("loading of %s denied: %d\n", name, auditResult);
      pthread_mutex_unlock(mutex);
      shutdown();
      return NULL;
    }
  }
  return (char *) name;
}

