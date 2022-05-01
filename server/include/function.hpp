#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <string>
#include <cstring>
#include "proto.pb.h"
#include "enum.h"
#include "conn.h"
#include "sqlpool.hpp"
#include "exsi.hpp"

#define debug(x) std::cout << "\"" << x << "\"= " << x << std::endl;

sqlpool *mysql = sqlpool::get_sqlpool();

void add_head(conn *buf_conn, int len)
{
    *(buf_conn->get_write_buf()) = (char)(len / 256);
    *(buf_conn->get_write_buf() + 1) = (char)(len % 256);
    buf_conn->add_write_tail(2);
}

void add_student(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request add person.");
    Proto::Student stu = msg->addstu(0);
    MYSQL *conn = mysql->get_sqlconn();
    std::string uid = AtoS(stu.uid());
    std::string name = AtoS(stu.name());
    std::string gymid = AtoS(msg->gymid());
    std::string query = "insert into personinfo(uid, name, gender, age , gymid) values('" + uid + "', '" + name + "', " + std::to_string(stu.gender()) + ", " + std::to_string(stu.age()) + ",'" + gymid + "');";
    mysql_query(conn, query.c_str());
    MYSQL_RES *res = mysql_store_result(conn);
    int rows = mysql_affected_rows(conn);
    mysql_free_result(res);
    mysql->append_sqlconn(conn);
    Proto::response *rsp = new Proto::response;
    std::cout << query << std::endl;
    rsp->set_function(func_name::AddStudent);
    if (rows > 0)
        rsp->set_res(true);
    else
        rsp->set_res(false);
    size_t len = rsp->ByteSizeLong();
    add_head(buf_conn, len);
    rsp->SerializePartialToArray(buf_conn->get_write_buf(), len);
    buf_conn->add_write_tail(len);
    delete rsp;
}

