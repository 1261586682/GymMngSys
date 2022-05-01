#ifndef REPAIRLOG_H
#define REPAIRLOG_H

#include <QString>
class Repairlog
{
public:
    Repairlog(){}
    Repairlog(QString t,QString r,qint32 spend,QString uid):time(t),reason(r),spend(spend),uid(uid){}

    QString gettime()const;
    void settime(const QString &value);

    QString getreason()const;
    void setreason(const QString &value);

    qint32 getspend()const;
    void setspend(const qint32 &value);

    QString getuid()const;
    void setuid(const QString &value);

private:
    QString time;//YYYY-MM-DD
    QString reason;
    qint32 spend;
    QString uid;
};

#endif // REPAIRLOG_H
