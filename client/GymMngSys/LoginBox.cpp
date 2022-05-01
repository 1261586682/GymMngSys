#include "LoginBox.h"

//构建添加学生主界面
LoginBox::LoginBox(QTcpSocket *socket)
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
    */
    setWindowTitle("登录");
    QVBoxLayout * VBox = new QVBoxLayout;
    this->socket = socket;

    QFormLayout *FormLayout = new QFormLayout;
    gymid = new QLineEdit;
    password = new QLineEdit;
    gymid->setPlaceholderText("最长11位");
    password->setPlaceholderText("最长11位");
    password->setEchoMode(QLineEdit::Password);
    FormLayout->addRow("体育馆编号：",gymid);
    FormLayout->addRow("体育馆密码：",password);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    QGridLayout* HBox = new QGridLayout;
    submit = new QPushButton("登录");
    cancel = new QPushButton("退出");
    reg = new QPushButton("注册");
    HBox->addWidget(submit,0,0);
    HBox->addWidget(reg,0,1);
    HBox->addWidget(cancel,1,0,1,0);

    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);

    //注册
    QObject::connect(reg,&QPushButton::clicked,this,&LoginBox::add_Regbox);

    //登录
    QObject::connect(submit,&QPushButton::clicked,this,&LoginBox::Login);

    //点击取消按钮时，关闭窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&LoginBox::close);
    connect(socket,SIGNAL(readyRead()),this,SLOT(recv_Msg()));
}

bool LoginBox::check()
{
    if(!socket->waitForConnected())
    {
        QMessageBox::warning(this,"提示", "连接服务器失败",QMessageBox::Ok);
        return false;
    }
    return true;
}

//当用户添加提交按钮时，保存学生信息
void LoginBox::Login(){
      //确保所有信息填写后，将信息写入到文件中，否则提示用户将信息填写完整
      //加入到数据库
        if(this->gymid->text() !="" && this->password->text()!=""){
            if(!check_str(gymid->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(!check_str(password->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(gymid->text().length()>11 || password->text().length()>11)
            {
                QMessageBox::warning(this,"提示","请按要求填写信息",QMessageBox::Ok);
                return;
            }
            Proto::request req;
            req.set_function(func_name::Login);
            Proto::Gyminfo* info = new Proto::Gyminfo;
            info = req.add_login();
            std::vector<int>ve=StoI(gymid->text().toStdString());
            for(int i=0 ;i<ve.size();++i)
                info->add_gymid(ve[i]);
            ve=StoI(password->text().toStdString());
            for(int i=0 ;i<ve.size();++i)
                info->add_password(ve[i]);
            int len = (int)req.ByteSizeLong();
            QByteArray buf = QByteArray(len,0);
            req.SerializePartialToArray(buf.data(),len);
            buf.push_front((char)(len%256));
            buf.push_front((char)(len/256));
            socket->write(buf,buf.size());
            socket->flush();
        }else{
            QMessageBox::warning(this,"提示","请将信息填写完整",QMessageBox::Ok);
        }
}

void LoginBox::unpack(QByteArray buf)
{
    while(buf.size()>0)
    {
        int len = AtoI(buf[0])*256+AtoI(buf[1]);
        decode(buf.mid(2,len));
        buf=buf.right(buf.size()-len-2);
    }
}

void LoginBox::decode(QByteArray buf)
{
    Proto::response res;
    res.ParsePartialFromArray(buf.data(),buf.size());
    if(res.function()==func_name::Login)
    {
        if(res.res())
        {
            __gymid = this->gymid->text().toStdString();
            accept();
        }
        else QMessageBox::warning(this,"提示", "编号或密码错误",QMessageBox::Ok);
    }
}

void LoginBox::recv_Msg()
{
    while(socket->bytesAvailable())
    {
        QByteArray buf = socket->readAll();
        unpack(buf);
    }
}

LoginBox::~LoginBox()
{
    delete gymid;
    delete password;
    delete submit;
    delete cancel;
    delete reg;
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(recv_Msg()));
}

void LoginBox::add_Regbox()
{
      disconnect(socket,SIGNAL(readyRead()),this,SLOT(recv_Msg()));
      Regbox = new EditRegBox(socket);
      Regbox->exec();
      delete Regbox;
      connect(socket,SIGNAL(readyRead()),this,SLOT(recv_Msg()));
}