void flush_table(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request flush tables.");
    int name = msg->table_name();
    MYSQL *conn = mysql->get_sqlconn();
    Proto::response *rsp = new Proto::response;
    rsp->set_function(func_name::FlushTable);
    std::string gymid = AtoS(msg->gymid());
    if (name == table_name::PersonInfo || name == table_name::UseInfo)
    {
        rsp->set_table_name(name);
        std::string query = "select * from personinfo where gymid = '" + gymid + "';";
        std::cout << query << std::endl;
        mysql_query(conn, query.c_str());
        MYSQL_RES *res = mysql_store_result(conn);
        int rows = mysql_num_rows(res);
        if (rows > 0)
        {
            rsp->set_res(true);
            MYSQL_ROW row;
            while (row = mysql_fetch_row(res))
            {
                Proto::Student *stu = new Proto::Student;
                stu = rsp->add_stu();
                std::string uid = CtoS(row[0]);
                std::string name = CtoS(row[1]);
                bool gender = atoi(row[2]);
                int age = atoi(row[3]);
                std::string st_time = CtoS(row[4]);
                int balance = atoi(row[5]);
                std::vector<int> ve = StoI(uid);
                for (int i = 0; i < ve.size(); ++i)
                    stu->add_uid(ve[i]);
                ve = StoI(name);
                for (int i = 0; i < ve.size(); ++i)
                    stu->add_name(ve[i]);
                stu->set_gender(gender);
                stu->set_age(age);
                ve = StoI(st_time);
                for (int i = 0; i < ve.size(); ++i)
                    stu->add_st_time(ve[i]);
                stu->set_balance(balance);
            }
        }
        else
            rsp->set_res(false);
        mysql_free_result(res);
    }
    else if (name == table_name::RepairLog)
    {
        rsp->set_table_name(name);
        std::string query = "select * from repairlog where gymid = '" + gymid + "';";
        std::cout << query << std::endl;
        mysql_query(conn, query.c_str());
        MYSQL_RES *res = mysql_store_result(conn);
        int rows = mysql_num_rows(res);
        if (rows > 0)
        {
            rsp->set_res(true);
            MYSQL_ROW row;
            while (row = mysql_fetch_row(res))
            {
                Proto::Repairlog *Log = new Proto::Repairlog;
                Log = rsp->add_log();
                std::string st_time = CtoS(row[1]);
                std::string reason = CtoS(row[2]);
                int spend = atoi(row[3]);
                std::string uid = CtoS(row[4]);
                std::vector<int> ve = StoI(st_time);
                for (int i = 0; i < ve.size(); ++i)
                    Log->add_time(ve[i]);
                ve = StoI(reason);
                for (int i = 0; i < ve.size(); ++i)
                    Log->add_reason(ve[i]);
                Log->set_spend(spend);
                ve = StoI(uid);
                for (int i = 0; i < ve.size(); ++i)
                    Log->add_uid(ve[i]);
            }
        }
        else
            rsp->set_res(false);
        mysql_free_result(res);
    }
    else if (name == table_name::GymInfo)
    {
        rsp->set_table_name(name);
        std::string query = "select * from gyminfo where gymid = '" + gymid + "';";
        std::cout << query << std::endl;
        mysql_query(conn, query.c_str());
        Proto::Gyminfo *Info = new Proto::Gyminfo;
        Info = rsp->add_gyminfo();
        MYSQL_RES *res = mysql_store_result(conn);
        int rows = mysql_num_rows(res);
        if (rows > 0)
        {
            rsp->set_res(true);
            MYSQL_ROW row;
            while (row = mysql_fetch_row(res))
            {
                std::string gymid = CtoS(row[0]);
                std::string gymname = CtoS(row[1]);
                std::string position = CtoS(row[2]);
                std::string builttime = CtoS(row[3]);
                int profit = atoi(row[4]);
                std::string password = CtoS(row[5]);
                int uprice = atoi(row[6]);
                std::vector<int> ve = StoI(gymid);
                for (int i = 0; i < ve.size(); ++i)
                    Info->add_gymid(ve[i]);
                ve = StoI(gymname);
                for (int i = 0; i < ve.size(); ++i)
                    Info->add_gymname(ve[i]);
                ve = StoI(position);
                for (int i = 0; i < ve.size(); ++i)
                    Info->add_position(ve[i]);
                ve = StoI(builttime);
                for (int i = 0; i < ve.size(); ++i)
                    Info->add_builttime(ve[i]);
                ve = StoI(password);
                for (int i = 0; i < ve.size(); ++i)
                    Info->add_password(ve[i]);
                Info->set_profit(profit);
                Info->set_uprice(uprice);
            }
            mysql_free_result(res);

            std::string query = "select * from features where gymid = '" + gymid + "';";
            std::cout << query << std::endl;
            mysql_query(conn, query.c_str());
            res = mysql_store_result(conn);
            int rows = mysql_num_rows(res);
            if (rows > 0)
            {
                rsp->set_res(true);
                MYSQL_ROW row;
                while (row = mysql_fetch_row(res))
                {
                    bool fitness = atoi(row[1]);
                    bool basketball = atoi(row[2]);
                    bool badminton = atoi(row[3]);
                    bool swimming = atoi(row[4]);
                    bool bath = atoi(row[5]);
                    bool contest = atoi(row[6]);
                    Info->set_fitness(fitness);
                    Info->set_basketball(basketball);
                    Info->set_badminton(badminton);
                    Info->set_swimming(swimming);
                    Info->set_bath(bath);
                    Info->set_contest(contest);
                }
            }
            else
                rsp->set_res(false);
        }
        mysql_free_result(res);
    }
    size_t len = rsp->ByteSizeLong();
    add_head(buf_conn, len);
    rsp->SerializePartialToArray(buf_conn->get_write_buf(), len);
    buf_conn->add_write_tail(len);
    delete rsp;
    mysql->append_sqlconn(conn);
}

void add_repairlog(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request add repairlog.");
    Proto::Repairlog log = msg->addlog(0);
    MYSQL *conn = mysql->get_sqlconn();
    std::string gymid = AtoS(log.gymid());
    std::string time = AtoS(log.time());
    std::string reason = AtoS(log.reason());
    std::string uid = AtoS(log.uid());
    int spend = log.spend();
    Proto::response *rsp = new Proto::response;
    rsp->set_function(func_name::AddRepairlog);
    std::string query = "select * from personinfo where uid = '" + uid + "' and gymid = '" + gymid + "';";
    mysql_query(conn, query.c_str());
    std::cout << query << std::endl;
    MYSQL_RES *res = mysql_store_result(conn);
    int rows = mysql_num_rows(res);
    mysql_free_result(res);
    if (rows <= 0)
        rsp->set_res(false);
    else
    {
        std::string query = "insert into repairlog(gymid,time,reason,spend,uid)values('" + gymid + "','" + time + "','" + reason + "'," + std::to_string(spend) + ",'" + uid + "');";
        mysql_query(conn, query.c_str());
        std::cout << query << std::endl;
        res = mysql_store_result(conn);
        int rows = mysql_affected_rows(conn);
        mysql_free_result(res);
        query = "update gyminfo set profit = profit - " + std::to_string(spend) + ";";
        mysql_query(conn, query.c_str());
        std::cout << query << std::endl;
        if (rows > 0)
            rsp->set_res(true);
        else
            rsp->set_res(false);
    }
    mysql->append_sqlconn(conn);
    size_t len = rsp->ByteSizeLong();
    std::cout << "The length of rps = " << len << std::endl;
    add_head(buf_conn, len);
    rsp->SerializePartialToArray(buf_conn->get_write_buf(), len);
    buf_conn->add_write_tail(len);
    delete rsp;
}

