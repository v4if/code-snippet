#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 函数open、read、write、lseek以及close提供了不用缓冲的IO
 从标准输入读，并写向标准输出
 */
#define BUFFSIZE 4096
int main(void)
{
  int n;
  char buf[BUFFSIZE];

  // 阻塞I/O
  while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
  {
    if (write(STDOUT_FILENO, buf, n) != n)
    {
      printf("write error");
      exit(1);
    }
  }

  if (n < 0)
  {
    printf("read error");
    exit(1);
  }

  exit(0);
}