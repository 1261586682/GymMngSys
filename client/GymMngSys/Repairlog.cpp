#include "Repairlog.h"

QString Repairlog::gettime()const{
    return time;
}
void Repairlog::settime(const QString &value){
    time = value;
}

QString Repairlog::getreason()const{
    return reason;
}
void Repairlog::setreason(const QString &value){
    reason = value;
}

qint32 Repairlog::getspend()const{
    return spend;
}
void Repairlog::setspend(const qint32 &value){
    spend = value;
}

QString Repairlog::getuid()const{
    return uid;
}
void Repairlog::setuid(const QString &value){
    uid = value;
}

