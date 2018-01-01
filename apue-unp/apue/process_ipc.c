// 管道pipe、fifo命名管道，优于消息队列
// 消息队列
// 信号量semaphore，它是一个计数器，用于进程对共享数据对象的访问，优于记录锁，同步原语
// 共享存储，一种最快的IPC
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLINE 4096

int main(void)
{
  int n;
  int fd[2];
  pid_t pid;
  char line[MAXLINE];

  // 读写文件描述符
  if (pipe(fd) < 0)
  {
    printf("pipe error");
    exit(1);
  }

  if ((pid = fork()) < 0)
  {
    printf("fork error");
    exit(1);
  }
  else if (pid > 0) // parent
  {
    close(fd[0]);
    write(fd[1], "hello world\n", 12);
  }
  else
  {
    close(fd[1]);
    n = read(fd[0], line, MAXLINE);
    write(STDOUT_FILENO, line, n);
  }

  exit(0);
}