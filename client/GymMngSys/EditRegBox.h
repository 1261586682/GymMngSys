#ifndef EDITREGBOX_H
#define EDITREGBOX_H

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
#include <QCheckBox>
#include "exsi.h"
#include "enum.h"
#include "Personnel.h"
#include "proto.pb.h"

class EditRegBox : public QDialog
{
    Q_OBJECT
public:
    EditRegBox(QTcpSocket*);
    ~EditRegBox();
public slots:
    void Register();
    void recv_Msg();
    void unpack(QByteArray buf);
    void decode(QByteArray buf);
private:
    QLineEdit* gymid;
    QLineEdit* gymname;
    QLineEdit* position;
    QLineEdit* builttime;
    QLineEdit* password;
    QCheckBox* fitness;
    QCheckBox* basketball;
    QCheckBox* badminton;
    QCheckBox* swimming;
    QCheckBox* bath;
    QCheckBox* contest;
    QPushButton* submit;
    QPushButton* cancel;
    QTcpSocket *socket;
};

#endif // EDITREGBOX_H