void del_student(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request delete person.");
    int len = msg->delstu().size();
    MYSQL *conn = mysql->get_sqlconn();
    for (int i = 0; i < len; ++i)
    {
        std::string uid = AtoS(msg->delstu(i).uid());
        std::string gymid = AtoS(msg->gymid());
        std::string query = "delete from personinfo where uid = '" + uid + "' and gymid = '" + gymid + "';";
        mysql_query(conn, query.c_str());
        std::cout << query << std::endl;
    }
    mysql->append_sqlconn(conn);
}

void del_repairlog(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request delete repairlog.");
    int len = msg->dellog().size();
    MYSQL *conn = mysql->get_sqlconn();
    for (int i = 0; i < len; ++i)
    {
        std::string gymid = AtoS(msg->dellog(i).gymid());
        std::string time = AtoS(msg->dellog(i).time());
        std::string reason = AtoS(msg->dellog(i).reason());
        std::string uid = AtoS(msg->dellog(i).uid());
        int spend = msg->dellog(i).spend();
        std::string query = "delete from repairlog where gymid = '" + gymid + "' and time = '" + time + "' and reason = '" + reason + "' and spend =" + std::to_string(spend) + " and uid ='" + uid + "';";
        mysql_query(conn, query.c_str());
        std::cout << query << std::endl;
    }
    mysql->append_sqlconn(conn);
}

void student_in(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request add person into gym.");
    int len = msg->in().size();
    MYSQL *conn = mysql->get_sqlconn();
    for (int i = 0; i < len; ++i)
    {
        std::string uid = AtoS(msg->in(i).uid());
        std::string gymid = AtoS(msg->gymid());
        std::string query = "select balance from personinfo where uid = '" + uid + "' and gymid = '" + gymid + "';";
        mysql_query(conn, query.c_str());
        MYSQL_RES *res = mysql_store_result(conn);
        int rows = mysql_num_rows(res);
        if (rows > 0)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            int balance = atoi(row[0]);
            if (balance > 0)
            {
                query = "update personinfo set st_time = NOW() where uid = '" + uid + "' and gymid = '" + gymid + "';";
                mysql_query(conn, query.c_str());
                std::cout << query << std::endl;
            }
        }
        mysql_free_result(res);
    }
    mysql->append_sqlconn(conn);
}

void student_out(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request add person out gym.");
    int len = msg->out().size();
    MYSQL *conn = mysql->get_sqlconn();
    for (int i = 0; i < len; ++i)
    {
        std::string uid = AtoS(msg->out(i).uid());
        std::string gymid = AtoS(msg->gymid());
        int uprice = 0;
        std::string query = "select uprice from gyminfo where gymid = '" + gymid + "';";
        mysql_query(conn, query.c_str());
        std::cout << query << std::endl;
        MYSQL_RES *res = mysql_store_result(conn);
        int rs = mysql_num_rows(res);
        if (rs > 0)
        {   
            MYSQL_ROW row = mysql_fetch_row(res);
            uprice = atoi(row[0]);
        }
        mysql_free_result(res);
        query = "select timestampdiff(hour,st_time,NOW()) from personinfo where uid = '" + uid + "' and gymid = '" + gymid + "'";
        mysql_query(conn, query.c_str());
        res = mysql_store_result(conn);
        int rows = mysql_num_rows(res);
        if (rows > 0)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            int t = atoi(row[0]) + 1;
            query = "update personinfo set balance = balance - " + std::to_string(uprice * t) + " ,st_time = NULL where uid = '" + uid + "' and gymid = '" + gymid + "'";
            mysql_query(conn, query.c_str());
            std::cout << query << std::endl;
        }
        mysql_free_result(res);
    }
    mysql->append_sqlconn(conn);
}

