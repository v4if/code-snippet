#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void wait4children(int signo)
{
  int status;
  pid_t pid;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    printf("teminated child %d\n", pid);
  ;
}

int main()
{

  int i;
  pid_t pid;

  signal(SIGCHLD, wait4children);

  for (i = 0; i < 100; i++)
  {
    pid = fork();
    if (pid == 0)
    {
      printf("from child %d\n", i);
      break;
    }
  }

  if (pid > 0)
  {
    printf("press Enter to exit...");
    getchar();
  }

  return 0;
}