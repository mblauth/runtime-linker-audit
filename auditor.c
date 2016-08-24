#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#include "pipes.h"
#include "rpm_checker.h"

void launch(char const * command, char * const * args) {
  char * environment[] = { "LD_AUDIT=./liblicense-audit.so", NULL };
  execve(command, args, environment);
}

int openPipe(char const * name, int mode) {
  mkfifo(name, 0666);
  return open(name, mode | O_CLOEXEC);
}

int main(int argc, char ** args) {
//  iterateRPMDB();
  printf("opening pipes\n");
  int a2l = openPipe(PIPE_A2L, O_RDWR);
  int l2a = openPipe(PIPE_L2A, O_RDWR);

  printf("launching %s\n", args[1]);
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

  char rcvbuf[1024];
  while (read(l2a, &rcvbuf, 1024)) {
    printf("audit-request for library %s\n", rcvbuf);
    bool auditResult = true;
    write(a2l, &auditResult, sizeof(bool));
  }

  int result;
  wait(&result);
  close(a2l);
  close(l2a);
  unlink(PIPE_A2L);
  unlink(PIPE_L2A);
  return result;
}