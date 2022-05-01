#include <mysql/mysql.h>
#include <list>
#include <iostream>
#include <exception>
#include "locker.hpp"

class sqlpool
{
public:
    ~sqlpool();
    MYSQL *get_sqlconn();
    void append_sqlconn(MYSQL *);
    static sqlpool *get_sqlpool();

private:
    sqlpool(int connect_number = 8);
    sqlpool(const sqlpool &) = delete;
    int connect_number;
    std::list<MYSQL *> sqlqueue;
    locker sqlqueuelocker;
    sem sqlqueuesem;
    static sqlpool pool;
};
sqlpool sqlpool::pool;

sqlpool *sqlpool::get_sqlpool()
{
    return &pool;
}

sqlpool::sqlpool(int connect_number) : connect_number(connect_number), sqlqueuesem(connect_number)
{
    if (connect_number <= 0)
        throw std::exception();
    for (int i = 0; i < connect_number; ++i)
    {
        MYSQL *mysql = new MYSQL;
        mysql_init(mysql);
        if (!mysql_real_connect(mysql, "rm-2zegwtj04xa28oh1zqo.mysql.rds.aliyuncs.com", "root", "Ll20000101", "gymmngsys", 3306, 0, 0))
        {
            std::cout << mysql_error(mysql) << std::endl;
            throw std::exception();
        }
        else
            printf("create the %dth sqlconnection\n", i + 1);
        sqlqueue.push_back(mysql);
    }
}

MYSQL *sqlpool::get_sqlconn()
{
    sqlqueuesem.wait();
    sqlqueuelocker.lock();
    if (sqlqueue.size() >= 0)
    {
        MYSQL *conn = sqlqueue.front();
        sqlqueue.pop_front();
        sqlqueuelocker.unlock();
        return conn;
    }
    sqlqueuelocker.unlock();
    return nullptr;
}

void sqlpool::append_sqlconn(MYSQL *conn)
{
    sqlqueuelocker.lock();
    if (conn != nullptr)
    {
        sqlqueue.push_back(conn);
        sqlqueuesem.post();
    }
    sqlqueuelocker.unlock();
}

sqlpool::~sqlpool()
{
    while (!sqlqueue.empty())
    {
        MYSQL *mysql = sqlqueue.front();
        sqlqueue.pop_front();
        mysql_close(mysql);
        delete mysql;
    }
}