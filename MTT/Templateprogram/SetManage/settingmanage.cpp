#include "settingmanage.h"
#include "ui_settingmanage.h"
#include "myweitu.h"
#include "settingnodelink_2.h"
#include <stdlib.h>
#include <stdio.h>
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <QWaitCondition>
#include <QMutex>

SettingManage::SettingManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingManage)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    settinglink=(struct SETTINGNODEIDLIST *)malloc(sizeof(struct SETTINGNODEIDLIST)); //所有的定值链表
    settinglink->next=NULL;
    settingnum=0;    //定值个数清0
    keyboardpos=0;
    keypadon=false;
        showFullScreen();
    ui->tableWidget->setItemDelegate(new NoFocusDelegate());

    ui->tableWidget->setContextMenuPolicy(Qt::NoContextMenu);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


}

void SettingManage::Setsettinglink(struct SETTINGNODELIST *link)
{

    if(link==NULL||link->next==NULL||settinglink==NULL)
        return;
   int id=0;
   settingnodelink=link;
   struct SETTINGNODELIST *next=link->next;
   SETTINGID value;
   //settinglink;
   while(next!=NULL)
   {
       strcpy(value.ID,next->set.ID);
       strcpy(value.name,next->set.name);
       strcpy(value.value,next->set.value);
     //  strcpy(value.ref,next->set.ref);

       value.index=id;
       insertsetnode_2(settinglink,value);
       id++;
       next=next->next;
   }
   if(id>=1)
   settingnum=id;

   Showinit(); //所有ID显示初始化
}

//显示上一页
void SettingManage::showprepage()
{
    /*
    int settingnum; //定值的个数
    int settingnum_of_page; //当前页的sect的个数
    int currentpage;       //当前显示第几页
    int sumpage;       //总的页数
    int page_remainder; //剩余页
    */
    if(currentpage==1) //已经是第一页返回
     {
      return;
     }
    currentpage--; //当前页号加1
    settingnum_of_page=num_per_row; //按下上一页 sect个数一定为6
    page_remainder++;  //

    ui->tableWidget->clear();
    QStringList head1;
    head1<<tr("name")<<tr("ID")<<tr("value");
    ui->tableWidget->setHorizontalHeaderLabels(head1);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(num_per_row);



    for(int i=0;i<num_per_row;i++)
    for(int j=0;j<3;j++)
    ui->tableWidget->setItem(i,j,new QTableWidgetItem);



    SETTINGID settingvalue;
    for(int i=0,j=0;i<settingnum_of_page;i++)
      { j=(currentpage-1)*num_per_row+i;
        if(getsettingidfromindex_2(settinglink,j,settingvalue)==true)
        {
        ui->tableWidget->item(i,0)->setText(QString(QLatin1String(settingvalue.name)));
        ui->tableWidget->item(i,1)->setText(QString(QLatin1String(settingvalue.ID)));
        ui->tableWidget->item(i,2)->setText(QString(QLatin1String(settingvalue.value)));
        }
      }
}

//下一页
void SettingManage::shownextpage()
{

    /*
    int settingnum; //定值的个数
    int settingnum_of_page; //当前页的sect的个数
    int currentpage;       //当前显示第几页
    int sumpage;       //总的页数
    int page_remainder; //剩余页
    */
    if(page_remainder==0)
       return ;
    currentpage++; //当前页号加1
    if(currentpage!=sumpage) //若不是最后一页则 这一页显示个数为
        settingnum_of_page=num_per_row;
    else settingnum_of_page=settingnum%num_per_row;
    page_remainder--;

    ui->tableWidget->clear();
    QStringList head1;
    head1<<tr("name")<<tr("ID")<<tr("value");
    ui->tableWidget->setHorizontalHeaderLabels(head1);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(num_per_row);
    for(int i=0;i<num_per_row;i++)
    for(int j=0;j<3;j++)
    ui->tableWidget->setItem(i,j,new QTableWidgetItem);
    SETTINGID settingvalue;
    for(int i=0,j=0;i<settingnum_of_page;i++)
      { j=(currentpage-1)*num_per_row+i;
        if(getsettingidfromindex_2(settinglink,j,settingvalue)==true)
        {
        ui->tableWidget->item(i,0)->setText(QString(QLatin1String(settingvalue.name)));
        ui->tableWidget->item(i,1)->setText(QString(QLatin1String(settingvalue.ID)));
        ui->tableWidget->item(i,2)->setText(QString(QLatin1String(settingvalue.value)));
        }
      }


}

