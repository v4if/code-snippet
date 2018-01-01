#include <sys/epoll.h> // epoll
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <memory.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 9877
#define BACKLOG 10
#define MAX_EVENTS 10 // 最大全连接数
#define MAX_LINE 4096

// ab 压测
// ulimit -a
// ulimit -n 65535

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("setnonblocking");
        exit(1);
    }
}

void do_use_fd(int conn_fd)
{
    int n;
    char buff[MAX_LINE];
    if ((n = read(conn_fd, buff, sizeof(buff))) == 0) {
//        FIN被动关闭
        close(conn_fd);
        return;
    }
    time_t ticks;
    ticks = time(NULL);

    char resp[MAX_LINE];
    char body[MAX_LINE];
    snprintf(body, sizeof(body), "<html><body>%s</body></html>", ctime(&ticks));

    snprintf(resp, sizeof(resp), "HTTP/1.1 200 OK\nContent-Length: %d\n\n%s", strlen(body), body);

    if (send(conn_fd,  resp,  strlen(resp),  0) == -1)
        perror("send");
    close(conn_fd);
}

int main() {
    struct sockaddr_in serv_addr, remote_addr;
    struct epoll_event ev, events[MAX_EVENTS];
    int listenfd, conn_sock, nfds, epollfd;
    char buff[MAX_LINE];

    socklen_t addrlen = sizeof(struct sockaddr_in);

    if ((listenfd = socket(AF_INET,  SOCK_STREAM,  0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero(&serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd,  (struct sockaddr *)&serv_addr,  sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(listenfd,  BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    epollfd = epoll_create(10);
    if (epollfd == -1)
    {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
    {
        perror("epoll_ctl: listenfd");
        exit(EXIT_FAILURE);
    }

    printf("server start on port : %d\n", PORT);
    for (;;)
    {

        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_pwait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == listenfd)
            {
                conn_sock = accept(listenfd,
                                   (struct sockaddr *)&remote_addr, &addrlen);
                if (conn_sock == -1)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                printf("connection from %s, port %d\n", inet_ntop(AF_INET, &remote_addr.sin_addr, buff, sizeof(buff)), ntohs(remote_addr.sin_port));

                setnonblocking(conn_sock);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                              &ev) == -1)
                {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                do_use_fd(events[n].data.fd);
            }
        }
    }
}