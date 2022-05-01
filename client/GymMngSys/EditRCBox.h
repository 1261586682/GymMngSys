#ifndef EDITRCBOX_H
#define EDITRCBOX_H

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

class EditRCBox : public QDialog
{
    Q_OBJECT
public:
    //构建添加学生信息界面
    EditRCBox(QTcpSocket *socket);
    ~EditRCBox();
signals:
    //自定义信号，当添加学生信息完成后，更新主界面上的表格
    void saveBox();

public slots:
    //将充值的信息写入到文件中
    void saveMsg();
public:
    //发出 closeBox 信号
    void emitSaveBox();
private:
    QLineEdit* uid;
    QLineEdit* money;
    QPushButton* submit;
    QPushButton* cancel;
    QTcpSocket *socket;
};

#endif // EDITRCBOX_H