//主界面的显示初始化
void SettingManage::Showinit()
{

   ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单个被选中


   if(settingnum>=1)
   {
       //当前页的个数
       if(settingnum>num_per_row)
       settingnum_of_page=num_per_row;
       else    settingnum_of_page=settingnum;
       //当前第几页
       currentpage=1;
       //总页数
       sumpage=(settingnum-1)/num_per_row+1;
       //剩余显示的页数
       page_remainder=(sumpage-currentpage);



       ui->tableWidget->setColumnCount(3);
       ui->tableWidget->setRowCount(num_per_row);
       for(int i=0;i<num_per_row;i++)
       for(int j=0;j<3;j++)
      { ui->tableWidget->setItem(i,j,new QTableWidgetItem);
        ui->tableWidget->item(i,j)->setText(tr(""));
      }
       QStringList head1;
       head1<<tr("name")<<tr("ID")<<tr("value");
       ui->tableWidget->setHorizontalHeaderLabels(head1);
       ui->tableWidget->verticalHeader()->setVisible(false);
       ui->tableWidget->setColumnWidth(0,400);
       ui->tableWidget->setColumnWidth(1,200);
       ui->tableWidget->setColumnWidth(2,150);


       SETTINGID settingvalue;
       for(int i=0;i<settingnum_of_page;i++)
        {
            if(getsettingidfromindex_2(settinglink,i,settingvalue)==true)
            {
                ui->tableWidget->item(i,0)->setText(QString(QLatin1String(settingvalue.name)));
                ui->tableWidget->item(i,1)->setText(QString(QLatin1String(settingvalue.ID)));
                ui->tableWidget->item(i,2)->setText(QString(QLatin1String(settingvalue.value)));
            }

        }


}

}


void SettingManage::clearkeypadflag()
{

keypadon=false;
}

//小键盘关闭后 更新显示定值的value
void SettingManage::dissettingvalue(QString set)
{
        bool ok;
        set.toFloat(&ok);
        if(ok==false)
            return;
        ui->tableWidget->item(keyboardpos,2)->setText(set);
        //更新链表中该定值的value
        setsettingvaluefromindex_2(settinglink,keyboardpos+(currentpage-1)*num_per_row,set);

}


void SettingManage::SetNewsettinglink(struct SETTINGNODELIST *link1,struct SETTINGNODEIDLIST *link2)
 {
     if(link1==NULL||link1->next==NULL||link2==NULL||link2->next==NULL)
     {
         return;
     }
     struct SETTINGNODELIST *next1=link1->next;
     struct SETTINGNODEIDLIST *next2=link2->next;
     while(next1!=NULL)
     {
           strcpy(next1->set.ID,next2->set.ID);
           strcpy(next1->set.name,next2->set.name);
           strcpy(next1->set.value,next2->set.value);
          // strcpy(next1->set.ref,next2->set.ref);
           next1=next1->next;
           next2=next2->next;
     }
 }

void SettingManage::saveclosewin()
{
    SetNewsettinglink(settingnodelink,settinglink);
    emit SIG_returnsetlink((void *)settingnodelink);
    emit SIG_Close();
    free(settinglink);
    settinglink=NULL;
    close();

}

void SettingManage::cancellclosewin() //取消退出
 {
     emit SIG_returnsetlink((void *)settingnodelink);
     emit SIG_Close();
     free(settinglink);
     settinglink=NULL;
     close();
 }



SettingManage::~SettingManage()
{
    delete ui;
}

void SettingManage::on_pushButton_clicked() //上一页
{
  showprepage();
}

void SettingManage::on_pushButton_2_clicked()
{
   shownextpage();
}

void SettingManage::on_pushButton_3_clicked()
{
  saveclosewin() ;
}

void SettingManage::on_pushButton_4_clicked()
{
   cancellclosewin();
}




void SettingManage::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if(!(column==2&&(row+1)<=settingnum_of_page))
    return; //超出值
    if(keypadon==false)
   {
    int index=0;
    keypadon=true;
    SETTINGID settingvalue;
    keyboardpos=row; //需要小键盘编辑的位置，相对于表格开头
    index=keyboardpos+(currentpage-1)*num_per_row;
    getsettingidfromindex_2(settinglink,index,settingvalue);
    keyboad=new Dialog;
    connect(keyboad,SIGNAL(sendvalue(QString)),this,SLOT(dissettingvalue(QString)));
    connect(keyboad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    keyboad->setorignaltext(QString(settingvalue.value));
    keyboad->setModal(true);
    keyboad->raise();
    keyboad->activateWindow();
    keyboad->show();
    }
}
