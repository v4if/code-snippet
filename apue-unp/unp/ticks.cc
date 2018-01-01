#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096
// 给定套接字上内核为之排队的最大已完成连接数
#define RQ_BACKLOG 5 //511

#define PORT 9877

#define SA struct sockaddr

int main(int argc, char **argv)
{
  int sockfd, connfd;
  socklen_t len;
  struct sockaddr_in servaddr, cliaddr;
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

  // 迭代
  for (;;)
  {
    len = sizeof(cliaddr);
    if ((connfd = accept(sockfd, (SA *)&cliaddr, &len)) < 0)
    {
      perror("connect error");
      continue;
    }

    printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));

    ticks = time(NULL);

    // sleep(5);
    snprintf(buff, sizeof(buff), "<html><body>%s</body></html>\n\n", ctime(&ticks));

    char server_header[MAXLINE];
    sprintf(server_header, "%s%d%s", "HTTP/1.1 200 OK\nContent-Length: ",
            strlen(buff), "\nContent-Type: text/html; charset=UTF-8\n\n");
    write(connfd, server_header, strlen(server_header));

    write(connfd, buff, strlen(buff));
    close(connfd);
  }

  exit(0);
}