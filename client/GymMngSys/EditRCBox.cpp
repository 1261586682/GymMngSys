#include "EditRCBox.h"

//构建添加学生主界面
EditRCBox::EditRCBox(QTcpSocket *socket)
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
    */
    setWindowTitle("充值");
    QVBoxLayout * VBox = new QVBoxLayout;
    this->socket = socket;

    QFormLayout *FormLayout = new QFormLayout;
    uid = new QLineEdit;
    money = new QLineEdit;
    money->setPlaceholderText("1~10000");
    FormLayout->addRow("人员编号：",uid);
    FormLayout->addRow("金额：",money);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QHBoxLayout * HBox = new QHBoxLayout;
    submit = new QPushButton("提交");
    cancel = new QPushButton("取消");
    HBox->addWidget(submit);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);

    //点击提交按钮时，将新学生信息保存到文件中
    QObject::connect(submit,&QPushButton::clicked,this,&EditRCBox::saveMsg);
    //点击取消按钮时，关闭添加学生信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&EditRCBox::close);
}

//当用户添加提交按钮时，保存学生信息
void EditRCBox::saveMsg(){
      //确保所有信息填写后，将信息写入到文件中，否则提示用户将信息填写完整
      //加入到数据库
        if(this->uid->text() !="" && this->money->text()!=""){
            if(!check_str(uid->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(!check_str(money->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(uid->text().length()>11 || money->text().toInt()<=0 || money->text().toInt()>10000)
            {
                QMessageBox::warning(this,"提示","请按要求填写信息",QMessageBox::Ok);
                return;
            }
            Proto::request req;
            req.set_function(func_name::ChangePerson);
            Proto::ChangePerson *p = new Proto::ChangePerson;
            p = req.add_changestu();
            std::vector<int>ve=StoI(uid->text().toStdString());
            for(int i=0 ;i<ve.size();++i)
                p->add_uid(ve[i]);
            ve=StoI("balance");
            for(int i=0 ;i<ve.size();++i)
                p->add_column(ve[i]);
            ve=StoI(money->text().toStdString());
            for(int i=0 ;i<ve.size();++i)
                p->add_value(ve[i]);
            ve=StoI(__gymid);
            for(int i=0 ;i<ve.size();++i)
                req.add_gymid(ve[i]);
            int len = (int)req.ByteSizeLong();
            QByteArray buf = QByteArray(len,0);
            req.SerializePartialToArray(buf.data(),len);
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

void EditRCBox::emitSaveBox(){
    emit saveBox();
}

EditRCBox::~EditRCBox()
{
    delete uid;
    delete money;
    delete submit;
    delete cancel;
}
