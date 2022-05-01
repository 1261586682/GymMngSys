#include "Personnel.h"

QString Personnel::getid()const{
    return id;
}
void Personnel::setid(const QString &value){
    id = value;
}

qint8 Personnel::getgender()const{
    return gender;
}
void Personnel::setgender(const qint8 &value){
    gender = value;
}

qint32 Personnel::getage()const{
    return age;
}
void Personnel::setage(const qint32 &value){
    age = value;
}

QString Personnel::getname()const{
    return name;
}
void Personnel::setname(const QString &value){
    name = value;
}

qint32 Personnel::getmoney()const{
    return money;
}
void Personnel::setmoney(const qint32 &value){
    money = value;
}

QString Personnel::getst_time()const{
    return st_time;
}
void Personnel::setst_time(const QString &value){
    st_time = value;
}
