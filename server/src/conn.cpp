#include "conn.h"
#include "proto.pb.h"
#include "enum.h"
#include "function.hpp"

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// 向epoll中添加需要监听的文件描述符
void addfd(int epollfd, int fd, bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP | EPOLLERR;
    if (one_shot)
    {
        // 防止同一个通信被不同的线程处理
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    // 设置文件描述符非阻塞
    setnonblocking(fd);
}

// 从epoll中移除监听的文件描述符
void removefd(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

// 修改文件描述符，重置socket上的EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN事件能被触发
void modfd(int epollfd, int fd, int ev)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP | EPOLLHUP | EPOLLERR;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

// 所有的客户数
int conn::user_count = 0;
// 所有socket上的事件都被注册到同一个epoll内核事件中，所以设置成静态的
int conn::epollfd = -1;

// 初始化连接,外部调用初始化套接字地址
void conn::init(int fd, const sockaddr_in &addr)
{
    sockfd = fd;
    address = addr;

    // 端口复用
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    addfd(epollfd, sockfd, true);
    user_count++;
    init();
    printf("ip:%s\tfd:%d\tconnected\n", inet_ntoa(address.sin_addr), sockfd);
}

void conn::init()
{
    bzero(read_buf, READ_BUFFER_SIZE);
    bzero(write_buf, READ_BUFFER_SIZE);
    read_head = read_tail = 0;
    write_head = write_tail = 0;
}

char *conn::get_write_buf()
{
    return write_buf + write_tail;
}

void conn::add_write_tail(int len)
{
    write_tail += len;
}

void conn::close_conn()
{
    if (sockfd != -1)
    {
        printf("ip:%s\tfd:%d\tdisconnected\n", inet_ntoa(address.sin_addr), sockfd);
        removefd(epollfd, sockfd);
        sockfd = -1;
        user_count--; // 关闭一个连接，将客户总数量-1
    }
}

bool conn::task(task_type event)
{
    bool res = true;
    switch (event)
    {
    case READ:
        res = read();
        break;
    case WRITE:
        res = write();
        break;
    default:
        res = true;
        break;
    }
    return res;
}

// 循环读取客户数据，直到无数据可读或者对方关闭连接
bool conn::read()
{
    while (true)
    {
        int len = recv(sockfd, read_buf + read_tail, READ_BUFFER_SIZE, 0);
        if (len == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) //没有数据
                break;
            else
                return false;
        }
        else if (len == 0)
            return false; //对方关闭连接
        read_tail += len;
        printf("Have read %d Bytes\n", len);
    }
    bool res = process();
    modfd(epollfd, sockfd, EPOLLOUT);
    return true && res;
}

bool conn::process()
{
    while (read_head < read_tail)
    {
        int len = AtoI(read_buf[read_head]) * 256 + AtoI(read_buf[read_head + 1]);
        read_head += 2;
        Proto::request *msg = new Proto::request;
        msg->ParsePartialFromArray(read_buf + read_head, len);
        int func = msg->function();
        int table_name = msg->table_name();
        try
        {
            switch (func)
            {
            case func_name::AddStudent:
                add_student(msg, this);
                break;
            case func_name::FlushTable:
                flush_table(msg, this);
                break;
            case func_name::AddRepairlog:
                add_repairlog(msg, this);
                break;
            case func_name::DelStudent:
                del_student(msg, this);
                break;
            case func_name::DelRepairlog:
                del_repairlog(msg, this);
                break;
            case func_name::StudentIn:
                student_in(msg, this);
                break;
            case func_name::StudentOut:
                student_out(msg, this);
                break;
            case func_name::ChangePerson:
                change_stu(msg, this);
                break;
            case func_name::Login:
                login(msg, this);
                break;
            case func_name::Register:
                reg(msg, this);
                break;
            case func_name::SetUprice:
                set_uprice(msg, this);
                break;
            default:
                break;
            }
        }
        catch (...)
        {
            printf("Error: Unknown Message format!\n");
            return false;
        }
        read_head += msg->ByteSizeLong();
        printf("Have solved %d Bytes\n", len + 2);
        delete msg;
    }
    read_head = read_tail = 0;
    return true;
}

bool conn::write()
{
    while (write_tail > write_head)
    {
        int len = send(sockfd, write_buf + write_head, write_tail - write_head, 0);
        if (len == 0)
        {
            //写完了
            break;
        }
        else if (len < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) //发送缓冲区满
                return true;
            else if (errno == EPIPE) //断开连接
                return false;
        }
        else
            write_head += len;
        printf("Have send %d bytes\n", len);
    }

    write_head = write_tail = 0;
    modfd(epollfd, sockfd, EPOLLIN);
    return true;
}

void conn::show(std::string s)
{
    printf("ip:%s\tfd:%d\t%s\n", inet_ntoa(address.sin_addr), sockfd,s.c_str());
}