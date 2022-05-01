#include "MainWidget.h"

MainWidget::MainWidget(QTcpSocket* sock)
{

    setWindowTitle("高校体育馆管理系统");
    this->resize(1000,600);
    socket = sock;
    LeftWidget = create_LeftWidget();
    RightWidget = create_RightWidget();

    QHBoxLayout *HBoxLayout = new QHBoxLayout(this);
    HBoxLayout->addWidget(LeftWidget,3);
    HBoxLayout->addWidget(RightWidget,1);
}

MainWidget::~MainWidget()
{
    socket->close();
    delete socket;
    delete LeftWidget;
    delete RightWidget;
}

void MainWidget::decode(QByteArray buf)
{
    Proto::response res;
    res.ParsePartialFromArray(buf.data(),buf.size());
    qDebug()<<res.function();
    if(res.function()==func_name::AddStudent||res.function()==func_name::AddRepairlog)
    {
        if(res.res())
            QMessageBox::warning(this,"提示", "添加成功",QMessageBox::Ok);
        else QMessageBox::warning(this,"提示", "添加失败",QMessageBox::Ok);
    }
    else if(res.function() == func_name::SetUprice)
    {
        if(res.res())
            QMessageBox::warning(this,"提示", "设置成功",QMessageBox::Ok);
        else QMessageBox::warning(this,"提示", "设置失败",QMessageBox::Ok);
    }
    else if(res.function() == func_name::AddRepairlog)
    {
        if(res.res())
            QMessageBox::warning(this,"提示", "添加成功",QMessageBox::Ok);
        else QMessageBox::warning(this,"提示", "添加失败",QMessageBox::Ok);
    }
    else if(res.function() == func_name::ChangePerson)
    {
        if(res.res())
            QMessageBox::warning(this,"提示", "成功",QMessageBox::Ok);
        else QMessageBox::warning(this,"提示", "失败",QMessageBox::Ok);
    }
    else if(res.function() == func_name::FlushTable)
    {
        if(res.res()&&res.table_name()==table_name::PersonInfo)
        {
            disconnect(TableWidgets[0],&QTableWidget::cellChanged,0,0);
            TableWidgets[0]->setSortingEnabled(false);
            int len = res.stu().size();
            TableWidgets[0]->setRowCount(0);
            for(int i=0;i<len;++i)
            {
                Proto::Student stu = res.stu(i);
                TableWidgets[0]->setRowCount(TableWidgets[0]->rowCount()+1);
                std::string uid = AtoS(stu.uid());
                std::string name = AtoS(stu.name());
                bool gender = stu.gender();
                int age = stu.age();
                std::string st_time = AtoS(stu.st_time());
                int balance = stu.balance();
                TableWidgets[0]->setItem(TableWidgets[0]->rowCount()-1,0,new ATableWidgetItem(QString::fromStdString(uid)));
                TableWidgets[0]->setItem(TableWidgets[0]->rowCount()-1,1,new ATableWidgetItem(QString::fromStdString(name)));
                TableWidgets[0]->setItem(TableWidgets[0]->rowCount()-1,2,new ATableWidgetItem(gender == true ?"男":"女"));
                TableWidgets[0]->setItem(TableWidgets[0]->rowCount()-1,3,new ATableWidgetItem(QString::fromStdString(std::to_string(age))));
                TableWidgets[0]->setItem(TableWidgets[0]->rowCount()-1,4,new ATableWidgetItem(QString::fromStdString(std::to_string(balance))));
            }
            connect(TableWidgets[0],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int,int)>(&MainWidget::change_Table));
            connect(TableWidgets[0],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int)>(&MainWidget::flush_ListWidget));
            TableWidgets[0]->setSortingEnabled(true);
        }
        else if(res.res()&&res.table_name()==table_name::UseInfo)
        {
            disconnect(TableWidgets[1],&QTableWidget::cellChanged,0,0);
            TableWidgets[1]->setSortingEnabled(false);
            int len = res.stu().size();
            TableWidgets[1]->setRowCount(0);
            for(int i=0;i<len;++i)
            {
                Proto::Student stu = res.stu(i);
                std::string uid = AtoS(stu.uid());
                std::string name = AtoS(stu.name());
                std::string st_time = AtoS(stu.st_time());
                int balance = stu.balance();
                if(st_time == "") continue;
                TableWidgets[1]->setRowCount(TableWidgets[1]->rowCount()+1);
                TableWidgets[1]->setItem(TableWidgets[1]->rowCount()-1,0,new ATableWidgetItem(QString::fromStdString(uid)));
                TableWidgets[1]->setItem(TableWidgets[1]->rowCount()-1,1,new ATableWidgetItem(QString::fromStdString(name)));
                TableWidgets[1]->setItem(TableWidgets[1]->rowCount()-1,2,new ATableWidgetItem(QString::fromStdString(st_time)));
                TableWidgets[1]->setItem(TableWidgets[1]->rowCount()-1,3,new ATableWidgetItem(QString::fromStdString(std::to_string(balance))));
            }
            connect(TableWidgets[1],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int,int)>(&MainWidget::change_Table));
            connect(TableWidgets[1],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int)>(&MainWidget::flush_ListWidget));
            TableWidgets[1]->setSortingEnabled(true);
        }
        else if(res.res()&&res.table_name()==table_name::RepairLog)
        {
            disconnect(TableWidgets[2],&QTableWidget::cellChanged,0,0);
            TableWidgets[2]->setSortingEnabled(false);
            int len = res.log().size();
            TableWidgets[2]->setRowCount(0);
            for(int i=0;i<len;++i)
            {
                Proto::Repairlog log = res.log(i);
                std::string st_time = AtoS(log.time());
                std::string reason = AtoS(log.reason());
                int spend = log.spend();
                std::string uid = AtoS(log.uid());
                std::string name = get_name_from_uid(uid);
                TableWidgets[2]->setRowCount(TableWidgets[2]->rowCount()+1);
                TableWidgets[2]->setItem(TableWidgets[2]->rowCount()-1,0,new ATableWidgetItem(QString::fromStdString(st_time)));
                TableWidgets[2]->setItem(TableWidgets[2]->rowCount()-1,1,new ATableWidgetItem(QString::fromStdString(reason)));
                TableWidgets[2]->setItem(TableWidgets[2]->rowCount()-1,2,new ATableWidgetItem(QString::fromStdString(std::to_string(spend))));
                TableWidgets[2]->setItem(TableWidgets[2]->rowCount()-1,3,new ATableWidgetItem(QString::fromStdString(uid)));
                TableWidgets[2]->setItem(TableWidgets[2]->rowCount()-1,4,new ATableWidgetItem(QString::fromStdString(name)));
            }
            connect(TableWidgets[2],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int,int)>(&MainWidget::change_Table));
            connect(TableWidgets[2],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int)>(&MainWidget::flush_ListWidget));
            TableWidgets[2]->setSortingEnabled(true);
        }
        else if(res.res()&&res.table_name()==table_name::GymInfo)
        {
            Info->clear();
            Proto::Gyminfo info = res.gyminfo(0);
            __uprice = info.uprice();
            Info->addItem("场馆编号：\t"+QString::fromStdString(AtoS(info.gymid())));
            Info->addItem("场馆名字：\t"+QString::fromStdString(AtoS(info.gymname())));
            Info->addItem("场馆地点：\t"+QString::fromStdString(AtoS(info.position())));
            Info->addItem("场馆建成时间：\t"+QString::fromStdString(AtoS(info.builttime())));
            Info->addItem("盈利：\t"+QString::fromStdString(std::to_string(info.profit())));
            Info->addItem("单价：\t"+QString::fromStdString(std::to_string(info.uprice())));
            Info->addItem("健身：\t"+QString::fromStdString(info.fitness()?"支持":"不支持"));
            Info->addItem("篮球：\t"+QString::fromStdString(info.basketball()?"支持":"不支持"));
            Info->addItem("羽毛球：\t"+QString::fromStdString(info.badminton()?"支持":"不支持"));
            Info->addItem("游泳：\t"+QString::fromStdString(info.swimming()?"支持":"不支持"));
            Info->addItem("浴室：\t"+QString::fromStdString(info.bath()?"具备":"不具备"));
            Info->addItem("比赛：\t"+QString::fromStdString(info.contest()?"支持":"不支持"));
        }
    }
}

