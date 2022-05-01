#ifndef PERSONNEL_H
#define PERSONNEL_H

#include <QString>
class Personnel
{
public:
    Personnel(){}
    Personnel(QString id,qint8 gender,qint32 age,QString name,qint32 money,QString st_time):id(id),gender(gender),age(age),name(name),money(money),st_time(st_time){}

    QString getid()const;
    void setid(const QString &value);

    qint8 getgender()const;
    void setgender(const qint8 &value);

    qint32 getage()const;
    void setage(const qint32 &value);

    QString getname()const;
    void setname(const QString &value);

    qint32 getmoney()const;
    void setmoney(const qint32 &value);

    QString getst_time()const;
    void setst_time(const QString &value);

private:
    QString id;
    qint8 gender;
    qint32 age;
    QString name;
    qint32 money;
    QString st_time;
};

#endif // PERSONNEL_H
