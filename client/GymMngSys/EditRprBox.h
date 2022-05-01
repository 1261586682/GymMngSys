#ifndef EDITRPRBOX_H
#define EDITRPRBOX_H

#include <QDialog>
#include <QLineEdit>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QTcpSocket>
#include <QMessageBox>
#include "Repairlog.h"
#include "proto.pb.h"
#include "enum.h"
#include "exsi.h"

class EditRprBox : public QDialog
{
    Q_OBJECT
public:
    //构建添加学生信息界面
    EditRprBox(QTcpSocket *socket);
    ~EditRprBox();
signals:
    //自定义信号，当添加学生信息完成后，更新主界面上的表格
    void saveBox();
public slots:
    //将新学生的信息写入到文件中
    void saveMsg();
public:
    //发出 closeBox 信号
    void emitSaveBox();
private:
    QLineEdit* time;//YYYY-MM-DD
    QLineEdit* reason;
    QLineEdit* spend;
    QLineEdit* uid;
    QPushButton* submit;
    QPushButton* cancel;
    QTcpSocket *socket;
};


#endif // EDITRPRBOX_H
