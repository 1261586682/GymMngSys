#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QTableWidget>
#include <QApplication>
#include <QListWidget>
#include <QLineEdit>
#include <QVector>
#include <QStringConverter>
#include <QHeaderView>
#include <QDebug>
#include <QCollator>
#include "EditMsgBox.h"
#include "EditRprBox.h"
#include "EditRCBox.h"
#include "EditUprice.h"
#include "proto.pb.h"
#include "enum.h"

class ATableWidgetItem : public QTableWidgetItem {
public:
    using QTableWidgetItem::QTableWidgetItem; // 继承构造
public:
    virtual bool operator<(const QTableWidgetItem& other) const override
    {
        QCollator collator;
        collator.setNumericMode(true);
        auto ret = collator.compare(this->text(), other.text());
        if (ret < 0)
            return true;
        else
            return false;
    }
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QTcpSocket*);
    ~MainWidget();
    bool check();
public slots:
    void flush_ListWidget(int);
    void change_Table(int,int);
    void add_Msgbox();
    void add_Rprbox();
    void add_RCbox();
    void del_func();
    void del_rpr();
    void in_func();
    void out_func();
    void flush_Table();
    void find_Msg();
    void flush_Info();
    void recv_Msg();
    void add_Ubox();
private:
    QGroupBox *create_LeftWidget();
    QGroupBox *create_RightWidget();
    void flush_ListWidget(QTableWidget*,int);
    void flush_Table(int);
    void decode(QByteArray buf);
    void unpack(QByteArray buf);
    std::string get_name_from_uid(std::string);
private:
    //信息面板
    const int TableNums = 3;
    QGroupBox* LeftWidget;
    QTabWidget* TabWidget;
    QListWidget* Info;
    QVector<QTableWidget*>TableWidgets;
    //控制面板
    QGroupBox* RightWidget;
    QListWidget* ListWidget;
    QPushButton* AddButton;
    QPushButton* DelButton;
    QPushButton* ExitButton;
    QLineEdit* FindEdit;
    QPushButton* FlushButton;
    QPushButton* InButton;
    QPushButton* OutButton;
    QPushButton* AddPair;
    QPushButton* DelPair;
    QPushButton* Recharge;
    QPushButton* Uprice;
    //添加弹窗
    EditMsgBox* MsgBox;
    EditRprBox* RprBox;
    EditRCBox* RCBox;
    EditUprice* Ubox;
    //网络套接字
    QTcpSocket *socket;
};

#endif // MAINWIDGET_H