void MainWidget::unpack(QByteArray buf)
{
    while(buf.size()>0)
    {
        int len = AtoI(buf[0])*256+AtoI(buf[1]);
        decode(buf.mid(2,len));
        buf=buf.right(buf.size()-len-2);
    }
}

void MainWidget::recv_Msg()
{
    while(socket->bytesAvailable())
    {
        QByteArray buf = socket->readAll();
        unpack(buf);
    }
}

QGroupBox* MainWidget::create_LeftWidget()
{
    QGroupBox *box = new QGroupBox("信息面板");
    //布局
    QHBoxLayout* AutoHBoxLayout = new QHBoxLayout(this);
    QHBoxLayout* HBoxLayout = new QHBoxLayout();
    //切换部件
    TabWidget = new QTabWidget;
    //添加表
    for(int i=0; i<TableNums; ++i)
        TableWidgets.push_back(new QTableWidget(this));
    //构建表结构
    TableWidgets[0]->setSelectionBehavior(QAbstractItemView::SelectRows);
    TableWidgets[0]->setColumnCount(5);
    TableWidgets[0]->setHorizontalHeaderLabels(QStringList() <<"人员编号"<< "姓名"<<"性别"<<"年龄"<<"余额");
    AutoHBoxLayout->addWidget(TableWidgets[0]);
    TableWidgets[1]->setSelectionBehavior(QAbstractItemView::SelectRows);
    TableWidgets[1]->setColumnCount(4);
    TableWidgets[1]->setHorizontalHeaderLabels(QStringList() <<"人员编号"<< "姓名"<<"入馆时间"<<"余额");
    AutoHBoxLayout->addWidget(TableWidgets[1]);
    TableWidgets[2]->setSelectionBehavior(QAbstractItemView::SelectRows);
    TableWidgets[2]->setColumnCount(5);
    TableWidgets[2]->setHorizontalHeaderLabels(QStringList() << "维修时间" << "维修原因" << "维修经费" << "维修人员编号" << "维修人员姓名" );
    AutoHBoxLayout->addWidget(TableWidgets[2]);

    //设置不可更改
    TableWidgets[1]->setEditTriggers(QAbstractItemView::NoEditTriggers);
    TableWidgets[2]->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //拉伸
    TableWidgets[0]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    TableWidgets[1]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    TableWidgets[2]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //场馆信息
    Info = new QListWidget;

    //添加表到切换部件
    TabWidget->addTab(TableWidgets[0],"人员信息");
    TabWidget->addTab(TableWidgets[1],"使用中");
    TabWidget->addTab(TableWidgets[2],"维修记录");
    TabWidget->addTab(Info,"场馆信息");

    //设置切换部件布局
    TabWidget->setLayout(AutoHBoxLayout);

    //设置Groupbox布局
    HBoxLayout->addWidget(TabWidget);

    TableWidgets[0]->setSortingEnabled(true);
    TableWidgets[1]->setSortingEnabled(true);
    TableWidgets[2]->setSortingEnabled(true);

    for(int i=0; i<TableNums; ++i)
    {
        //当用户点击某个单元格时，更新列表中显示人员信息
        connect(TableWidgets[i],&QTableWidget::cellClicked,this,static_cast<void (MainWidget::*)(int)>(&MainWidget::flush_ListWidget));
        //当用户更改某个单元格内的内容时，保存数据
        connect(TableWidgets[i],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int,int)>(&MainWidget::change_Table));
        //将用户更改某个单元格内的内容时，同时还要更新表格中显示的人员信息
        connect(TableWidgets[i],&QTableWidget::cellChanged,this,static_cast<void (MainWidget::*)(int)>(&MainWidget::flush_ListWidget));
    }

    box->setLayout(HBoxLayout);
    return box;
}

