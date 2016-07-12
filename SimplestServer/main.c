//
//  main.c
//  SimplestServer
//
//  Created by 李抗 on 7/10/16.
//  Copyright © 2016 李抗. All rights reserved.
//  用于测试backlog参数


#include "SimplestServer.h"
#define false 0
#define true 1
typedef int bool;
static int BUF_SIZE = 1024;
static bool stop = false;
static void handle_term (int sig)
{
    printf("using crtl-c to stop program.");
    stop = true;
}
int main(int argc, const char * argv[]) {
    signal(SIGTERM,handle_term);//处理ctl+c手动关闭请求
    if(argc<=3)
    {
        printf("usage: %s ip_address port_number backlog\n",
               basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    int sock = socket(PF_INET, SOCK_STREAM, 0);//声明socket文件描述符，返回类型为int
    assert(sock>=0);
    
    struct sockaddr_in address;//声明服务端socket地址
    bzero(&address, sizeof(address));//置0
    address.sin_family = AF_INET;//
    inet_pton(AF_INET,ip,&address.sin_addr);//将ip地址转换成网络字节序整数表示的ip地址，反之inet_ntop
    address.sin_port = htons(port);//host to network short
    
    int ret = bind(sock,
                   (const struct sockaddr*) &address,
                   sizeof(address));//bind socket和address
    assert(ret!=-1);//0表示成功，-1表示失败
    
    ret = listen(sock, backlog);//listen函数
    assert(ret!=-1);
    //sleep(10);
    while(!stop)
    {
        struct sockaddr_in client;//声明client socket地址
        socklen_t client_addrlength = sizeof(client);//socket长度
        int connfd = accept(sock,
                            (struct sockaddr*)&client,
                            &client_addrlength);//接收socket，放到client，阻塞调用
        if(connfd<0)
        {
            printf("error is:%d\n",errno);
        }
        else
        {
            /*char remote[INET_ADDRSTRLEN];
            printf("connected with ip: %s and port: %d\n",inet_ntop(AF_INET,
                                                                    &client.sin_addr,
                                                                    remote,
                                                                    INET_ADDRSTRLEN),
                   ntohs(client.sin_port));
            close(connfd);*/
            char buffer[BUF_SIZE];
            
            memset(buffer, '\0', BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE-1, 0);
            printf("got %d byte of normal data '%s'\n",ret,buffer);
            
            memset(buffer, '\0', BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE-1, MSG_OOB);
            printf("got %d byte of normal data '%s'\n",ret,buffer);
            
            memset(buffer, '\0', BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE-1, 0);
            printf("got %d byte of normal data '%s'\n",ret,buffer);
            close(connfd);
        }
    }
    close(sock);
    return 0;
}