#include "EditRegBox.h"

//构建添加学生主界面
EditRegBox::EditRegBox(QTcpSocket *socket)
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
    */
    setWindowTitle("注册");
    QVBoxLayout * VBox = new QVBoxLayout;
    this->socket = socket;

    QFormLayout *FormLayout = new QFormLayout;
    gymid = new QLineEdit;
    gymname = new QLineEdit;
    position = new QLineEdit;
    builttime = new QLineEdit;
    password = new QLineEdit;
    fitness = new QCheckBox;
    basketball = new QCheckBox;
    badminton = new QCheckBox;
    swimming = new QCheckBox;
    bath = new QCheckBox;
    contest = new QCheckBox;
    gymid->setPlaceholderText("最长11位");
    password->setPlaceholderText("最长11位");
    gymname->setPlaceholderText("最长11位");
    position->setPlaceholderText("最长11位");
    builttime->setPlaceholderText("XXXX-XX-XX");

    FormLayout->addRow("体育馆编号：",gymid);
    FormLayout->addRow("体育馆名字：",gymname);
    FormLayout->addRow("体育馆地点：",position);
    FormLayout->addRow("体育馆建立时间：",builttime);
    FormLayout->addRow("体育馆密码：\t",password);
    FormLayout->addRow("是否支持健身：\t",fitness);
    FormLayout->addRow("是否支持篮球：\t",basketball);
    FormLayout->addRow("是否支持羽毛球：\t",badminton);
    FormLayout->addRow("是否支持游泳：\t",swimming);
    FormLayout->addRow("是否有浴室：\t",bath);
    FormLayout->addRow("是否支持比赛：\t",contest);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QGridLayout* HBox = new QGridLayout;
    submit = new QPushButton("提交");
    cancel = new QPushButton("取消");
    HBox->addWidget(submit,0,0);
    HBox->addWidget(cancel,0,1);

    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);

//    //点击提交按钮时，将新学生信息保存到文件中
    QObject::connect(submit,&QPushButton::clicked,this,&EditRegBox::Register);
//    //点击取消按钮时，关闭添加学生信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&EditRegBox::close);
    connect(socket,SIGNAL(readyRead()),this,SLOT(recv_Msg()));
}

void EditRegBox::Register()
{
    //确保所有信息填写后，将信息写入到文件中，否则提示用户将信息填写完整
    //加入到数据库
      if(this->gymid->text() !="" && this->gymname->text() !="" && this->position->text() !="" && this->builttime->text() !="" && this->password->text() !=""){
          if(!check_str(gymid->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(!check_str(gymname->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(!check_str(position->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(!check_str(builttime->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(!check_str(password->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(gymid->text().length()>11 || password->text().length()>11 || position->text().length()>11 || builttime->text().length()>11 || gymname->text().length()>11)
          {
              QMessageBox::warning(this,"提示","请按要求填写信息",QMessageBox::Ok);
              return;
          }
          bool fitness_ = this->fitness->isChecked();
          bool basketball_ = this->basketball->isChecked();
          bool badminton_ = this->badminton->isChecked();
          bool swimming_ = this->swimming->isChecked();
          bool bath_ = this->bath->isChecked();
          bool contest_ = this->contest->isChecked();
          Proto::request req;
          req.set_function(func_name::Register);
          Proto::Gyminfo* info = new Proto::Gyminfo;
          info = req.add_reg();
          std::vector<int>ve=StoI(gymid->text().toStdString());
          for(int i=0 ;i<ve.size();++i)
              info->add_gymid(ve[i]);
          ve=StoI(gymname->text().toStdString());
          for(int i=0 ;i<ve.size();++i)
              info->add_gymname(ve[i]);
          ve=StoI(position->text().toStdString());
          for(int i=0 ;i<ve.size();++i)
              info->add_position(ve[i]);
          ve=StoI(builttime->text().toStdString());
          for(int i=0 ;i<ve.size();++i)
              info->add_builttime(ve[i]);
          ve=StoI(password->text().toStdString());
          for(int i=0 ;i<ve.size();++i)
              info->add_password(ve[i]);
          info->set_fitness(fitness_);
          info->set_basketball(basketball_);
          info->set_badminton(badminton_);
          info->set_swimming(swimming_);
          info->set_bath(bath_);
          info->set_contest(contest_);
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

void EditRegBox::unpack(QByteArray buf)
{
    while(buf.size()>0)
    {
        int len = AtoI(buf[0])*256+AtoI(buf[1]);
        decode(buf.mid(2,len));
        buf=buf.right(buf.size()-len-2);
    }
}

void EditRegBox::decode(QByteArray buf)
{
    Proto::response res;
    res.ParsePartialFromArray(buf.data(),buf.size());
    if(res.function()== func_name::Register)
    {
        if(res.res())
        {
            QMessageBox::warning(this,"提示", "注册成功",QMessageBox::Ok);
            this->close();
        }
        else QMessageBox::warning(this,"提示", "注册失败",QMessageBox::Ok);
    }
}

void EditRegBox::recv_Msg()
{
    while(socket->bytesAvailable())
    {
        QByteArray buf = socket->readAll();
        unpack(buf);
    }
}

EditRegBox::~EditRegBox()
{
    delete gymid;
    delete gymname;
    delete position;
    delete builttime;
    delete password;
    delete fitness;
    delete basketball;
    delete badminton;
    delete swimming;
    delete bath;
    delete contest;
    delete submit;
    delete cancel;
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(recv_Msg()));
}

