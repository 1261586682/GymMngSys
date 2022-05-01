#ifndef EDITUPRICE_H
#define EDITUPRICE_H

#include <QDialog>
#include <QLineEdit>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTcpSocket>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include "exsi.h"
#include "proto.pb.h"
#include "enum.h"

class EditUprice : public QDialog
{
    Q_OBJECT
public:
    EditUprice(QTcpSocket *socket);
    ~EditUprice();
signals:
    //自定义信号，当添加学生信息完成后，更新主界面上的表格
    void saveBox();
public:
    //发出 closeBox 信号
    void emitSaveBox();
public slots:
    void set();
private:
    QLineEdit* price;
    QPushButton* submit;
    QPushButton* cancel;
    QTcpSocket* socket;
};

#endif // EDITUPRICE_H
