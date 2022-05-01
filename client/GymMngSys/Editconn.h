#ifndef EDITCONN_H
#define EDITCONN_H

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

class Editconn : public QDialog
{
    Q_OBJECT
public:
    Editconn(QTcpSocket *socket);
    ~Editconn();
public slots:
    void cnct();
private:
    QLineEdit* ip;
    QLineEdit* port;
    QPushButton* submit;
    QPushButton* cancel;
    QTcpSocket* socket;
};

#endif // EDITCONN_H