QGroupBox* MainWidget::create_RightWidget()
{
      QGroupBox * box = new QGroupBox("功能面板");
      QVBoxLayout *VBoxLayout = new QVBoxLayout();

      ListWidget = new QListWidget;
      QGridLayout *Buttons = new QGridLayout();
      AddPair = new QPushButton("添加维修记录");
      DelPair = new QPushButton("删除维修记录");
      AddButton = new QPushButton("添加人员");
      DelButton = new QPushButton("删除人员");
      ExitButton = new QPushButton("退出");
      FlushButton = new QPushButton("刷新");
      InButton = new QPushButton("入馆");
      OutButton = new QPushButton("出馆");
      Recharge = new QPushButton("充值");
      Uprice = new QPushButton("设置价格");
      FindEdit = new QLineEdit;
      FindEdit->setPlaceholderText("输入人员编号或姓名查找...");
      FindEdit->setClearButtonEnabled(true);
      FindEdit->setStyleSheet("QLineEdit{padding:3,3,3,3;}");
      Buttons->addWidget(Recharge,0,0);
      Buttons->addWidget(Uprice,0,1);
      Buttons->addWidget(AddPair,1,0);
      Buttons->addWidget(DelPair,1,1);
      Buttons->addWidget(AddButton,2,0);
      Buttons->addWidget(DelButton,2,1);
      Buttons->addWidget(FindEdit,3,0,1,0);
      Buttons->addWidget(InButton,4,0);
      Buttons->addWidget(OutButton,4,1);
      Buttons->addWidget(FlushButton,5,0);
      Buttons->addWidget(ExitButton,5,1);

      VBoxLayout->addWidget(ListWidget,4);
      VBoxLayout->addLayout(Buttons,2);

      //点击添加按钮，执行 add_Msgbox() 方法
      connect(AddButton,&QPushButton::clicked,this,&MainWidget::add_Msgbox);
      connect(AddPair,&QPushButton::clicked,this,&MainWidget::add_Rprbox);
      connect(Recharge,&QPushButton::clicked,this,&MainWidget::add_RCbox);
      connect(Uprice,&QPushButton::clicked,this,&MainWidget::add_Ubox);
      //点击删除按钮，执行 delStuFun() 方法
      connect(DelButton,&QPushButton::clicked,this,&MainWidget::del_func);
      connect(DelPair,&QPushButton::clicked,this,&MainWidget::del_rpr);
      //当向输入框内输入信息后，执行 findStuMess() 方法
      connect(FindEdit,&QLineEdit::returnPressed,this,&MainWidget::find_Msg);
      //点击退出按钮，关闭主窗口
      connect(ExitButton,&QPushButton::clicked,this,&MainWidget::close);
      //点击刷新，刷新所有表
      connect(FlushButton,&QPushButton::clicked,this,static_cast<void (MainWidget::*)()>(&MainWidget::flush_Table));
      //点击入馆，更改状态，刷新所有表
      connect(InButton,&QPushButton::clicked,this,&MainWidget::in_func);
      //点击出馆，更改状态，刷新所有表
      connect(OutButton,&QPushButton::clicked,this,&MainWidget::out_func);

      box->setLayout(VBoxLayout);
      return box;
}

