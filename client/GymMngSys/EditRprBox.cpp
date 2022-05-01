#include "EditRprBox.h"

//构建添加维修记录主界面
EditRprBox::EditRprBox(QTcpSocket *socket)
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
    */
    setWindowTitle("添加维修记录");
    QVBoxLayout * VBox = new QVBoxLayout;
    QFormLayout *FormLayout = new QFormLayout;
    this->socket = socket;

    time = new QLineEdit;
    reason = new QLineEdit;
    spend = new QLineEdit;
    uid = new QLineEdit;
    FormLayout->addRow("维修时间：",time);
    FormLayout->addRow("维修原因：",reason);
    FormLayout->addRow("维修经费：",spend);
    FormLayout->addRow("维修人员编号：",uid);
    time->setPlaceholderText("YYYY-MM-DD");
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QHBoxLayout * HBox = new QHBoxLayout;
    submit = new QPushButton("提交");
    cancel = new QPushButton("取消");
    HBox->addWidget(submit);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);

    //点击提交按钮时，将新学生信息保存到数据库中
    QObject::connect(submit,&QPushButton::clicked,this,&EditRprBox::saveMsg);
    //点击取消按钮时，关闭添加学生信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&EditRprBox::close);
}

//当用户添加提交按钮时，保存学生信息
void EditRprBox::saveMsg(){
    //确保所有信息填写后，将信息写入到数据库中，否则提示用户将信息填写完整
    //加入到数据库
      if(this->time->text() !="" && this->reason->text()!="" && this->spend->text()!="" && this->uid->text()!="" ){
          if(!check_str(time->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(!check_str(uid->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(!check_str(reason->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          if(time->text().length()!=10 || uid->text().length()>11 || reason->text().length()>100)
          {
              QMessageBox::warning(this,"提示","请按要求填写信息",QMessageBox::Ok);
              return;
          }
          Proto::request msg;
          Proto::Repairlog *log = new Proto::Repairlog;
          msg.set_function(func_name::AddRepairlog);
          log = msg.add_addlog();
          std::vector<int>ve = StoI(__gymid);
          for(int i=0;i<ve.size();++i)
              log->add_gymid(ve[i]);
          ve = StoI(time->text().toStdString());
          for(int i=0;i<ve.size();++i)
              log->add_time(ve[i]);
          ve = StoI(reason->text().toStdString());
          for(int i=0;i<ve.size();++i)
              log->add_reason(ve[i]);
          ve = StoI(uid->text().toStdString());
          for(int i=0;i<ve.size();++i)
              log->add_uid(ve[i]);
          log->set_spend(spend->text().toInt());
          int len = (int)msg.ByteSizeLong();
          QByteArray buf = QByteArray(len,0);
          msg.SerializePartialToArray(buf.data(),len);
          buf.push_front((char)(len%256));
          buf.push_front((char)(len/256));
          socket->write(buf,buf.size());
          socket->flush();
          this->close();
          emitSaveBox();
      }else{
          QMessageBox::warning(this,"提示","请将信息填写完整",QMessageBox::Ok);
      }
}

void EditRprBox::emitSaveBox(){
    emit saveBox();
}

EditRprBox::~EditRprBox()
{
    delete time;//YYYY-MM-DD
    delete reason;
    delete spend;
    delete uid;
    delete submit;
    delete cancel;
}
