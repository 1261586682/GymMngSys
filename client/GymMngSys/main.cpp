#include "MainWidget.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QTcpSocket>
#include <QFile>
#include "LoginBox.h"
#include "Editconn.h"
#include "enum.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTcpSocket *socket = new QTcpSocket();

    Editconn* conn = new Editconn(socket);
    if(!conn->exec()) return 0;
    delete conn;

    LoginBox *login = new LoginBox(socket);
    if (login->check()&& login->exec() == QDialog::Accepted)
    {
        delete login;
        MainWidget w(socket);
        if (w.check())
        {
            w.show();
            a.exec();
        }
    }
    return 0;
}