bool MainWidget::check()
{
    if(!socket->waitForConnected())
    {
        QMessageBox::warning(this,"提示", "连接服务器失败",QMessageBox::Ok);
        return false;
    }
    connect(socket,SIGNAL(readyRead()),this,SLOT(recv_Msg()));
    flush_Table();
    return true;
}

void MainWidget::flush_ListWidget(int row)
{
    QTableWidget* cur_TableWidget = qobject_cast<QTableWidget*>(sender());
    flush_ListWidget(cur_TableWidget,row);
}

void MainWidget::flush_ListWidget(QTableWidget* table, int row)
{
    ListWidget->clear();
    int column_num = table->columnCount();
    for(int i=0; i<column_num ;++i)
    {
        QTableWidgetItem* header = table->horizontalHeaderItem(i);
        ListWidget->addItem(header->text()+":\t"+table->item(row,i)->text());
    }
}

void MainWidget::change_Table(int row,int column)
{
    QTableWidget* cur_TableWidget = TableWidgets[0];
    QString uid = cur_TableWidget->item(row,0)->text();
    QString val = cur_TableWidget->item(row,column)->text();
    if(!check_str(val)) {QMessageBox::warning(this,"提示", "不能为空",QMessageBox::Ok);flush_Table();return;}
    if(column == 4|| column == 0) {QMessageBox::warning(this,"提示", "不可修改",QMessageBox::Ok);flush_Table();return;}
    std::string col;
    switch (column)
    {
    case 0:
        col = "uid";
        break;
    case 1:
        col = "name";
        break;
    case 2:
        col = "gender";
        break;
    case 3:
        col = "age";
        break;
    case 4:
        col = "balance";
        break;
    }
    Proto::request req;
    req.set_function(func_name::ChangePerson);
    Proto::ChangePerson *p = new Proto::ChangePerson;
    p = req.add_changestu();
    std::vector<int>ve=StoI(uid.toStdString());
    for(int i=0 ;i<ve.size();++i)
        p->add_uid(ve[i]);
    ve=StoI(col);
    for(int i=0 ;i<ve.size();++i)
        p->add_column(ve[i]);
    ve=StoI(val.toStdString());
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
    flush_Table();
}

