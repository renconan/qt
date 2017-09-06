#include "settingmanage.h"
#include "ui_settingmanage.h"

#include <QStringList>
#include <QString>
#include <QDebug>
#include <QMessageBox>




Settingmanage::Settingmanage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settingmanage)
{
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setColumnCount(3);
    setFixedWidth(500);


    setWindowTitle(QString("Setting management"));
    setWindowIcon(QIcon(":/set.ico"));
        setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    QStringList strlist;
    strlist<<"name"<<"V"<<"ID";
    ui->tableWidget->setHorizontalHeaderLabels(strlist);
    settingheader=(struct SETTINGNODELIST *)malloc(sizeof(struct SETTINGNODELIST));
    settingheader->next=NULL;
    isedit=false;
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,150);

    ui->pushButton_4->setText("Save");







}


void Settingmanage::getsettinglist(struct SETTINGNODELIST *list)
{

    int nodenum=0;
    if(list==NULL) //如果此链表为空，返回
    {
     return;
    }
    else //复制list链表
    {
    if(settingheader==NULL)
       return;
    if(list->next==NULL) //没有返回
        return;

    settingheader=list;
    struct SETTINGNODELIST *settingnext1;
    settingnext1=settingheader->next;
    while(settingnext1!=NULL)
    {
        nodenum++;
        settingnext1=settingnext1->next;

    }
    ui->tableWidget->setRowCount(nodenum);

    if(nodenum>=1) //初始化表格
    {
       int i=0;
       for(i=0;i<nodenum;i++)
       for(int j=0;j<3;j++)
            ui->tableWidget->setItem(i,j,new QTableWidgetItem);

       struct SETTINGNODELIST  *settingnext2;
       settingnext2=settingheader->next;
       i=0;
       while(settingnext2!=NULL)
       {
       ui->tableWidget->item(i,0)->setText(QString(QLatin1String(settingnext2->set.name)));
       ui->tableWidget->item(i,1)->setText(QString(QLatin1String(settingnext2->set.value)));
       ui->tableWidget->item(i,2)->setText(QString(QLatin1String(settingnext2->set.ID)));
     //  ui->tableWidget->item(i,3)->setText(QString(QLatin1String(settingnext2->set.ref)));
       settingnext2=settingnext2->next;
       i++;
       }
    }
}
}



void Settingmanage::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}


void Settingmanage::updateshow()
{
    //更新 链表
    struct SETTINGNODELIST *next;  //找到当前
    next=settingheader->next;
    int nodenum=0;
    while(next!=NULL)
    {
     next=next->next;
     nodenum++;
    }
    //更新显示

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(nodenum);
    ui->tableWidget->setColumnCount(3);
    QStringList strlist;
    strlist<<"name"<<"V"<<"ID";
    ui->tableWidget->setHorizontalHeaderLabels(strlist);
    int i=0;
    for(i=0;i<nodenum;i++)
    for(int j=0;j<3;j++)
         ui->tableWidget->setItem(i,j,new QTableWidgetItem);



   struct SETTINGNODELIST *settingnext2=settingheader->next;
    i=0;
    while(settingnext2!=NULL)
    {
    ui->tableWidget->item(i,0)->setText(QString(QLatin1String(settingnext2->set.name)));
    ui->tableWidget->item(i,1)->setText(QString(QLatin1String(settingnext2->set.value)));
    ui->tableWidget->item(i,2)->setText(QString(QLatin1String(settingnext2->set.ID)));
  //  ui->tableWidget->item(i,3)->setText(QString(QLatin1String(settingnext2->set.ref)));
    i++;
    settingnext2=settingnext2->next;
    }

}

// false 重合  true 没有重合
bool Settingmanage::checkidchonghe(QString ID) //检查ID是否重合
{
     struct SETTINGNODELIST *next=settingheader;
     while(next!=NULL)
     {
         if(QString(QLatin1String(next->set.ID))==ID)
          return false;
          next=next->next;
     }
     return true;
}


