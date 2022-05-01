#ifndef LOGINBOX_H
#define LOGINBOX_H

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
#include "enum.h"
#include "Personnel.h"
#include "proto.pb.h"
#include "EditRegBox.h"

class LoginBox : public QDialog
{
    Q_OBJECT
public:
    LoginBox(QTcpSocket *socket);
    ~LoginBox();
public slots:
    void Login();
    void recv_Msg();
    void unpack(QByteArray buf);
    void decode(QByteArray buf);
    void add_Regbox();
    bool check();
private:
    QLineEdit* gymid;
    QLineEdit* password;
    QPushButton* submit;
    QPushButton* cancel;
    QPushButton* reg;
    EditRegBox* Regbox;
    QTcpSocket* socket;
};

#endif // LOGINBOX_H