void MainWidget::flush_Table(){
    for(int i=0; i<TableNums; ++i)
        flush_Table(i);
    flush_Info();
}

void MainWidget::flush_Table(int i)
{
    //从数据库取记录
    Proto::request req;
    req.set_function(func_name::FlushTable);
    req.set_table_name(i+1);
    std::vector<int>ve=StoI(__gymid);
    for(int i=0 ;i<ve.size();++i)
        req.add_gymid(ve[i]);
    int len = (int)req.ByteSizeLong();
    QByteArray buf = QByteArray(len,0);
    req.SerializePartialToArray(buf.data(),len);
    buf.push_front((char)(len%256));
    buf.push_front((char)(len/256));
    socket->write(buf,buf.size());
    socket->flush();
}

void MainWidget::add_Msgbox()
{
    MsgBox = new EditMsgBox(socket);
    //当添加人员信息窗口关闭时，更新表格，同时清空列表中显示的人员信息
    QObject::connect(MsgBox,&EditMsgBox::saveBox,this,static_cast<void (MainWidget::*)()>(&MainWidget::flush_Table));
    QObject::connect(MsgBox,&EditMsgBox::saveBox,ListWidget,&QListWidget::clear);
    MsgBox->exec();
    delete MsgBox;
}

void MainWidget::add_Rprbox()
{
    RprBox = new EditRprBox(socket);
    //当添加人员信息窗口关闭时，更新表格，同时清空列表中显示的人员信息
    QObject::connect(RprBox,&EditRprBox::saveBox,this,static_cast<void (MainWidget::*)()>(&MainWidget::flush_Table));
    QObject::connect(RprBox,&EditRprBox::saveBox,ListWidget,&QListWidget::clear);
    RprBox->exec();
    delete RprBox;
}

void MainWidget::add_RCbox()
{
    RCBox = new EditRCBox(socket);
    //当添加人员信息窗口关闭时，更新表格，同时清空列表中显示的人员信息
    QObject::connect(RCBox,&EditRCBox::saveBox,this,static_cast<void (MainWidget::*)()>(&MainWidget::flush_Table));
    QObject::connect(RCBox,&EditRCBox::saveBox,ListWidget,&QListWidget::clear);
    RCBox->exec();
    delete RCBox;
}

void MainWidget::add_Ubox()
{
    Ubox = new EditUprice(socket);
    //当添加人员信息窗口关闭时，更新表格，同时清空列表中显示的人员信息
    QObject::connect(Ubox,&EditUprice::saveBox,this,static_cast<void (MainWidget::*)()>(&MainWidget::flush_Table));
    QObject::connect(Ubox,&EditUprice::saveBox,ListWidget,&QListWidget::clear);
    Ubox->exec();
    delete Ubox;
}

