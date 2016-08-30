#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#include "pipes.h"
#include "rpm_checker.h"

void launch(char const * command, char * const * args) {
  char * environment[] = { "LD_AUDIT=./libaudit.so", NULL };
  execve(command, args, environment);
}

void deleteFifosAndExit(int result) {
  if (unlink(PIPE_A2L) == -1 || unlink(PIPE_L2A) == -1) perror("failed to delete fifos");
  exit(result);
}

int openPipe(char const * name, int mode) {
  if (mkfifo(name, 0666) == -1) {
    if (errno == EEXIST) {
      //accept multiple instances for now perror("Another auditor instance might already be running");
    } else perror(strerror(errno));
    //accept multiple instances for now exit(1);
  }
  return open(name, mode | O_CLOEXEC);
}

int main(int argc, char ** args) {

  if (argc < 2) {
    fprintf(stderr, "no command provided\n");
    deleteFifosAndExit(4);
  }

  printf("launching %s\n", args[1]);
  pid_t pid = fork();
  switch (pid) {
    case 0:
      launch(args[1], &args[2]);
      break;
    case -1:
      perror("fork failed");
      deleteFifosAndExit(2);
    default:
      printf("child pid is %d\n", pid);
  }


  int l2a = openPipe(PIPE_L2A, O_RDONLY);
  int a2l = openPipe(PIPE_A2L, O_WRONLY);
  fcntl(a2l, F_SETPIPE_SZ, sizeof(char));
  if (a2l == -1 || l2a == -1) {
    perror("couldn't open fifo for communication with audit library");
    deleteFifosAndExit(3);
  }

  char rcvbuf[1024];
  while (read(l2a, &rcvbuf, 1024) != 0) {
    printf("audit-request for library %s\n", rcvbuf);
    char auditResult = audit(rcvbuf);
    write(a2l, &auditResult, sizeof(char));
  }

  int result;
  printf("waiting for child process to exit\n");
  wait(&result);
  printf("%s exited\n", args[1]);
  close(a2l);
  close(l2a);
  deleteFifosAndExit(0);
}