/*
 * tcp_client.c —— 配合 tcp_server.c 的 TCP 客户端
 *
 * 编译：
 *   clang -Wall -Wextra -O2 tcp_client.c -o tcp_client
 * 运行：
 *   ./tcp_client                     # 连接 127.0.0.1:8080
 *   ./tcp_client 127.0.0.1 9000      # 指定地址和端口
 *
 * 客户端比服务端简单，只有两步：
 *   socket()   创建套接字
 *   connect()  向服务器发起连接
 * 成功后同样用 send() / recv() 收发。
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

static int send_all(int fd, const char *buf, size_t len)
{
    size_t sent = 0;

    while (sent < len) {
        ssize_t n = send(fd, buf + sent, len - sent, 0);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        sent += (size_t)n;
    }
    return 0;
}

/* 读满 len 字节才返回（TCP 是字节流，一次 recv 不一定拿全）
   返回实际读到的字节数；-1 表示出错 */
static int recv_all(int fd, char *buf, size_t len)
{
    size_t got = 0;

    while (got < len) {
        ssize_t n = recv(fd, buf + got, len - got, 0);
        if (n == 0)
            return (int)got;          /* 服务器关闭了连接 */
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        got += (size_t)n;
    }
    return (int)got;
}

int main(int argc, char **argv)
{
    const char *host = (argc > 1) ? argv[1] : "127.0.0.1";
    int port = (argc > 2) ? atoi(argv[2]) : 8080;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);

    /* 这个例子只接受 IPv4 数字地址（如 127.0.0.1）。
       如果要支持域名（如 example.com），用 getaddrinfo() 更合适。 */
    if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
        fprintf(stderr, "地址无效：%s（这里只支持 IPv4 数字地址）\n", host);
        close(fd);
        return 1;
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof addr) < 0) {
        perror("connect");
        close(fd);
        return 1;
    }

    printf("已连接 %s:%d，输入内容回车发送（Ctrl+D 退出）\n", host, port);
    fflush(stdout);

    char line[1024];
    char back[1024];

    while (fgets(line, sizeof line, stdin) != NULL) {
        size_t len = strlen(line);

        if (send_all(fd, line, len) < 0) {
            perror("send");
            break;
        }

        /* 回声服务器会把 len 个字节原样发回来 */
        int got = recv_all(fd, back, len);
        if (got < 0) {
            perror("recv");
            break;
        }
        if ((size_t)got < len) {
            printf("服务器关闭了连接\n");
            break;
        }

        fwrite(back, 1, (size_t)got, stdout);
        fflush(stdout);
    }

    close(fd);
    return 0;
}
