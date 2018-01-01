#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h> // sockaddr
#include <string.h>
#include <sys/wait.h>   // wait
#include <arpa/inet.h>  // inet_ntop
#include <errno.h>      // errno
#include <sys/select.h> // select

/*
 存在问题：read时可能因为一个客户而被阻塞，而不能再为其他任何客户提供服务
 */

#define MAXLINE 4096
// 给定套接字上内核为之排队的最大已完成连接数
#define RQ_BACKLOG 5 //511

#define PORT 80

#define SA struct sockaddr

// 单进程 + select
int main(int argc, char **argv)
{
  int sockfd, connfd;
  socklen_t len;
  struct sockaddr_in servaddr, cliaddr;
  pid_t pid;
  char buff[MAXLINE];
  time_t ticks;

  // select相关
  int i, nready, client[FD_SETSIZE]; // 就绪描述符数目， 记录客户端connfd描述符
  int maxi, maxfd;                   // client数组当前使用项的最大下标， 当前需要监听的最大描述符
  fd_set rfds, origin_set;           // 需要监听的描述符集

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

  maxfd = sockfd;
  maxi = -1;
  for (i = 0; i < FD_SETSIZE; i++)
  {
    client[i] = -1; // -1代表项可用
  }
  FD_ZERO(&origin_set);
  FD_SET(sockfd, &origin_set);

  for (;;)
  {
    rfds = origin_set; // 传参为描述符集副本
    nready = select(maxfd + 1, &rfds, NULL, NULL, NULL);

    // rfds已被修改为就绪描述符集合
    if (FD_ISSET(sockfd, &rfds)) // 新客户端SYN
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

      printf("connection from %s, port %d\r\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));

      // 记录ESTABLISHED的客户端connfd
      for (i = 0; i < FD_SETSIZE; i++)
      {
        if (client[i] < 0) // 最近适配，第一个未用项记录这个已连接描述符
        {
          client[i] = connfd;
          break;
        }
      }

      if (i == FD_SETSIZE)
      {
        perror("too many clients");
        exit(1);
      }

      FD_SET(connfd, &origin_set);
      if (connfd > maxfd)
      {
        maxfd = connfd;
      }
      if (i > maxi)
      {
        maxi = i;
      }

      // 是否还有未处理的就绪描述符
      if (--nready <= 0)
      {
        continue;
      }
    }

    // 遍历客户端描述符是否就绪，检查现有连接
    for (i = 0; i < maxi; i++)
    {
      if ((connfd = client[i]) < 0)
      {
        continue;
      }

      if (FD_ISSET(connfd, &rfds))
      {
        if ((len = read(connfd, buff, MAXLINE)) == 0)
        {
          // 连接被动关闭
          close(connfd);
          FD_CLR(connfd, &origin_set);
          client[i] = -1;
        }
        else
        {
          ticks = time(NULL);
          snprintf(buff, sizeof(buff), "%s\r\n", ctime(&ticks));

          char server_header[MAXLINE];
          sprintf(server_header, "%s%d%s", "HTTP/1.1 200 OK\nContent-Length: ",
                  strlen(buff), "\nContent-Type: text/html; charset=UTF-8\n\n");
          write(connfd, server_header, strlen(server_header));

          write(connfd, buff, strlen(buff));
        }

        if (--nready <= 0)
        {
          break;
        }
      }
    }
  }

  exit(0);
}