void Settingmanage::updatesetting(QString name,QString value,QString ID)
{

    if(isedit==false)
  {
    //检查ID是否有重合的 有重合提示出错，
        if(!checkidchonghe(ID))
        {
            QMessageBox::information(this, "warning","ID is reduplicated,please a new ID!", QMessageBox::Yes);
            return;
        }

   struct SETTINGNODELIST *tmp;
   tmp=(struct SETTINGNODELIST *)malloc(sizeof(struct SETTINGNODELIST));
   tmp->next=NULL;
   copytoshuzu(tmp->set.name,name,name.size());
   copytoshuzu(tmp->set.value,value,value.size());
   copytoshuzu(tmp->set.ID,ID,ID.size());
  // copytoshuzu(tmp->set.ref,ref,ref.size());



   struct SETTINGNODELIST *pre,*next;  //找到当前
   pre=settingheader;
   next=pre->next;
   while(next!=NULL)
   {
    pre=next;
    next=next->next;
   }
   pre->next=tmp;
   updateshow();
   }
   else //是正在编辑的editID 查找该ID 进行赋值
    {

     struct SETTINGNODELIST *tmp=settingheader->next;
     while(tmp!=NULL)
     {
         if(QString(QLatin1String(tmp->set.ID))==editID)
          {
             copytoshuzu(tmp->set.name,name,name.size());
             copytoshuzu(tmp->set.value,value,value.size());
             copytoshuzu(tmp->set.ID,ID,ID.size());
           //  copytoshuzu(tmp->set.ref,ref,ref.size());
          //   qDebug()<<__func__<<__LINE__<<tmp->set.ref;
          //   qDebug()<<__func__<<__LINE__<<ref;

             updateshow();
             isedit=false;
          }
         tmp=tmp->next;
     }
    }
}



void Settingmanage::on_pushButton_clicked()
{
 addsetting=new Addsetting;
 connect(addsetting,SIGNAL(SIG_sendsetting(QString,QString,QString)),this,SLOT(updatesetting(QString,QString,QString)));
 addsetting->show();

}

//编辑节点
void Settingmanage::on_pushButton_2_clicked()
{
   QList<QTableWidgetItem *> items=ui->tableWidget->selectedItems();

   if(!items.isEmpty()) //已经选中了某航
  {

    addsetting=new Addsetting;
    connect(addsetting,SIGNAL(SIG_sendsetting(QString,QString,QString)),this,SLOT(updatesetting(QString,QString,QString)));


    addsetting->setsetting(items.at(0)->text(),items.at(1)->text(),items.at(2)->text());

    addsetting->show();
    isedit=true;
    editID=items.at(2)->text();
  }
}

//删除节点
void Settingmanage::on_pushButton_3_clicked()
{
    QList<QTableWidgetItem *> items=ui->tableWidget->selectedItems();
    if(!items.isEmpty()) //已经选中了某航
    {
        QString ID_del=items.at(2)->text();
        struct SETTINGNODELIST *pre,*next;
        pre=settingheader;
        next=settingheader->next;
        while(next!=NULL)
        {

            if(QString(QLatin1String(next->set.ID))==ID_del)
            {
            break;
            }
            pre=next;
            next=next->next;
        }
        if(next==NULL)
            return;
        pre->next=next->next;

        struct SETTINGNODELIST *next2;
        next2=settingheader->next;
        while(next2!=NULL)
        {

            next2=next2->next;

        }
        updateshow();
    }
}


//保存新的节点 链表
void Settingmanage::on_pushButton_4_clicked()
{

   emit savesettingnode((void *)settingheader);

   struct SETTINGNODELIST *next; //所有的定值链表
   next=settingheader->next;
   while(next!=NULL)
   {
       next=next->next;
   }

   close();

}

//退出 不保存
void Settingmanage::on_pushButton_5_clicked()
{
    close();
}

Settingmanage::~Settingmanage()
{
   // free(settingheader);
    delete ui;
}








