#ifndef CONN_H
#define CONN_H

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <string>
#include <errno.h>
#include <sys/uio.h>
#include <iostream>
#include "locker.hpp"
#include "threadpool.hpp"

class conn
{
public:
    static const int READ_BUFFER_SIZE = 65536;  // 读缓冲区的大小
    static const int WRITE_BUFFER_SIZE = 65536; // 写缓冲区的大小

public:
    conn() {}
    ~conn() {}
    void init(int sockfd, const sockaddr_in &addr); // 初始化新接受的连接
    void close_conn();                              // 关闭连接
    bool task(task_type);                           //根据event判断进行的操作
    bool read();                                    // 非阻塞读
    bool write();                                   // 非阻塞写
    bool process();                                 // 处理请求
    char *get_write_buf();
    void add_write_tail(int);
    void show(std::string);
    static int epollfd;                           // 所有socket上的事件都被注册到同一个epoll内核事件中，所以设置成静态的
    static int user_count;                        // 统计用户的数量

private:
    void init();  // 初始化连接
    int sockfd; // 该TCP连接的socketfd
    sockaddr_in address;
    char read_buf[READ_BUFFER_SIZE];   // 读缓冲区
    char write_buf[WRITE_BUFFER_SIZE]; // 写缓冲区
    int read_head, read_tail;         //读指针
    int write_head, write_tail;       //写指针
};

#endif