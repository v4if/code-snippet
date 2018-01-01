#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

/*
 可以使用nc -u 127.0.0.1 9877进行命令测试
 */
#define MAXLINE 4096
// 给定套接字上内核为之排队的最大已完成连接数
#define RQ_BACKLOG 5 //511

#define PORT 9877

#define SA struct sockaddr

int main(int argc, char **argv)
{
  int sockfd, nbytes;
  socklen_t len;
  struct sockaddr_in servaddr, cliaddr;
  char buff[MAXLINE], addrnp[MAXLINE];

  // (domain, type, protocol)根据指定的地址族、数据类型和协议来分配一个socket的描述字及其所用的资源。
  // TCP 字节流
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    perror("bind error");
  }

  for (;;)
  {
    len = sizeof(cliaddr);
    nbytes = recvfrom(sockfd, buff, MAXLINE, 0, (SA *)&cliaddr, &len);
    printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, addrnp, sizeof(addrnp)), ntohs(cliaddr.sin_port));
    sendto(sockfd, buff, nbytes, 0, (SA *)&cliaddr, len);
  }

  exit(0);
}