void MainWidget::del_func()
{
    QTableWidget* cur_TableWidget = TableWidgets[0];
    QList<QTableWidgetItem*>items = cur_TableWidget->selectedItems();
    //判断用户是否在表格中选中了某个人员信息，只有选中之后，才能执行删除操作
    if(items.count() <= 0)
    {
        QMessageBox::warning(this,"提示","请选择要删除的人员");
        return;
    }
    QMessageBox::StandardButton result=QMessageBox::question(this, "删除","确定要删除这些人吗？");
    Proto::request req;
    req.set_function(func_name::DelStudent);
    std::vector<int>v = StoI(__gymid);
    for(int i=0;i<v.size();++i)
        req.add_gymid(v[i]);
    for(int i=0;i<(items.count()/cur_TableWidget->columnCount());++i)
    {
        int index =i*cur_TableWidget->columnCount();
        if(result == QMessageBox::Yes)
        {
            //更改数据库
            QString uid = items.at(index)->text();
            Proto::Student *stu = new Proto::Student;
            stu = req.add_delstu();
            std::vector<int>ve = StoI(uid.toStdString());
            for(int i=0;i<ve.size();++i)
                stu->add_uid(ve[i]);
        }
    }
    int len = (int)req.ByteSizeLong();
    QByteArray buf = QByteArray(len,0);
    req.SerializePartialToArray(buf.data(),len);
    buf.push_front((char)(len%256));
    buf.push_front((char)(len/256));
    socket->write(buf,buf.size());
    socket->flush();
    flush_Table();
    ListWidget->clear();
}

void MainWidget::del_rpr()
{
    QTableWidget* cur_TableWidget = TableWidgets[2];
    QList<QTableWidgetItem*>items = cur_TableWidget->selectedItems();
    //判断用户是否在表格中选中了某个维修信息，只有选中之后，才能执行删除操作
    if(items.count() <= 0)
    {
        QMessageBox::warning(this,"提示","请选择要删除的维修记录");
        return;
    }
    QMessageBox::StandardButton result=QMessageBox::question(this, "删除","确定要删除这些维修记录吗？");
    Proto::request req;
    req.set_function(func_name::DelRepairlog);
    for(int i=0;i<(items.count()/cur_TableWidget->columnCount());++i)
    {
        int index =i*cur_TableWidget->columnCount();
        if(result == QMessageBox::Yes)
        {
            //更改数据库
            Proto::Repairlog *log = new Proto::Repairlog;
            log = req.add_dellog();
            QString time = items.at(index)->text();
            QString reason = items.at(index+1)->text();
            int spend = items.at(index+2)->text().toInt();
            QString uid = items.at(index+3)->text();
            std::vector<int>ve = StoI(time.toStdString());
            for(int i=0;i<ve.size();++i)
                log->add_time(ve[i]);
            ve = StoI(reason.toStdString());
            for(int i=0;i<ve.size();++i)
                log->add_reason(ve[i]);
            ve = StoI(uid.toStdString());
            for(int i=0;i<ve.size();++i)
                log->add_uid(ve[i]);
            ve = StoI(__gymid);
            for(int i=0;i<ve.size();++i)
                log->add_gymid(ve[i]);
            log->set_spend(spend);
        }
    }
    int len = (int)req.ByteSizeLong();
    QByteArray buf = QByteArray(len,0);
    req.SerializePartialToArray(buf.data(),len);
    buf.push_front((char)(len%256));
    buf.push_front((char)(len/256));
    socket->write(buf,buf.size());
    socket->flush();
    flush_Table();
    ListWidget->clear();
}

void MainWidget::find_Msg()
{
    QTableWidget *cur_TableWidget = TableWidgets[0];
    qint32 count = cur_TableWidget->rowCount();
    bool findSuccess = false;
    if (count > 0)
    {
        for (qint32 i = 0; i < count; i++)
        {
            QString id = cur_TableWidget->item(i, 0)->text();
            QString name = cur_TableWidget->item(i, 1)->text();
            if (id == FindEdit->text() || name == FindEdit->text())
            {
                findSuccess = true;
                cur_TableWidget->selectRow(i);
                flush_ListWidget(cur_TableWidget, i);
                break;
            }
        }
        if (findSuccess == false)
        {
            QMessageBox::information(this, "查找失败", "当前表格中没有姓名或编号为【" + FindEdit->text() + "】人员");
        }
    }
}

