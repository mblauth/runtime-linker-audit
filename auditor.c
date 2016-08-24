#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#include "rpm_checker.h"

void launch(char const * command, char * const * args) {
  char * environment[] = { "LD_AUDIT=./liblicense-audit.so", NULL };
  execve(command, args, environment);
}

int main(int argc, char ** args) {
//  iterateRPMDB();
  pid_t pid = fork();
  switch (pid) {
    case 0:
      launch(args[1], &args[2]);
      break;
    case -1:
      perror("fork failed");
      break;
    default:
      printf("child pid is %d\n", pid);
  }
  int result;
  wait(&result);
  return result;
}