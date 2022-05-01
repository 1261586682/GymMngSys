#include "EditMsgBox.h"

//构建添加学生主界面
EditMsgBox::EditMsgBox(QTcpSocket *socket)
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
    */
    setWindowTitle("添加人员信息");
    QVBoxLayout * VBox = new QVBoxLayout;
    this->socket = socket;

    QFormLayout *FormLayout = new QFormLayout;
    id = new QLineEdit;
    id->setPlaceholderText("长度不大于11");
    sex = new QLineEdit;
    sex->setPlaceholderText("男或女");
    age = new QLineEdit;
    age->setPlaceholderText("0~150");
    name = new QLineEdit;
    name->setPlaceholderText("长度不大于11");
    FormLayout->addRow("人员编号：",id);
    FormLayout->addRow("姓名：",name);
    FormLayout->addRow("性别：",sex);
    FormLayout->addRow("年龄：",age);
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
    QObject::connect(submit,&QPushButton::clicked,this,&EditMsgBox::saveMsg);
    //点击取消按钮时，关闭添加学生信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&EditMsgBox::close);
}

//当用户添加提交按钮时，保存学生信息
void EditMsgBox::saveMsg(){
      //确保所有信息填写后，将信息写入到文件中，否则提示用户将信息填写完整
      //加入到数据库
        if(this->id->text() !="" && this->name->text()!="" && this->sex->text()!="" && this->age->text()!="" ){
            if(!check_str(name->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(!check_str(id->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(!check_str(age->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(!check_str(sex->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
            if(name->text().length()>11 || id->text().length()>11 || age->text().toInt()<0 || age->text().toInt()>150 ||(sex->text()!="男"&&sex->text()!="女"))
            {
                QMessageBox::warning(this,"提示","请按要求填写信息",QMessageBox::Ok);
                return;
            }
            Proto::request msg;
            Proto::Student *stu = new Proto::Student;
            msg.set_function(func_name::AddStudent);
            stu = msg.add_addstu();
            std::vector<int>ve = StoI(name->text().toStdString());
            for(int i=0;i<ve.size();++i)
                stu->add_name(ve[i]);
            ve.clear();
            ve = StoI(id->text().toStdString());
            for(int i=0;i<ve.size();++i)
                stu->add_uid(ve[i]);
            stu->set_gender(sex->text()=="男"?true:false);
            stu->set_age(age->text().toInt());
            ve=StoI(__gymid);
            for(int i=0 ;i<ve.size();++i)
                msg.add_gymid(ve[i]);
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

void EditMsgBox::emitSaveBox(){
    emit saveBox();
}

EditMsgBox::~EditMsgBox()
{
    delete id;
    delete sex;
    delete age;
    delete name;
    delete submit;
    delete cancel;
}