void MainWidget::flush_Info()
{
    //从数据库取得场馆信息
    Proto::request req;
    req.set_function(func_name::FlushTable);
    req.set_table_name(table_name::GymInfo);
    std::vector<int>ve=StoI(__gymid);
    for(int i=0 ;i<ve.size();++i)
        req.add_gymid(ve[i]);
    int len = (int)req.ByteSizeLong();
    QByteArray buf = QByteArray(len,0);
    req.SerializePartialToArray(buf.data(),len);
    buf.push_front((char)(len%256));
    buf.push_front((char)(len/256));
    socket->write(buf,buf.size());
    socket->flush();
}

void MainWidget::in_func()
{
    QTableWidget* cur_TableWidget = TableWidgets[0];
    QList<QTableWidgetItem*>items = cur_TableWidget->selectedItems();
    //判断用户是否在表格中选中了某个人员信息，只有选中之后，才能执行删除操作
    if(items.count() <= 0)
    {
        QMessageBox::warning(this,"提示","请选择要进入体育馆的人员");
        return;
    }
    QMessageBox::StandardButton result=QMessageBox::question(this, "提示","确定这些人要入馆吗？");
    Proto::request req;
    req.set_function(func_name::StudentIn);
    std::vector<int>v = StoI(__gymid);
    for(int i=0;i<v.size();++i)
        req.add_gymid(v[i]);
    for(int i=0;i<(items.count()/cur_TableWidget->columnCount());++i)
    {
        int index =i*cur_TableWidget->columnCount();
        if(result == QMessageBox::Yes)
        {
            //更改数据库
            QString uid = items.at(index)->text();
            int balance = items.at(index+4)->text().toInt();
            if(balance <= 0)
            {
                QMessageBox::warning(this,"提示","编号为"+uid+"的人员余额不足，请先充值");
                continue;
            }
            Proto::Student *stu = new Proto::Student;
            stu = req.add_in();
            std::vector<int>ve = StoI(uid.toStdString());
            for(int i=0;i<ve.size();++i)
                stu->add_uid(ve[i]);
        }
    }
    int len = (int)req.ByteSizeLong();
    QByteArray buf = QByteArray(len,0);
    req.SerializePartialToArray(buf.data(),len);
    buf.push_front((char)(len%256));
    buf.push_front((char)(len/256));
    socket->write(buf,buf.size());
    socket->flush();
    flush_Table();
    ListWidget->clear();
}

void MainWidget::out_func()
{
    QTableWidget* cur_TableWidget = TableWidgets[1];
    QList<QTableWidgetItem*>items = cur_TableWidget->selectedItems();
    //判断用户是否在表格中选中了某个人员信息，只有选中之后，才能执行删除操作
    if(items.count() <= 0)
    {
        QMessageBox::warning(this,"提示","请选择要离开体育馆的人员");
        return;
    }
    QMessageBox::StandardButton result=QMessageBox::question(this, "提示","确定这些人要出馆吗？");
    Proto::request req;
    req.set_function(func_name::StudentOut);
    std::vector<int>v = StoI(__gymid);
    for(int i=0;i<v.size();++i)
        req.add_gymid(v[i]);
    req.set_uprice(__uprice);
    for(int i=0;i<(items.count()/cur_TableWidget->columnCount());++i)
    {
        int index =i*cur_TableWidget->columnCount();
        if(result == QMessageBox::Yes)
        {
            //更改数据库
            QString uid = items.at(index)->text();
            Proto::Student *stu = new Proto::Student;
            stu = req.add_out();
            std::vector<int>ve = StoI(uid.toStdString());
            for(int i=0;i<ve.size();++i)
                stu->add_uid(ve[i]);
        }
    }
    int len = (int)req.ByteSizeLong();
    QByteArray buf = QByteArray(len,0);
    req.SerializePartialToArray(buf.data(),len);
    buf.push_front((char)(len%256));
    buf.push_front((char)(len/256));
    socket->write(buf,buf.size());
    socket->flush();
    flush_Table();
    ListWidget->clear();
}

std::string MainWidget::get_name_from_uid(std::string uid)
{
    for(int i=0;i<TableWidgets[0]->rowCount();++i)
    {
        QTableWidgetItem* item = TableWidgets[0]->item(i,0);
        QTableWidgetItem* name = TableWidgets[0]->item(i,1);
        if(item->text()==QString::fromStdString(uid))
            return name->text().toStdString();
    }
    return "";
}