void change_stu(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request update person.");
    MYSQL *conn = mysql->get_sqlconn();
    Proto::ChangePerson stu = msg->changestu(0);
    std::string gymid = AtoS(msg->gymid());
    std::string uid = AtoS(stu.uid());
    std::string column = AtoS(stu.column());
    std::string value = AtoS(stu.value());
    std::string query;
    if (column != "balance")
        query = "update personinfo set " + column + " = '" + value + "' where uid = '" + uid + "' and gymid = '" + gymid + "';";
    else
        query = "update personinfo set balance = balance + '" + value + "' where uid = '" + uid + "' and gymid = '" + gymid + "';";
    mysql_query(conn, query.c_str());
    std::cout << query << std::endl;
    MYSQL_RES *res = mysql_store_result(conn);
    int rows = mysql_affected_rows(conn);
    mysql_free_result(res);
    if (column == "balance")
        query = "update gyminfo set profit = profit + '" + value + "' where gymid = '" + gymid + "';";
    mysql_query(conn, query.c_str());
    std::cout << query << std::endl;
    Proto::response *rsp = new Proto::response;
    rsp->set_function(func_name::ChangePerson);
    if (rows > 0)
        rsp->set_res(true);
    else
        rsp->set_res(false);
    size_t len = rsp->ByteSizeLong();
    add_head(buf_conn, len);
    rsp->SerializePartialToArray(buf_conn->get_write_buf(), len);
    buf_conn->add_write_tail(len);
    mysql->append_sqlconn(conn);
    delete rsp;
}

void login(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request login.");
    Proto::Gyminfo info = msg->login(0);
    Proto::response rsp;
    rsp.set_function(func_name::Login);
    std::string gymid = AtoS(info.gymid());
    std::string password = AtoS(info.password());
    MYSQL *conn = mysql->get_sqlconn();
    std::string query = "select * from gyminfo where gymid = '" + gymid + "' and password = '" + password + "';";
    mysql_query(conn, query.c_str());
    std::cout << query << std::endl;
    MYSQL_RES *res = mysql_store_result(conn);
    int rows = mysql_num_rows(res);
    mysql_free_result(res);
    if (rows > 0)
        rsp.set_res(true);
    else
        rsp.set_res(false);
    size_t len = rsp.ByteSizeLong();
    add_head(buf_conn, len);
    rsp.SerializePartialToArray(buf_conn->get_write_buf(), len);
    buf_conn->add_write_tail(len);
    mysql->append_sqlconn(conn);
}

void reg(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request register.");
    Proto::Gyminfo info = msg->reg(0);
    Proto::response rsp;
    rsp.set_function(func_name::Register);
    std::string gymid = AtoS(info.gymid());
    std::string gymname = AtoS(info.gymname());
    std::string position = AtoS(info.position());
    std::string builttime = AtoS(info.builttime());
    std::string password = AtoS(info.password());
    bool fitness = info.fitness();
    bool basketball = info.basketball();
    bool badminton = info.badminton();
    bool swimming = info.swimming();
    bool bath = info.bath();
    bool contest = info.contest();
    MYSQL *conn = mysql->get_sqlconn();
    std::string query = "insert into gyminfo(gymid,gymname,position,builttime,profit,password)values('" + gymid + "','" + gymname + "','" + position + "','" + builttime + "',0,'" + password + "');";
    mysql_query(conn, query.c_str());
    std::cout << query << std::endl;
    MYSQL_RES *res = mysql_store_result(conn);
    int rows = mysql_affected_rows(conn);
    mysql_free_result(res);
    if (rows > 0)
    {
        query = "insert into features(gymid,fitness,basketball,badminton,swimming,bath,contest)values('" + gymid + "'," + std::to_string(fitness) + "," + std::to_string(basketball) + "," + std::to_string(badminton) + "," + std::to_string(swimming) + "," + std::to_string(bath) + "," + std::to_string(contest) + ");";
        mysql_query(conn, query.c_str());
        std::cout << query << std::endl;
        rsp.set_res(true);
    }
    else
        rsp.set_res(false);
    size_t len = rsp.ByteSizeLong();
    add_head(buf_conn, len);
    rsp.SerializePartialToArray(buf_conn->get_write_buf(), len);
    buf_conn->add_write_tail(len);
    mysql->append_sqlconn(conn);
}

void set_uprice(Proto::request *msg, conn *buf_conn)
{
    buf_conn->show("request set uprice.");
    Proto::SetPrice price = msg->setprice(0);
    Proto::response rsp;
    rsp.set_function(func_name::SetUprice);
    std::string gymid = AtoS(price.gymid());
    int p = price.uprice();
    MYSQL *conn = mysql->get_sqlconn();
    std::string query = "update gyminfo set uprice = '" + std::to_string(p) + "' where gymid = '" + gymid + "';";
    mysql_query(conn, query.c_str());
    std::cout << query << std::endl;
    MYSQL_RES *res = mysql_store_result(conn);
    int rows = mysql_affected_rows(conn);
    mysql_free_result(res);
    if (rows > 0)
        rsp.set_res(true);
    else
        rsp.set_res(false);
    size_t len = rsp.ByteSizeLong();
    add_head(buf_conn, len);
    rsp.SerializePartialToArray(buf_conn->get_write_buf(), len);
    buf_conn->add_write_tail(len);
    mysql->append_sqlconn(conn);
}

#endif