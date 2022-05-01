#include "Editconn.h"

Editconn::Editconn(QTcpSocket *socket)
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
    */
    setWindowTitle("连接服务器");
    QVBoxLayout * VBox = new QVBoxLayout;
    this->socket = socket;

    QFormLayout *FormLayout = new QFormLayout;
    ip = new QLineEdit;
    port = new QLineEdit;
    ip->setPlaceholderText("X.X.X.X");
    port->setPlaceholderText("0~65535");
    FormLayout->addRow("服务器ip：",ip);
    FormLayout->addRow("服务器端口：",port);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    QGridLayout* HBox = new QGridLayout;
    submit = new QPushButton("连接");
    cancel = new QPushButton("退出");

    HBox->addWidget(submit,0,0);
    HBox->addWidget(cancel,0,1);

    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);

    //连接
    QObject::connect(submit,&QPushButton::clicked,this,&Editconn::cnct);

    //点击取消按钮时，关闭窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&Editconn::close);
}

void Editconn::cnct()
{
    socket->connectToHost(ip->text(), port->text().toInt());
    socket->waitForConnected(2000);
    if(!socket->waitForConnected(2000))
        QMessageBox::warning(this,"提示", "连接服务器失败",QMessageBox::Ok);
    else this->accept();
}

Editconn::~Editconn()
{
    delete ip;
    delete port;
    delete submit;
    delete cancel;
}
