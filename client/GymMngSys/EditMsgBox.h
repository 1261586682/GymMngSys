#ifndef EDITMSGBOX_H
#define EDITMSGBOX_H

#include <QDialog>
#include <QLineEdit>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include "exsi.h"
#include "enum.h"
#include "Personnel.h"
#include "proto.pb.h"

class EditMsgBox : public QDialog
{
    Q_OBJECT
public:
    //构建添加学生信息界面
    EditMsgBox(QTcpSocket *socket);
    ~EditMsgBox();
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
    QLineEdit* id;
    QLineEdit* sex;
    QLineEdit* age;
    QLineEdit* name;
    QPushButton* submit;
    QPushButton* cancel;
    QTcpSocket *socket;
};

#endif // EDITMSGBOX_H
