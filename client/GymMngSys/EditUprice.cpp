#include "EditUprice.h"

EditUprice::EditUprice(QTcpSocket *socket)
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
    price = new QLineEdit;
    price->setPlaceholderText("0~10000");
    FormLayout->addRow("单价（元/小时）：",price);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    QGridLayout* HBox = new QGridLayout;
    submit = new QPushButton("确定");
    cancel = new QPushButton("取消");

    HBox->addWidget(submit,0,0);
    HBox->addWidget(cancel,0,1);

    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);

    //设置
    QObject::connect(submit,&QPushButton::clicked,this,&EditUprice::set);

    //点击取消按钮时，关闭窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&EditUprice::close);
}

void EditUprice::set()
{
    //确保所有信息填写后，将信息写入到文件中，否则提示用户将信息填写完整
    //加入到数据库
      if(this->price->text().toInt() > 0 && this->price->text() != "" && this->price->text().toInt() < 10000){
          if(!check_str(price->text())) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);return;}
          __uprice = price->text().toInt();
          Proto::request req;
          Proto::SetPrice *setprice = new Proto::SetPrice;
          setprice = req.add_setprice();
          std::vector<int>ve=StoI(__gymid);
          for(int i=0;i<ve.size();++i)
              setprice->add_gymid(ve[i]);
          setprice->set_uprice(price->text().toInt());
          req.set_function(func_name::SetUprice);
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

void EditUprice::emitSaveBox(){
    emit saveBox();
}

EditUprice::~EditUprice()
{
    delete price;
    delete submit;
    delete cancel;
}
