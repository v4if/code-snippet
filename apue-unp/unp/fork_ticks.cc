#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h> // sockaddr
#include <string.h>
#include <sys/wait.h>  // wait
#include <arpa/inet.h> // inet_ntop
#include <errno.h>     // errno

#define MAXLINE 4096
// 给定套接字上内核为之排队的最大已完成连接数
#define RQ_BACKLOG 5 //511

#define PORT 81

#define SA struct sockaddr

// 等待子进程僵死进程信号处理
void wait4children(int signo)
{
  int status;
  pid_t pid;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    printf("teminated child %d\n", pid);
  ;
}

int main(int argc, char **argv)
{
  int sockfd, connfd;
  socklen_t len;
  struct sockaddr_in servaddr, cliaddr;
  pid_t pid;
  char buff[MAXLINE];
  time_t ticks;

  // (domain, type, protocol)根据指定的地址族、数据类型和协议来分配一个socket的描述字及其所用的资源。
  // TCP 字节流
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    perror("bind error");
  }
  // 全连接队列
  if (listen(sockfd, RQ_BACKLOG) < 0)
  {
    perror("listen error");
  }

  // 处理子进程僵死进程
  signal(SIGCHLD, wait4children);

  for (;;)
  {
    len = sizeof(cliaddr);
    if ((connfd = accept(sockfd, (SA *)&cliaddr, &len)) < 0)
    {
      // 当阻塞于某个慢系统调用的一个进程捕获某个信号且相应信号处理函数返回时，该系统调用可能会返回一个EINTR错误。
      if (errno == EINTR)
      {
        continue;
      }
      else
      {
        perror("connect error");
        exit(1);
      }
    }

    if ((pid = fork()) == 0) // child
    {
      close(sockfd);

      // 服务器处理
      printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));

      ticks = time(NULL);

      // sleep(5);
      snprintf(buff, sizeof(buff), "%s\n", ctime(&ticks));

      char server_header[MAXLINE];
      sprintf(server_header, "%s%d%s", "HTTP/1.1 200 OK\nContent-Length: ",
              strlen(buff), "\nContent-Type: text/html; charset=UTF-8\n\n");
      write(connfd, server_header, strlen(server_header));

      write(connfd, buff, strlen(buff));

      close(connfd);
      exit(0);
    }

    if (pid > 0)
    {
      printf("fork child pid : %d\n", pid);
    }

    close(connfd);
  }

  exit(0);
}