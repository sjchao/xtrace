#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

void getErrmsg(char*);

int main(int argc, char** argv) {
  if (argc <= 1) {
    printf("Usage: xtrace pid\n");
    return -1;
  }
  int pid = atoi(argv[1]);
  long ret = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
  if (ret != 0) {
    char msg[100];
    getErrmsg(msg);
    printf("failed to attach to process %d\n\t%s\n", pid, msg);
    return -1;
  }
  printf("attach to process %d\n", pid);
  ptrace(PTRACE_CONT, pid, NULL, NULL);
  int status;
  while (1) {
    printf("wait process %d signal\n", pid);
    waitpid(pid, &status, 0);
    if (WIFSTOPPED(status)) {
      siginfo_t siginfo;
      ptrace(PTRACE_GETSIGINFO, pid, NULL, &siginfo);
      printf("receive signal\n\tsigno:%d pid:%d uid:%d\n", siginfo.si_signo, siginfo.si_pid, siginfo.si_uid);
      ptrace(PTRACE_CONT, pid, NULL, siginfo.si_signo);
      break;
    }
  }
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  printf("detach from process %d\n", pid);
  return 0;
}

void getErrmsg(char* errmsg) {
  if (errmsg == NULL) {
    return;
  }
  char* msg;
  switch (errno) {
    case EBUSY:
      msg = "EBUSY: error with allocating or freeing a debug register.";
      break;
    case EFAULT:
      msg = "EFAULT: attempt to read from or write to an invalid area.";
      break;
    case EINVAL:
      msg = "EINVAL: attemp to set an invalid option.";
      break;
    case EIO:
      msg = "EIO: request is invalid.";
      break;
    case EPERM:
      msg = "EPERM: the specified process cannot be tarced, maybe you need more privileges.";
      break;
    case ESRCH:
      msg = "ESRCH: the specified process does not exist.";
      break;
    default:
      msg = "unknown error";
      break;
  }
  memcpy(errmsg, msg, strlen(msg));
}
