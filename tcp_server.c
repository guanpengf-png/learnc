/*
 * tcp_server.c —— 一个最小的 TCP 回声服务器
 *
 * 编译：
 *   clang -Wall -Wextra -O2 tcp_server.c -o tcp_server
 * 运行：
 *   ./tcp_server            # 默认监听 8080
 *   ./tcp_server 9000       # 指定端口
 *
 * 它做的事（TCP 服务端的标准四步）：
 *   socket()  创建套接字
 *   bind()    绑定本机 IP + 端口
 *   listen()  开始监听，等待连接
 *   accept()  取出一个已建立的连接（没有连接时会阻塞）
 * 之后就是 recv() / send() 收发数据。
 *
 * 注意：这是“一次服务一个客户端”的版本。想同时服务多个客户端，
 * 需要在 accept 后 fork() 子进程、或开线程、或用 select()/poll() 多路复用。
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

/* send() 可能只发出去一部分，循环直到全部发完 */
static int send_all(int fd, const char *buf, size_t len)
{
    size_t sent = 0;

    while (sent < len) {
        ssize_t n = send(fd, buf + sent, len - sent, 0);
        if (n < 0) {
            if (errno == EINTR)      /* 被信号打断，重试 */
                continue;
            return -1;
        }
        sent += (size_t)n;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int port = 8080;
    if (argc > 1)
        port = atoi(argv[1]);

    /* 对端已经关闭后继续 send，默认会触发 SIGPIPE 把进程杀掉。
       忽略它，让 send 返回 -1 和 EPIPE，我们才能优雅处理。 */
    signal(SIGPIPE, SIG_IGN);

    /* 1. 创建套接字：IPv4 + 流式（TCP） */
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        return 1;
    }

    /* 允许重启服务器后立刻复用端口，避免 "Address already in use" */
    int yes = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    /* 2. 绑定地址：INADDR_ANY 表示本机所有网卡都监听 */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);   /* 主机字节序 -> 网络字节序 */
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof addr) < 0) {
        perror("bind");
        close(listen_fd);
        return 1;
    }

    /* 3. 开始监听，第二个参数是等待队列长度 */
    if (listen(listen_fd, 8) < 0) {
        perror("listen");
        close(listen_fd);
        return 1;
    }

    printf("TCP 回声服务器已启动，监听端口 %d（Ctrl+C 退出）\n", port);
    fflush(stdout);

    /* 4. 主循环：一次处理一个客户端 */
    for (;;) {
        struct sockaddr_in peer;
        socklen_t peerlen = sizeof peer;

        int fd = accept(listen_fd, (struct sockaddr *)&peer, &peerlen);
        if (fd < 0) {
            if (errno == EINTR)
                continue;
            perror("accept");
            break;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peer.sin_addr, ip, sizeof ip);
        printf("客户端 %s:%d 已连接\n", ip, ntohs(peer.sin_port));
        fflush(stdout);

        /* 收多少就原样发回多少 */
        char buf[4096];
        ssize_t n;

        while ((n = recv(fd, buf, sizeof buf, 0)) > 0) {
            if (send_all(fd, buf, (size_t)n) < 0) {
                perror("send");
                break;
            }
        }

        if (n < 0)
            perror("recv");

        close(fd);
        printf("客户端已断开\n");
        fflush(stdout);
    }

    close(listen_fd);
    return 0;
}
