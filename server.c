#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#define SERV_PORT 8000

int main(void)
{
    int sfd, cfd;
    int i, len;
    struct sockaddr_in serv_addr, client_addr;
    char buf[4096], client_ip[128];
    socklen_t addr_len;
    
    //AF_INET:ipv4  SOCK_STREAM:流协议  0:默认协议(tcp,udp)
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    /* 绑定构造的服务器地址 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //网络字节序
    serv_addr.sin_port = htons(SERV_PORT);
    //INADDR_ANY主机所有ip
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //INADDR_ANY主机所有ip
    bind(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    /* 服务器能接受并发链接的能力 */
    listen(sfd, 128);

    printf("wait for connect ...\n");
    addr_len = sizeof(client_addr);
    /* 阻塞，等待客户端连接，成功则返回新的文件描述符，用于和客户端通信 */
    cfd = accept(sfd, (struct sockaddr*)&client_addr, &addr_len);
    printf("client IP:%s\t%d\n", 
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)),
            ntohs(client_addr.sin_port));

    while(1)
    {
        //阻塞接受客户端数据
        //客户端数据长度
        len = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);
        //处理业务
        for(i=0; i<len; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        //返回给客户端结果
        write(cfd, buf, len);
    }
    close(cfd);
    close(sfd);

    return 0;
}