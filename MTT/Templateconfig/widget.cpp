#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QListWidgetItem>
#include <nodeprocess.h>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "settingnodelink.h"
#include <QDebug>
#include <QDateTime>
#include <QMenu>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(QString("Template Configuration Tool"));
    setWindowIcon(QIcon(":/set.ico"));

    setFixedSize(840,540);

    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);

    memset(&sectmentset,0,sizeof(struct SECTMENTSET));
    nodelink=Nodecreate();
    memset(nodelink,0,sizeof(struct SECTMENTSETNODE));
    nodelink->next=NULL;
    settinglink=(struct SETTINGNODELIST *)malloc(sizeof(struct SETTINGNODELIST)); //?ùóDμ??¨?μá′±í
    settinglink->next=NULL;
    currentsectfunc=0;
    iseditting=false;
    currentid=0;
    sumsectnum=0;
    AuthorizedFileValid=false;
    ui->comboBox->clear();
    ui->comboBox->addItems(QStringList()<<tr("Current")<<tr("Voltage")<<tr("Direction")\
                           <<tr("Frequency")<<tr("Phase")<<tr("Harmonic"));
}
void Widget::on_comboBox_activated(int index)
{
    if(AuthorizedFileValid==true) //只有在授权文件无效的条件下 才能创建新的测试模块
        return;

     if(index==0)
     {
         if(sectmentset.curset.valid)
         {
        QMessageBox::information(this, "warning", "current test already exits!", QMessageBox::Yes);
        return;
         }
      curset=new Currentset;
      curset->setsectmentset(sectmentset);
      curset->settinglink=settinglink;      
      connect(curset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(curset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      curset->show();
     }
     else if(index==1)
     {
        if(sectmentset.volset.valid)
        {
        QMessageBox::information(this, "warning", "voltage test already exits!", QMessageBox::Yes);
        return;
        }
     volset=new Voltageset;
     volset->settinglink=settinglink;
     volset->setsectmentset(sectmentset);
     connect(volset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
     connect(volset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
     volset->show();
     }
     else if(index==2) //方向
     {
         if(sectmentset.dirset.valid)
         {
         QMessageBox::information(this, "warning", "direction test already exits!", QMessageBox::Yes);
         return;
         }
      dirset=new  Direction;

      dirset->setsectmentset(sectmentset);
      dirset->settinglink=settinglink;
      connect(dirset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(dirset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      dirset->show();
     }
     else if(index==3)//频率
     {
         if(sectmentset.freset.valid)
         {
         QMessageBox::information(this, "warning", "frequency test already exits!", QMessageBox::Yes);
         return;
         }
      freset=new  Frequency;
      freset->settinglink=settinglink;
      freset->setsectmentset(sectmentset);
      connect(freset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(freset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      freset->show();

     }


     /*
     else if(index==4) //è?òa2aê?
     {
         if(sectmentset.anytestset.valid)
         {
         QMessageBox::information(this, "warning", "any test already exits!", QMessageBox::Yes);
         return;
         }
      anytestset=new  Anytest;

      anytestset->setsectmentset(sectmentset);
      connect(anytestset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(anytestset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      anytestset->show();

     }

     */

     else if(index==4) //phase
     {
         if(sectmentset.phraseset.valid)
         {
         QMessageBox::information(this, "warning", "Phase test already exits!", QMessageBox::Yes);
         return;
         }
      phraseset=new  Phrase;
      phraseset->setsectmentset(sectmentset);
      phraseset->settinglink=settinglink;
      connect(phraseset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(phraseset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      phraseset->show();
     }
     else if(index==5) //harmonic
     {
         if(sectmentset.harmonicset.valid)
         {
         QMessageBox::information(this, "warning", "Harmonic test already exits!", QMessageBox::Yes);
         return;
         }
      harmonicset=new  Harmonic;
      harmonicset->setsectmentset(sectmentset);
      harmonicset->settinglink=settinglink;
      connect(harmonicset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(harmonicset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      harmonicset->show();
     }

}



void Widget::on_comboBox_currentIndexChanged(int index)
{

}



void Widget::addtestfunc(void *value,int type)
{


    if(type==1)
    {
       if(iseditting==false) //?y?ú±à?-
     {
       sectmentset.curset=*((CURRENTSET *)value);
       sectmentset.curset.valid=true;
       if(curset!=NULL)
       curset->close();

       updatefunclist();
     }
       else
       { //?aà?Dèòa?ü??
        sectmentset.curset=*((CURRENTSET *)value);
        sectmentset.curset.valid=true;
        iseditting=false;
        if(curset!=NULL)
        curset->close();
        QListWidgetItem *item=ui->listWidget->currentItem();
        if(item==NULL)
        return;
        ui->listWidget->currentItem()->setText(sectmentset.curset.name);

       }
    }

    else if(type==2) //μ??1
    {
     if(iseditting==false)
      {
        sectmentset.volset=*((VOLTAGESET *)value);
        sectmentset.volset.valid=true;
        if(volset!=NULL)
        volset->close();
        updatefunclist();
      }
      else {
           sectmentset.volset=*((VOLTAGESET *)value);
           sectmentset.volset.valid=true;
           iseditting=false;
           if(volset!=NULL)
           volset->close();
           QListWidgetItem *item=ui->listWidget->currentItem();
           if(item==NULL)
           return;
           ui->listWidget->currentItem()->setText(sectmentset.volset.name);
       }
    }

    else if(type==3)  //·??ò
   {
       if(iseditting==false)
        {
          sectmentset.dirset=*((DIRECTIONSET *)value);
          sectmentset.dirset.valid=true;
          if(dirset!=NULL)
          dirset->close();
          updatefunclist();


        }
        else
        {
             sectmentset.dirset=*((DIRECTIONSET *)value);
             sectmentset.dirset.valid=true;
             iseditting=false;
             if(dirset!=NULL)
             dirset->close();
             QListWidgetItem *item=ui->listWidget->currentItem();
             if(item==NULL)
             return;
             ui->listWidget->currentItem()->setText(sectmentset.dirset.name);
         }
   }
    else if(type==4)  //?μ?ê
  {
      if(iseditting==false)
       {
         sectmentset.freset=*((FRECURRENCESET *)value);
         sectmentset.freset.valid=true;
         if(freset!=NULL)
         freset->close();
         updatefunclist();
       }
       else
       {
            sectmentset.freset=*((FRECURRENCESET *)value);
            sectmentset.freset.valid=true;
            iseditting=false;
            if(freset!=NULL)
            freset->close();
            QListWidgetItem *item=ui->listWidget->currentItem();
            if(item==NULL)
            return;
            ui->listWidget->currentItem()->setText(sectmentset.freset.name);
        }
  }

    else if(type==5)  //è?òa2aê?
  {
      if(iseditting==false)
       {

         sectmentset.anytestset=*((ANYTESTSET *)value);
         sectmentset.anytestset.valid=true;
         if(anytestset!=NULL)
         anytestset->close();
         updatefunclist();
       }
       else
       {
            sectmentset.anytestset=*((ANYTESTSET *)value);
            sectmentset.anytestset.valid=true;
            iseditting=false;
            if(anytestset!=NULL)
            anytestset->close();
            QListWidgetItem *item=ui->listWidget->currentItem();
            if(item==NULL)
            return;
            ui->listWidget->currentItem()->setText(sectmentset.anytestset.name);
        }
  }

    else if(type==6)
    {
        if(iseditting==false) //?y?ú±à?-
      {
        sectmentset.phraseset=*((PHRASESET *)value);
        sectmentset.phraseset.valid=true;
        if(phraseset!=NULL)
        phraseset->close();
        updatefunclist();
      }
        else
        { //?aà?Dèòa?ü??
         sectmentset.phraseset=*((PHRASESET *)value);
         sectmentset.phraseset.valid=true;
         iseditting=false;
         if(phraseset!=NULL)
         phraseset->close();
         QListWidgetItem *item=ui->listWidget->currentItem();
         if(item==NULL)
         return;
         ui->listWidget->currentItem()->setText(sectmentset.phraseset.name);
        }
    }
    else if(type==7)//harmonic
    {
        if(iseditting==false) //?y?ú±à?-
      {
        sectmentset.harmonicset=*((HARMONICSET *)value);
        sectmentset.harmonicset.valid=true;
        if(harmonicset!=NULL)
        harmonicset->close();
        updatefunclist();
      }
        else
        { //?aà?Dèòa?ü??
         sectmentset.harmonicset=*((HARMONICSET *)value);
         sectmentset.harmonicset.valid=true;
         iseditting=false;
         if(harmonicset!=NULL)
         harmonicset->close();
         QListWidgetItem *item=ui->listWidget->currentItem();
         if(item==NULL)
         return;
         ui->listWidget->currentItem()->setText(sectmentset.harmonicset.name);
        }
    }
}


void Widget::closefuncwin(int type) //1?±?2aê?3ìDòμ? ′°?ú
{
    switch(type)
    {
    case 1:
        if(curset!=NULL)
        {
         curset->close();

        }
         iseditting=false;
        break;
    case 2:
        if(volset!=NULL)
        {
         volset->close();

        }
         iseditting=false;
        break;
    case 3:
        if(dirset!=NULL)
        {
         dirset->close();

        }
         iseditting=false;
        break;

    case 4:
        if(freset!=NULL)
        {
         freset->close();

        }
         iseditting=false;
        break;

    case 5:
        if(anytestset!=NULL)
        {
         anytestset->close();
        }
         iseditting=false;
        break;
    case 6:
        if(phraseset!=NULL)
        {
         phraseset->close();
        }
         iseditting=false;
        break;
    case 7:
        if(harmonicset!=NULL)
        {
         harmonicset->close();
        }
         iseditting=false;
        break;
    }

}



void Widget::updatefunclist()  //?üD?ò???à???μ?2aê?1|?ü??
{
    ui->listWidget->clear();

   if(sectmentset.curset.valid)
   { QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setText(QString(QLatin1String(sectmentset.curset.name)));
    ui->listWidget->addItem(pItem);
   }
    if(sectmentset.volset.valid)
   { QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setText(QString(QLatin1String(sectmentset.volset.name)));
    ui->listWidget->addItem(pItem);
   }

    if(sectmentset.dirset.valid)
   { QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setText(QString(QLatin1String(sectmentset.dirset.name)));
    ui->listWidget->addItem(pItem);
   }

    if(sectmentset.freset.valid)
   { QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setText(QString(QLatin1String(sectmentset.freset.name)));
    ui->listWidget->addItem(pItem);
   }

    if(sectmentset.anytestset.valid)
   { QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setText(QString(QLatin1String(sectmentset.anytestset.name)));
    ui->listWidget->addItem(pItem);
   }
    if(sectmentset.phraseset.valid)
   { QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setText(QString(QLatin1String(sectmentset.phraseset.name)));
    ui->listWidget->addItem(pItem);
   }
    if(sectmentset.harmonicset.valid)
   { QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setText(QString(QLatin1String(sectmentset.harmonicset.name)));
    ui->listWidget->addItem(pItem);
   }

}



void Widget::on_pushButton_clicked()
{
    QListWidgetItem *item=ui->listWidget->currentItem();
    if(item==NULL)
    return;
   QString str=ui->listWidget->currentItem()->text();

  if(str==QString(QLatin1String(sectmentset.curset.name)))
  {

      iseditting=true;
      curset=new Currentset;
      connect(curset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(curset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      curset->show();
      curset->settinglink=settinglink;
      curset->setsectmentset(sectmentset);
      curset->Setcurrentset(sectmentset.curset);

  }
  else if(str==QString(QLatin1String(sectmentset.volset.name)))
  {
      iseditting=true;
      volset=new Voltageset;
      connect(volset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(volset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));

      volset->settinglink=settinglink;
      volset->setsectmentset(sectmentset);
      volset->Setvoltageset(sectmentset.volset);
      volset->show();

  }
   else if(str==QString(QLatin1String(sectmentset.dirset.name)))
  {
      iseditting=true;
      dirset=new Direction;
      connect(dirset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(dirset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      dirset->setsectmentset(sectmentset);
      dirset->SetDirectionset(sectmentset.dirset);
      dirset->settinglink=settinglink;
      dirset->show();

  }

 else if(str==QString(QLatin1String(sectmentset.freset.name)))
  {   //??ê?μ??1????
      iseditting=true;
      freset=new Frequency;
      connect(freset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(freset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      freset->show();
      freset->settinglink=settinglink;
      freset->setsectmentset(sectmentset);
      freset->Setfrequencyset(sectmentset.freset);

  }

  /*
 else if(str==QString(QLatin1String(sectmentset.anytestset.name)))
  {   //??ê?μ??1????
      iseditting=true;
      anytestset=new Anytest;
      connect(anytestset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(anytestset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      anytestset->show();
      anytestset->setsectmentset(sectmentset);
      anytestset->Setanytestset(sectmentset.anytestset);
  }

  */
 else if(str==QString(QLatin1String(sectmentset.phraseset.name)))
  {
      iseditting=true;
      phraseset=new Phrase;
      connect(phraseset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(phraseset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      phraseset->show();
      phraseset->settinglink=settinglink;
      phraseset->setsectmentset(sectmentset);
      phraseset->Setphraseset(sectmentset.phraseset);
  }
  else if(str==QString(QLatin1String(sectmentset.harmonicset.name)))
   {
       iseditting=true;
       harmonicset=new Harmonic;
       connect(harmonicset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
       connect(harmonicset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
       harmonicset->show();
       harmonicset->settinglink=settinglink;
       harmonicset->setsectmentset(sectmentset);
       harmonicset->Setharmonicset(sectmentset.harmonicset);
   }


}
//é?3y?3ò?????
void Widget::on_pushButton_6_clicked()
{
    QListWidgetItem *itemtmp=ui->listWidget->currentItem();
    if(itemtmp==NULL)
    return;
    QString str=ui->listWidget->currentItem()->text();
    if(str==tr(""))
    return;

if(str==QString(QLatin1String(sectmentset.curset.name)))
{
    memset(&sectmentset.curset,0,sizeof(CURRENTSET));
}
else if(str==QString(QLatin1String(sectmentset.volset.name)))
{
    memset(&sectmentset.volset,0,sizeof(VOLTAGESET));
}

else if(str==QString(QLatin1String(sectmentset.dirset.name)))
{
    memset(&sectmentset.dirset,0,sizeof(DIRECTIONSET));
}

else if(str==QString(QLatin1String(sectmentset.freset.name)))
{
    memset(&sectmentset.freset,0,sizeof(FRECURRENCESET));
}

else if(str==QString(QLatin1String(sectmentset.anytestset.name)))
{
    memset(&sectmentset.anytestset,0,sizeof(ANYTESTSET));
}

else if(str==QString(QLatin1String(sectmentset.phraseset.name)))
{
   memset(&sectmentset.phraseset,0,sizeof(PHRASESET));
}
else if(str==QString(QLatin1String(sectmentset.harmonicset.name)))
{
   memset(&sectmentset.harmonicset,0,sizeof(HARMONICSET));
}


ui->listWidget->clear();
if(sectmentset.curset.valid==true)
{ QListWidgetItem *pItem = new QListWidgetItem();
pItem->setText(QString(QLatin1String(sectmentset.curset.name)));
ui->listWidget->addItem(pItem);
}
if(sectmentset.volset.valid==true)
{ QListWidgetItem *pItem = new QListWidgetItem();
pItem->setText(QString(QLatin1String(sectmentset.volset.name)));
ui->listWidget->addItem(pItem);
}
if(sectmentset.dirset.valid==true)
{ QListWidgetItem *pItem = new QListWidgetItem();
pItem->setText(QString(QLatin1String(sectmentset.dirset.name)));
ui->listWidget->addItem(pItem);
}

if(sectmentset.freset.valid==true)
{ QListWidgetItem *pItem = new QListWidgetItem();
  pItem->setText(QString(QLatin1String(sectmentset.freset.name)));
  ui->listWidget->addItem(pItem);
}

if(sectmentset.anytestset.valid==true)
{ QListWidgetItem *pItem = new QListWidgetItem();
  pItem->setText(QString(QLatin1String(sectmentset.anytestset.name)));
  ui->listWidget->addItem(pItem);
}
if(sectmentset.phraseset.valid==true)
{ QListWidgetItem *pItem = new QListWidgetItem();
  pItem->setText(QString(QLatin1String(sectmentset.phraseset.name)));
  ui->listWidget->addItem(pItem);
}
if(sectmentset.harmonicset.valid==true)
{ QListWidgetItem *pItem = new QListWidgetItem();
  pItem->setText(QString(QLatin1String(sectmentset.harmonicset.name)));
  ui->listWidget->addItem(pItem);
}
}

 void Widget::copytoshuzu(char shuzu[],QString str,int len)
 {
     if(len>=512)
         return;
     for(int i=0;i<len;i++)
         shuzu[i]=str.at(i).toLatin1();
     shuzu[len]='\0';
 }


//??3y????
void Widget::on_pushButton_2_clicked()
{
    ui->listWidget->clear();
    memset(&sectmentset,0,sizeof(struct SECTMENTSET));
}


//ìí?ó??
void Widget::on_pushButton_3_clicked()
{
    if(sectmentset.curset.valid==true||\
       sectmentset.volset.valid==true||\
       sectmentset.dirset.valid==true||\
       sectmentset.freset.valid==true||\
       sectmentset.anytestset.valid==true||\
       sectmentset.phraseset.valid==true||\
       sectmentset.harmonicset.valid==true)

    {
        if(nodelink==NULL)
            return;
        bool ok;
        QString sectname=QInputDialog::getText(this,QString(QLatin1String("")),QString(QLatin1String("Please input name of sectment!")),
            QLineEdit::Normal,QString(QLatin1String(sectmentset.name)),&ok);

        if(!ok)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Please input name of sectment!"),QMessageBox::Ok);
            return;
        }

        if(sectname.isEmpty()) {
            QMessageBox::critical(this,tr("Warning"),tr("Please input name of sectment!"),QMessageBox::Ok);
            return;
        }

        copytoshuzu(sectmentset.name,sectname,sectname.size());
        insertnode(nodelink,sectmentset,currentid);
        memset(&sectmentset,0,sizeof(struct SECTMENTSET));
        ui->listWidget->clear();
        currentid++;
        updatesectionlist();

    }
}


//é?3y??
void Widget::on_pushButton_5_clicked()
{
    QListWidgetItem *itemtmp=ui->listWidget_2->currentItem();
    if(itemtmp==NULL)
    return;
    //?òμ?×?oóò??? ??é?3y

    QString str=itemtmp->text();
    QByteArray ba=str.toLatin1();
    char *strchar=ba.data();
    deletenode(nodelink,strchar); //é?3y??á′±í?Dμ??úμ?
    updatesectionlist();
    return;
}





void  Widget::updatesetting(char *filename) //?áè????tμ?ID name value
{
     FILE *file= NULL;
    SETTING setvalue;
    char buf[BUFFSIZE];
    char *p1;
    if((file=fopen(filename,"r")) == NULL)
      {
          printf("fopen error\n");
          return;
      }
    fgets(buf,BUFFSIZE,file);
    if(memcmp(buf,"<SETTINGBEGIN>",14)==0)
    {
        while(fgets(buf,BUFFSIZE,file)!=NULL&&memcmp(buf,"<SETTINGEND>",12)!=0)
        {
            p1=buf;
            if(memcmp(p1,"ID=",3)==0)
            {
                  int i=0;
                  p1=p1+3;
                  while((*p1!=',')&&(*p1!='\0'))
                  {
                  setvalue.ID[i++]=*p1;
                  p1++;
                  }
                  setvalue.ID[i]='\0';
                  if(*p1!='\0') p1++;
                  if(memcmp(p1,"NAME=",5)==0)
                    {
                          i=0;
                          p1=p1+5;
                          while((*p1!=',')&&(*p1!='\0'))
                          {
                          setvalue.name[i++]=*p1;
                          p1++;
                          }
                          setvalue.name[i]='\0';
                        if(*p1!='\0') p1++;
                        if(memcmp(p1,"VALUE=",6)==0)
                          {
                              i=0;
                              p1=p1+6;
                              while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                              {
                              setvalue.value[i++]=*p1;
                              p1++;
                              }
                              setvalue.value[i]='\0';

                              if(strlen(setvalue.ID)!=0) //???úμ??óè?á′±í ±íê??áè?μ?ê??¨?μμ?éè??
                              {
                               setvalue.isprint=true;
                               insertsetnode(settinglink,setvalue);
                              }

                              /*
                              if(*p1!='\0') p1++;
                              if(memcmp(p1,"REF=",4)==0)
                                {
                                    i=0;
                                    p1=p1+4;
                                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                                    {
                                    setvalue.ref[i++]=*p1;
                                    p1++;
                                    }
                                setvalue.ref[i]='\0';
                              if(strlen(setvalue.ID)!=0) //???úμ??óè?á′±í ±íê??áè?μ?ê??¨?μμ?éè??
                              {
                               setvalue.isprint=true;
                               insertsetnode(settinglink,setvalue);
                              }
                                 }

                              */

                          }
        }
        }
   }
    }
    fclose(file);
}


int Widget::is_digit(char ch)
{
    if((ch>='0'&&ch<='9')||ch=='.')
        return 1;
    else
        return 0;
}

bool Widget::checkvalue(char  *ch)
{
    char *c=ch;
    if(ch==NULL)
        return false;

    while(*c!='\0')
    {
        if(isdigit(*c)||(*c=='.'&&isdigit(*ch)))
        c++;
       else return false;
    }
    return true;
}



void Widget::updatesectionlist()
{

    ui->listWidget_2->clear();
    if(nodelink==NULL||nodelink->next==NULL)
        return;

    struct SECTMENTSETNODE *next=nodelink->next;
    while(next!=NULL)
    {
        QListWidgetItem *pItem = new QListWidgetItem();
        pItem->setText(QString(QLatin1String(next->value.name)));
        ui->listWidget_2->addItem(pItem);
        next=next->next;
    }
}

//??3y??á′±í
void Widget::on_pushButton_4_clicked()
{
 nodelink->next=NULL;
 ui->listWidget_2->clear();
 currentid=0;
 AuthorizedFileValid=false;
}




void Widget::Writetestfunc(struct SECTMENTSETNODE *node,FILE *fp)//2016-12-13????
{
    if(node==NULL||fp==NULL)
        return;
    char strbuf[512];

    char tmpstr1[512];

    if(node->value.curset.valid) //óDμ?á÷2aê?1|?ü
   {
     strcpy(strbuf,"<TestfunctionBegin>");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"TESTTYPE=OVERCURRENT.COM"); //ê?μ?á÷2aê?
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTNAME=%s",node->value.curset.name);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTITEM=%s",node->value.curset.Testitem.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"OUTPUTSEL=%s",node->value.curset.output.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"FAULTDURATION=%s",node->value.curset.Faultduraion.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VFAULT=%s",node->value.curset.Vfault.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"ANGLE=%s",node->value.curset.Angle.text);
     fprintf(fp,"%s\n",strbuf);




     int testitem=atoi(node->value.curset.Testitem.text);
     if(testitem&0x01&&node->value.curset.TripValue.valid==true)
     {
        strcpy(strbuf,"<TESTITEMBEGIN>");
        fprintf(fp,"%s\n",strbuf);
        strcpy(strbuf,"TESTITEMNAME=OVERCURRENT.TRIPVALUE"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"IENDSTART=%s",node->value.curset.TripValue.StartValue.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"IENDEND=%s",node->value.curset.TripValue.EndValue.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"ISTEP=%s",node->value.curset.TripValue.StepValue.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"HOLDTIME=%s",node->value.curset.TripValue.HoldTime.text);
        fprintf(fp,"%s\n",strbuf);
        strcpy(strbuf,"TRIPVALUE=0.000");
        fprintf(fp,"%s\n",strbuf);
        //′￥·￠?μ?à1à
        if(node->value.curset.TripValue.assesstripvalue.relerror.valid==true&&node->value.curset.TripValue.assesstripvalue.abserror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.curset.TripValue.assesstripvalue.relerror.errorvalue);
            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }

        else if(node->value.curset.TripValue.assesstripvalue.abserror.valid==true&&node->value.curset.TripValue.assesstripvalue.relerror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
                    node->value.curset.TripValue.assesstripvalue.abserror.errorvalue,\
                    node->value.curset.TripValue.assesstripvalue.abserror.errorvalue_2);

            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }


        else  if(node->value.curset.TripValue.assesstripvalue.relerror.valid==true&&\
           node->value.curset.TripValue.assesstripvalue.abserror.valid==true)
        {

            if(node->value.curset.TripValue.assesstripvalue.assessandor==1) //ó?μ?1??μ
            {
                 sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                         node->value.curset.TripValue.assesstripvalue.relerror.errorvalue,\
                         node->value.curset.TripValue.assesstripvalue.abserror.errorvalue,\
                         node->value.curset.TripValue.assesstripvalue.abserror.errorvalue_2);


                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
            }
            else if(node->value.curset.TripValue.assesstripvalue.assessandor==2)
            {
                sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                        node->value.curset.TripValue.assesstripvalue.relerror.errorvalue,\
                        node->value.curset.TripValue.assesstripvalue.abserror.errorvalue,\
                        node->value.curset.TripValue.assesstripvalue.abserror.errorvalue_2
                        );
                fprintf(fp,"%s\n",strbuf);
                sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
                fprintf(fp,"%s\n",tmpstr1);
            }
        }



        strcpy(strbuf,"<TESTITEMEND>");
        fprintf(fp,"%s\n",strbuf);
     }
     if(testitem&0x02&&node->value.curset.TripTime.valid==true)
     {
        strcpy(strbuf,"<TESTITEMBEGIN>");
        fprintf(fp,"%s\n",strbuf);
        strcpy(strbuf,"TESTITEMNAME=OVERCURRENT.TRIPTIME"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"IEND=%s",node->value.curset.TripTime.EndVaule.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"HOLDTIME=%s",node->value.curset.TripTime.HoldTime.text);
        fprintf(fp,"%s\n",strbuf);
        strcpy(strbuf,"TRIPTIME=0.000");
        fprintf(fp,"%s\n",strbuf);

        if(node->value.curset.TripTime.assesstriptime.relerror.valid==true&&node->value.curset.TripTime.assesstriptime.abserror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.curset.TripTime.assesstriptime.relerror.errorvalue);
            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.curset.TripTime.assesstriptime.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }

        else if(node->value.curset.TripTime.assesstriptime.abserror.valid==true&&node->value.curset.TripTime.assesstriptime.relerror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
            node->value.curset.TripTime.assesstriptime.abserror.errorvalue,\
            node->value.curset.TripTime.assesstriptime.abserror.errorvalue_2);
            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.curset.TripTime.assesstriptime.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }
        else  if(node->value.curset.TripTime.assesstriptime.relerror.valid==true&&\
        node->value.curset.TripTime.assesstriptime.abserror.valid==true)
        {

            if(node->value.curset.TripTime.assesstriptime.assessandor==1) //ó?μ?1??μ
            {
                 sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                 node->value.curset.TripTime.assesstriptime.relerror.errorvalue,\
                 node->value.curset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.curset.TripTime.assesstriptime.abserror.errorvalue_2);
                 fprintf(fp,"%s\n",strbuf);
            }
            else if(node->value.curset.TripTime.assesstriptime.assessandor==2)
            {
                sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                        node->value.curset.TripTime.assesstriptime.relerror.errorvalue,\
                        node->value.curset.TripTime.assesstriptime.abserror.errorvalue,\
                        node->value.curset.TripTime.assesstriptime.abserror.errorvalue_2);
                fprintf(fp,"%s\n",strbuf);
            }
            sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.curset.TripTime.assesstriptime.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }


        strcpy(strbuf,"<TESTITEMEND>");
        fprintf(fp,"%s\n",strbuf);
     }
     sprintf(strbuf,"<TestfunctionEnd>");
     fprintf(fp,"%s\n",strbuf);

   }

    if(node->value.volset.valid) //óDμ??12aê?1|?ü
   {
       strcpy(strbuf,"<TestfunctionBegin>");
       fprintf(fp,"%s\n",strbuf);
       strcpy(strbuf,"TESTTYPE=UNDERVOLTAGE.COM"); //ê?μ??12aê?
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"TESTNAME=%s",node->value.volset.name);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"TESTITEM=%s",node->value.volset.Testitem.text);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"FAULTTYPE=%s",node->value.volset.FaultType.text);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"FAULTDURATION=%s",node->value.volset.Faultduraion.text);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"VPNVPP=%d",node->value.volset.Vpn_Vpp);
       fprintf(fp,"%s\n",strbuf);




       int testitem=atoi(node->value.volset.Testitem.text);
       if(testitem&0x01&&node->value.volset.TripValue.valid==true)
       {
          strcpy(strbuf,"<TESTITEMBEGIN>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTITEMNAME=UNDERVOLTAGE.TRIPVALUE"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"VENDSTART=%s",node->value.volset.TripValue.StartValue.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"VENDEND=%s",node->value.volset.TripValue.EndValue.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"VSTEP=%s",node->value.volset.TripValue.StepValue.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"HOLDTIME=%s",node->value.volset.TripValue.HoldTime.text);
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TRIPVALUE=0.000");
          fprintf(fp,"%s\n",strbuf);
          //′￥·￠?μ?à1à
          if(node->value.volset.TripValue.assesstripvalue.relerror.valid==true\
                  &&node->value.volset.TripValue.assesstripvalue.abserror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.volset.TripValue.assesstripvalue.relerror.errorvalue);
              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }

          else if(node->value.volset.TripValue.assesstripvalue.abserror.valid==true\
                  &&node->value.volset.TripValue.assesstripvalue.relerror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
              node->value.volset.TripValue.assesstripvalue.abserror.errorvalue,\
              node->value.volset.TripValue.assesstripvalue.abserror.errorvalue_2);

              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }


          else  if(node->value.volset.TripValue.assesstripvalue.relerror.valid==true&&\
          node->value.volset.TripValue.assesstripvalue.abserror.valid==true)
          {

              if(node->value.volset.TripValue.assesstripvalue.assessandor==1) //ó?μ?1??μ
              {
                   sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                   node->value.volset.TripValue.assesstripvalue.relerror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue_2);

                   fprintf(fp,"%s\n",strbuf);
                   sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
                   fprintf(fp,"%s\n",tmpstr1);
              }
              else if(node->value.volset.TripValue.assesstripvalue.assessandor==2)
              {
                  sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                   node->value.volset.TripValue.assesstripvalue.relerror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue_2);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }
          }



          strcpy(strbuf,"<TESTITEMEND>");
          fprintf(fp,"%s\n",strbuf);
       }
       if(testitem&0x02&&node->value.volset.TripTime.valid==true)
       {
          strcpy(strbuf,"<TESTITEMBEGIN>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTITEMNAME=UNDERVOLTAGE.TRIPTIME"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
          fprintf(fp,"%s\n",strbuf);


          sprintf(strbuf,"USEMEASUREDVALUE=%d",node->value.volset.TripTime.UseMeasureValue);
          fprintf(fp,"%s\n",strbuf);

          if(node->value.volset.TripTime.UseMeasureValue==0)
         { sprintf(strbuf,"VEND=%s",node->value.volset.TripTime.EndVaule.text);
           fprintf(fp,"%s\n",strbuf);
         }

          sprintf(strbuf,"HOLDTIME=%s",node->value.volset.TripTime.HoldTime.text);
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TRIPTIME=0.000");
          fprintf(fp,"%s\n",strbuf);
          if(node->value.volset.TripTime.assesstriptime.relerror.valid==true&&node->value.volset.TripTime.assesstriptime.abserror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.volset.TripTime.assesstriptime.relerror.errorvalue);
              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }

         else if(node->value.volset.TripTime.assesstriptime.abserror.valid==true&&node->value.volset.TripTime.assesstriptime.relerror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
              node->value.volset.TripTime.assesstriptime.abserror.errorvalue,\
              node->value.volset.TripTime.assesstriptime.abserror.errorvalue_2);

              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }
         else  if(node->value.volset.TripTime.assesstriptime.relerror.valid==true&&\
             node->value.volset.TripTime.assesstriptime.abserror.valid==true)
          {

              if(node->value.volset.TripTime.assesstriptime.assessandor==1) //ó?μ?1??μ
              {
                   sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                   node->value.volset.TripTime.assesstriptime.relerror.errorvalue,\
                   node->value.volset.TripTime.assesstriptime.abserror.errorvalue,\
                   node->value.volset.TripTime.assesstriptime.abserror.errorvalue_2);
                   fprintf(fp,"%s\n",strbuf);
                   sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
                   fprintf(fp,"%s\n",tmpstr1);

              }
              else if(node->value.volset.TripTime.assesstriptime.assessandor==2)
              {
                  sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                  node->value.volset.TripTime.assesstriptime.relerror.errorvalue,\
                  node->value.volset.TripTime.assesstriptime.abserror.errorvalue,\
                  node->value.volset.TripTime.assesstriptime.abserror.errorvalue_2);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }
          }

          strcpy(strbuf,"<TESTITEMEND>");
          fprintf(fp,"%s\n",strbuf);
       }
       sprintf(strbuf,"<TestfunctionEnd>");
       fprintf(fp,"%s\n",strbuf);

   }

    if(node->value.dirset.valid) //óD·??ò2aê?1|?ü
   {

       strcpy(strbuf,"<TestfunctionBegin>");
       fprintf(fp,"%s\n",strbuf);
       strcpy(strbuf,"TESTTYPE=DIRECTION.COM"); //ê?μ??12aê?
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"TESTNAME=%s",node->value.dirset.name);
       fprintf(fp,"%s\n",strbuf);
    if(node->value.dirset.TripMta.valid)
    { //ò???′òó?3?3ìDò?ùDèòaμ?±?á?
     strcpy(strbuf,"<TESTITEMBEGIN>");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"TESTITEMNAME=DIRECTION.MTA"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"IFAULT=%s",node->value.dirset.TripMta.Ifault.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VFAULT=%s",node->value.dirset.TripMta.vfault.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"MTA=%s",node->value.dirset.TripMta.mta.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"FAULTTYPE=%s",node->value.dirset.TripMta.faulttype.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"STEP=%s",node->value.dirset.TripMta.step.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"HOLDTIME=%s",node->value.dirset.TripMta.steptime.text);
     fprintf(fp,"%s\n",strbuf);


     //?à1à
     strcpy(strbuf,"ACTANGLE1=0.000");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"ACTANGLE2=0.000");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"PHRASEMTA=0.000");
     fprintf(fp,"%s\n",strbuf);
     if(node->value.dirset.TripMta.assessmta.relerror.valid==true\
      &&node->value.dirset.TripMta.assessmta.abserror.valid!=true)
     {
         sprintf(tmpstr1,"ASSESSMTA=REL(%f)",node->value.dirset.TripMta.assessmta.relerror.errorvalue);
         fprintf(fp,"%s\n",tmpstr1);
         sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
         fprintf(fp,"%s\n",tmpstr1);
     }

    else if(node->value.dirset.TripMta.assessmta.abserror.valid==true&&\
            node->value.dirset.TripMta.assessmta.relerror.valid!=true)
     {
         sprintf(tmpstr1,"ASSESSMTA=ABS(%f,%f)",\
         node->value.dirset.TripMta.assessmta.abserror.errorvalue,\
         node->value.dirset.TripMta.assessmta.abserror.errorvalue_2);


         fprintf(fp,"%s\n",tmpstr1);
         sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
         fprintf(fp,"%s\n",tmpstr1);
     }
    else   if(node->value.dirset.TripMta.assessmta.relerror.valid==true&&\
        node->value.dirset.TripMta.assessmta.abserror.valid==true)
     {
         if(node->value.dirset.TripMta.assessmta.assessandor==1) //ó?μ?1??μ
         {
              sprintf(strbuf,"ASSESSMTA=REL(%f)&ABS(%f,%f)",\
              node->value.dirset.TripMta.assessmta.relerror.errorvalue,\
              node->value.dirset.TripMta.assessmta.abserror.errorvalue,\
              node->value.dirset.TripMta.assessmta.abserror.errorvalue_2);
              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.dirset.TripMta.assessmta.assessandor==2)
         {
             sprintf(strbuf,"ASSESSMTA=REL(%f)|ABS(%f,%f)",\
             node->value.dirset.TripMta.assessmta.relerror.errorvalue,\
             node->value.dirset.TripMta.assessmta.abserror.errorvalue,\
             node->value.dirset.TripMta.assessmta.abserror.errorvalue_2);
              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
         }
     }


     strcpy(strbuf,"<TESTITEMEND>");
     fprintf(fp,"%s\n",strbuf);
    }
     sprintf(strbuf,"<TestfunctionEnd>");
     fprintf(fp,"%s\n",strbuf);
   }

    if(node->value.freset.valid) //?μ?ê
   {
     strcpy(strbuf,"<TestfunctionBegin>");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"TESTTYPE=UNDERFREQUENCY.COM");
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTNAME=%s",node->value.freset.name);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTITEM=%s",node->value.freset.TestItem.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"FAULTDURATION=%s",node->value.freset.Faultduraion.text);
     fprintf(fp,"%s\n",strbuf);

     int testitem=atoi(node->value.freset.TestItem.text);
     if(testitem&0x01&&node->value.freset.TripValue.valid==true)
     {
      strcpy(strbuf,"<TESTITEMBEGIN>");
      fprintf(fp,"%s\n",strbuf);
      strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.TRIPVALUE"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQENDSTART=%s",node->value.freset.TripValue.FreEndValueStart.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQENDEND=%s",node->value.freset.TripValue.FreEndValueEnd.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQSTEP=%s",node->value.freset.TripValue.FreStep.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"HOLDTIME=%s",node->value.freset.TripValue.HoldTime.text);
      fprintf(fp,"%s\n",strbuf);
      strcpy(strbuf,"TRIPFREQ=0.000");
      fprintf(fp,"%s\n",strbuf);

      if(node->value.freset.TripValue.assesstripvalue.relerror.valid==true&&\
         node->value.freset.TripValue.assesstripvalue.abserror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.freset.TripValue.assesstripvalue.relerror.errorvalue);
          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }



     else if(node->value.freset.TripValue.assesstripvalue.abserror.valid==true&&\
             node->value.freset.TripValue.assesstripvalue.relerror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
          node->value.freset.TripValue.assesstripvalue.abserror.errorvalue,\
          node->value.freset.TripValue.assesstripvalue.abserror.errorvalue_2);

          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }
     else   if(node->value.freset.TripValue.assesstripvalue.relerror.valid==true&&\
               node->value.freset.TripValue.assesstripvalue.abserror.valid==true)
      {
          if(node->value.freset.TripValue.assesstripvalue.assessandor==1) //ó?μ?1??μ
          {
              sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
              node->value.freset.TripValue.assesstripvalue.relerror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue_2);

              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }
          else if(node->value.freset.TripValue.assesstripvalue.assessandor==2)
          {
              sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
              node->value.freset.TripValue.assesstripvalue.relerror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue_2);
              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }

      }



      strcpy(strbuf,"<TESTITEMEND>");
      fprintf(fp,"%s\n",strbuf);
     }

     if(testitem&0x02&&node->value.freset.TripTime.valid==true)
     {
         strcpy(strbuf,"<TESTITEMBEGIN>");
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.TRIPTIME"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.TripTime.FreEndValue.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"HOLDTIME=%s",node->value.freset.TripTime.HoldTime.text);
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TRIPTIME=0.000");
         fprintf(fp,"%s\n",strbuf);
         if(node->value.freset.TripTime.assesstriptime.relerror.valid==true\
          &&node->value.freset.TripTime.assesstriptime.abserror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.freset.TripTime.assesstriptime.relerror.errorvalue);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }

        else if(node->value.freset.TripTime.assesstriptime.abserror.valid==true\
                &&node->value.freset.TripTime.assesstriptime.relerror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
             node->value.freset.TripTime.assesstriptime.abserror.errorvalue,\
             node->value.freset.TripTime.assesstriptime.abserror.errorvalue_2);


             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
        else   if(node->value.freset.TripTime.assesstriptime.relerror.valid==true&&\
                  node->value.freset.TripTime.assesstriptime.abserror.valid==true)
         {

             if(node->value.freset.TripTime.assesstriptime.assessandor==1) //ó?μ?1??μ
             {
                 sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                 node->value.freset.TripTime.assesstriptime.relerror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue_2);

                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
             else if(node->value.freset.TripTime.assesstriptime.assessandor==2)
             {
                 sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                 node->value.freset.TripTime.assesstriptime.relerror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue_2);



                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
         }


     }


     if(testitem&0x04&&node->value.freset.Tripdfdt.valid==true)
     {
         strcpy(strbuf,"<TESTITEMBEGIN>");
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.TRIPDFDT"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
         fprintf(fp,"%s\n",strbuf);


         sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.Tripdfdt.FreEndValue.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DFDTSTART=%s",node->value.freset.Tripdfdt.DfdtStart.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DFDTEND=%s",node->value.freset.Tripdfdt.DfdtEnd.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DFDTSTEP=%s",node->value.freset.Tripdfdt.DfdtStep.text);
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TRIPDFDT=0.000");
         fprintf(fp,"%s\n",strbuf);
         if(node->value.freset.Tripdfdt.assessdfdttrip.relerror.valid==true&&\
            node->value.freset.Tripdfdt.assessdfdttrip.abserror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSTRIPDFDT=REL(%f)",node->value.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }


         else if(node->value.freset.Tripdfdt.assessdfdttrip.abserror.valid==true&&\
                 node->value.freset.Tripdfdt.assessdfdttrip.relerror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPDFDT=ABS(%f,%f)",\
              node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
              node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_2);


              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }
         else   if(node->value.freset.Tripdfdt.assessdfdttrip.relerror.valid==true&&\
                   node->value.freset.Tripdfdt.assessdfdttrip.abserror.valid==true)
          {
              if(node->value.freset.Tripdfdt.assessdfdttrip.assessandor==1) //ó?μ?1??μ
              {
                  sprintf(strbuf,"ASSESSTRIPDFDT=REL(%f)&ABS(%f,%f)",\
                  node->value.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_2);


                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }
              else if(node->value.freset.Tripdfdt.assessdfdttrip.assessandor==2)
              {
                  sprintf(strbuf,"ASSESSTRIPDFDT=REL(%f)|ABS(%f,%f)",\
                  node->value.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_2);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }

          }


         strcpy(strbuf,"<TESTITEMEND>");
         fprintf(fp,"%s\n",strbuf);
     }

     if(testitem&0x08&&node->value.freset.Tripdfdttime.valid==true)
     {
         strcpy(strbuf,"<TESTITEMBEGIN>");
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.DFDTTRIPTIME"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
         fprintf(fp,"%s\n",strbuf);

         sprintf(strbuf,"DFDT=%s",node->value.freset.Tripdfdttime.dfdt.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.Tripdfdttime.stopfreq.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DURATION=%s",node->value.freset.Tripdfdttime.Duration.text);
         fprintf(fp,"%s\n",strbuf);

         if(node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.valid==true&&\
            node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSDFDTTRIPTIME=REL(%f)",\
             node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.valid!=true&&\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.valid==true)
         {
             sprintf(tmpstr1,"ASSESSDFDTTRIPTIME=ABS(%f,%f)",\
             node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
             node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_2);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.valid==true&&\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.valid==true)
         {

             if(node->value.freset.Tripdfdttime.assessdfdttriptime.assessandor==1) //ó?μ?1??μ
             {
                 sprintf(strbuf,"ASSESSDFDTTRIPTIME=REL(%f)&ABS(%f,%f)",\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_2);


                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
       else if(node->value.freset.Tripdfdttime.assessdfdttriptime.assessandor==2)
             {
                 sprintf(strbuf,"ASSESSDFDTTRIPTIME=REL(%f)|ABS(%f,%f)",\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_2);


                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

         }

        strcpy(strbuf,"<TESTITEMEND>");
        fprintf(fp,"%s\n",strbuf);
     }




     if(testitem&0x10&&node->value.freset.TripVBlock.valid==true)
     {
      strcpy(strbuf,"<TESTITEMBEGIN>");
      fprintf(fp,"%s\n",strbuf);
      strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.VOLTAGEBLOCKING"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"DFDT=%s",node->value.freset.TripVBlock.Dfdt.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.TripVBlock.FreEndValue.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VSTART=%s",node->value.freset.TripVBlock.VStart.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VEND=%s",node->value.freset.TripVBlock.VEnd.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VSTEP=%s",node->value.freset.TripVBlock.VStep.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"HOLDTIME=%s",node->value.freset.TripVBlock.HoldTime.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VSELECT=%d",node->value.freset.TripVBlock.VSelect);
      fprintf(fp,"%s\n",strbuf);
      strcpy(strbuf,"VBLOCKTRIP=0.000");
      fprintf(fp,"%s\n",strbuf);
      if(node->value.freset.TripVBlock.assessvblock.relerror.valid==true\
       &&node->value.freset.TripVBlock.assessvblock.abserror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSVBLOCK=REL(%f)",\
          node->value.freset.TripVBlock.assessvblock.relerror.errorvalue);
          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }

     else if(node->value.freset.TripVBlock.assessvblock.abserror.valid==true\
           &&node->value.freset.TripVBlock.assessvblock.relerror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSVBLOCK=ABS(%f,%f)",\
          node->value.freset.TripVBlock.assessvblock.abserror.errorvalue,\
          node->value.freset.TripVBlock.assessvblock.abserror.errorvalue_2);
          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }
     else if(node->value.freset.TripVBlock.assessvblock.relerror.valid==true&&\
             node->value.freset.TripVBlock.assessvblock.abserror.valid==true)
      {

         if(node->value.freset.TripVBlock.assessvblock.assessandor==1) //ó?μ?1??μ
         {
               sprintf(strbuf,"ASSESSVBLOCK=REL(%f)&ABS(%f,%f)",\
               node->value.freset.TripVBlock.assessvblock.relerror.errorvalue,\
               node->value.freset.TripVBlock.assessvblock.abserror.errorvalue,\
               node->value.freset.TripVBlock.assessvblock.abserror.errorvalue_2);



               fprintf(fp,"%s\n",strbuf);
               sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
               fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.freset.TripVBlock.assessvblock.assessandor==2)
         {
             sprintf(strbuf,"ASSESSVBLOCK=REL(%f)|ABS(%f,%f)",\
             node->value.freset.TripVBlock.assessvblock.relerror.errorvalue,\
             node->value.freset.TripVBlock.assessvblock.abserror.errorvalue,\
             node->value.freset.TripVBlock.assessvblock.abserror.errorvalue_2);



             fprintf(fp,"%s\n",strbuf);
             sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
      }



     strcpy(strbuf,"<TESTITEMEND>");
     fprintf(fp,"%s\n",strbuf);
     }
     sprintf(strbuf,"<TestfunctionEnd>");
     fprintf(fp,"%s\n",strbuf);
   }

    if(node->value.anytestset.valid) //óD·??ò2aê?1|?ü
   { strcpy(strbuf,"<TestfunctionBegin>");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"TESTTYPE=ANYTEST.COM");
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTNAME=%s",node->value.anytestset.name);
     fprintf(fp,"%s\n",strbuf);

     strcpy(strbuf,"<TESTITEMBEGIN>");
     fprintf(fp,"%s\n",strbuf);

     //ò???′òó?3?3ìDò?ùDèòaμ?±?á?
     //VAμ?·ù?μ ?à?? ?μ?ê
     sprintf(strbuf,"VAValue=%s",node->value.anytestset.VAAP.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VAPhase=%s",node->value.anytestset.VAPH.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VAFrec=%s",node->value.anytestset.VAFR.text);
     fprintf(fp,"%s\n",strbuf);
     //VBμ?·ù?μ ?à?? ?μ?ê
     sprintf(strbuf,"VBValue=%s",node->value.anytestset.VBAP.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VBPhase=%s",node->value.anytestset.VBPH.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VBFrec=%s",node->value.anytestset.VBFR.text);
     fprintf(fp,"%s\n",strbuf);
      //VCμ?·ù?μ ?à?? ?μ?ê
     sprintf(strbuf,"VCValue=%s",node->value.anytestset.VCAP.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VCPhase=%s",node->value.anytestset.VCPH.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VCFrec=%s",node->value.anytestset.VCFR.text);
     fprintf(fp,"%s\n",strbuf);
     //VZμ?·ù?μ ?à?? ?μ?ê
    sprintf(strbuf,"VZValue=%s",node->value.anytestset.VZAP.text);
    fprintf(fp,"%s\n",strbuf);
    sprintf(strbuf,"VZPhase=%s",node->value.anytestset.VZPH.text);
    fprintf(fp,"%s\n",strbuf);
    sprintf(strbuf,"VZFrec=%s",node->value.anytestset.VZFR.text);
    fprintf(fp,"%s\n",strbuf);
    //IAμ?·ù?μ ?à?? ?μ?ê
   sprintf(strbuf,"IAValue=%s",node->value.anytestset.IAAP.text);
   fprintf(fp,"%s\n",strbuf);
   sprintf(strbuf,"IAPhase=%s",node->value.anytestset.IAPH.text);
   fprintf(fp,"%s\n",strbuf);
   sprintf(strbuf,"IAFrec=%s",node->value.anytestset.IAFR.text);
   fprintf(fp,"%s\n",strbuf);
   //IBμ?·ù?μ ?à?? ?μ?ê
  sprintf(strbuf,"IBValue=%s",node->value.anytestset.IBAP.text);
  fprintf(fp,"%s\n",strbuf);
  sprintf(strbuf,"IBPhase=%s",node->value.anytestset.IBPH.text);
  fprintf(fp,"%s\n",strbuf);
  sprintf(strbuf,"IBFrec=%s",node->value.anytestset.IBFR.text);
  fprintf(fp,"%s\n",strbuf);
 //ICμ?·ù?μ ?à?? ?μ?ê
 sprintf(strbuf,"ICValue=%s",node->value.anytestset.ICAP.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"ICPhase=%s",node->value.anytestset.ICPH.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"ICFrec=%s",node->value.anytestset.ICFR.text);
 fprintf(fp,"%s\n",strbuf);
 //2?3¤
 sprintf(strbuf,"CurStep=%s",node->value.anytestset.Istep.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VolStep=%s",node->value.anytestset.Vstep.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"PhaStep=%s",node->value.anytestset.Pstep.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"FreStep=%s",node->value.anytestset.Fstep.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"StepTime=%s",node->value.anytestset.Steptime.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"StepNum=%s",node->value.anytestset.Stepnum.text);
 fprintf(fp,"%s\n",strbuf);
 //1ê??ê±??
 sprintf(strbuf,"PreFaultTime=%s",node->value.anytestset.Prefault.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"FaultTime=%s",node->value.anytestset.Infault.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"PostFaultTime=%s",node->value.anytestset.Postfault.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"Faultduration=%s",node->value.anytestset.Faultduraion.text);
 fprintf(fp,"%s\n",strbuf);


 //±??ˉ????
 sprintf(strbuf,"VariatyVA=%s",node->value.anytestset.VariatyVA.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VariatyVB=%s",node->value.anytestset.VariatyVB.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VariatyVC=%s",node->value.anytestset.VariatyVC.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VariatyVZ=%s",node->value.anytestset.VariatyVZ.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VariatyVz3U0=%s",node->value.anytestset.VZ3U0.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VariatyIA=%s",node->value.anytestset.VariatyIA.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VariatyIB=%s",node->value.anytestset.VariatyIB.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"VariatyIC=%s",node->value.anytestset.VariatyIC.text);
 fprintf(fp,"%s\n",strbuf);
 //2aê??￡ê? ′￥·￠?￠×??ˉ
 sprintf(strbuf,"TestMode=%s",node->value.anytestset.Testmode.text);
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"Variableitem=%s",node->value.anytestset.ChangeItem.text);
 fprintf(fp,"%s\n",strbuf);

 strcpy(strbuf,"TRIPTIME=0.000");
 fprintf(fp,"%s\n",strbuf);
 strcpy(strbuf,"<TESTITEMEND>");
 fprintf(fp,"%s\n",strbuf);
 sprintf(strbuf,"<TestfunctionEnd>");
 fprintf(fp,"%s\n",strbuf);
   }

    if(node->value.phraseset.valid)
    {

          strcpy(strbuf,"<TestfunctionBegin>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTTYPE=VECTORSHIFT.COM"); //ê?μ?á÷2aê?
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TESTNAME=%s",node->value.phraseset.name);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TESTITEM=%s",node->value.phraseset.Testitem.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"FAULTDURATION=%s",node->value.phraseset.Faultduraion.text);
          fprintf(fp,"%s\n",strbuf);

          sprintf(strbuf,"FIXVOLTAGE=%s",node->value.phraseset.FixVoltage.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"FIXCURRENT=%s",node->value.phraseset.FixCurrent.text);
          fprintf(fp,"%s\n",strbuf);


          if(node->value.phraseset.IAch)
           fprintf(fp,"IACH=1");
          else fprintf(fp,"IACH=0");
           fprintf(fp,"\n");


          if(node->value.phraseset.IBch)
          fprintf(fp,"IBCH=1");
          else fprintf(fp,"IBCH=0");
          fprintf(fp,"\n");


          if(node->value.phraseset.ICch)
          fprintf(fp,"ICCH=1");
          else fprintf(fp,"ICCH=0");
         fprintf(fp,"\n");


          if(node->value.phraseset.VAch)
          fprintf(fp,"VACH=1");
          else
          fprintf(fp,"VACH=0");
          fprintf(fp,"\n");


          if(node->value.phraseset.VBch)
          fprintf(fp,"VBCH=1");
          else fprintf(fp,"VBCH=0");
        fprintf(fp,"\n");


          if(node->value.phraseset.VCch)
          fprintf(fp,"VCCH=1");
          else fprintf(fp,"VCCH=0");
          fprintf(fp,"\n");

          int testitem=atoi(node->value.phraseset.Testitem.text);
          if(testitem&0x01&&node->value.phraseset.TripValue.valid==true)
          {
             strcpy(strbuf,"<TESTITEMBEGIN>");
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TESTITEMNAME=VECTORSHIFT.TRIPVALUE"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PENDSTART=%s",node->value.phraseset.TripValue.StartValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PENDEND=%s",node->value.phraseset.TripValue.EndValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PSTEP=%s",node->value.phraseset.TripValue.StepValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HOLDTIME=%s",node->value.phraseset.TripValue.HoldTime.text);
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TRIPVALUE=0.000");
             fprintf(fp,"%s\n",strbuf);
             //′￥·￠?μ?à1à
             if(node->value.phraseset.TripValue.assesstripvalue.relerror.valid==true&&node->value.phraseset.TripValue.assesstripvalue.abserror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.phraseset.TripValue.assesstripvalue.relerror.errorvalue);
                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

             else if(node->value.phraseset.TripValue.assesstripvalue.abserror.valid==true&&node->value.phraseset.TripValue.assesstripvalue.relerror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
                 node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
                 node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue_2);



                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }


             else  if(node->value.phraseset.TripValue.assesstripvalue.relerror.valid==true&&\
                node->value.phraseset.TripValue.assesstripvalue.abserror.valid==true)
             {

                 if(node->value.phraseset.TripValue.assesstripvalue.assessandor==1) //ó?μ?1??μ
                 {
                      sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                      node->value.phraseset.TripValue.assesstripvalue.relerror.errorvalue,\
                      node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
                      node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue_2);


                      fprintf(fp,"%s\n",strbuf);
                      sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                      fprintf(fp,"%s\n",tmpstr1);
                 }
                 else if(node->value.phraseset.TripValue.assesstripvalue.assessandor==2)
                 {
                     sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                     node->value.phraseset.TripValue.assesstripvalue.relerror.errorvalue,\
                     node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
                     node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue_2);
                     fprintf(fp,"%s\n",strbuf);
                     sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                     fprintf(fp,"%s\n",tmpstr1);
                 }
             }


             strcpy(strbuf,"<TESTITEMEND>");
             fprintf(fp,"%s\n",strbuf);
          }
          if(testitem&0x02&&node->value.phraseset.TripTime.valid==true)
          {
             strcpy(strbuf,"<TESTITEMBEGIN>");
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TESTITEMNAME=VECTORSHIFT.TRIPTIME"); //ó?óú??·? ê??ˉ×÷?μ2aê? ?1ê??ˉ×÷ê±??2aê?
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PEND=%s",node->value.phraseset.TripTime.EndVaule.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HOLDTIME=%s",node->value.phraseset.TripTime.HoldTime.text);
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TRIPTIME=0.000");
             fprintf(fp,"%s\n",strbuf);
             if(node->value.phraseset.TripTime.assesstriptime.relerror.valid==true&&node->value.phraseset.TripTime.assesstriptime.abserror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.phraseset.TripTime.assesstriptime.relerror.errorvalue);
                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.phraseset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

             else if(node->value.phraseset.TripTime.assesstriptime.abserror.valid==true&&node->value.phraseset.TripTime.assesstriptime.relerror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
                 node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue_2);



                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.phraseset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
            else  if(node->value.phraseset.TripTime.assesstriptime.relerror.valid==true&&\
                node->value.phraseset.TripTime.assesstriptime.abserror.valid==true)
             {

                 if(node->value.phraseset.TripTime.assesstriptime.assessandor==1) //ó?μ?1??μ
                 {
                      sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                      node->value.phraseset.TripTime.assesstriptime.relerror.errorvalue,\
                      node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                      node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue_2);
                      fprintf(fp,"%s\n",strbuf);
                 }
                 else if(node->value.phraseset.TripTime.assesstriptime.assessandor==2)
                 {
                     sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                     node->value.phraseset.TripTime.assesstriptime.relerror.errorvalue,\
                     node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                     node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue_2);

                     fprintf(fp,"%s\n",strbuf);
                 }
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.phraseset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }


             strcpy(strbuf,"<TESTITEMEND>");
             fprintf(fp,"%s\n",strbuf);
          }
          sprintf(strbuf,"<TestfunctionEnd>");
          fprintf(fp,"%s\n",strbuf);

    }
    if(node->value.harmonicset.valid)
    {

          strcpy(strbuf,"<TestfunctionBegin>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTTYPE=HARMONIC.COM"); //谐波 2016-12-13
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TESTNAME=%s",node->value.harmonicset.name);
          fprintf(fp,"%s\n",strbuf);

          sprintf(strbuf,"COUNT=%s",node->value.harmonicset.HarmonicCount.text);
          fprintf(fp,"%s\n",strbuf);

          sprintf(strbuf,"CURRENT=%s",node->value.harmonicset.Current.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"PHASE=%s",node->value.harmonicset.Phase.text);
          fprintf(fp,"%s\n",strbuf);


          if(node->value.harmonicset.IAch)
           fprintf(fp,"IACH=1");
          else fprintf(fp,"IACH=0");
           fprintf(fp,"\n");


          if(node->value.harmonicset.IBch)
          fprintf(fp,"IBCH=1");
          else fprintf(fp,"IBCH=0");
          fprintf(fp,"\n");


          if(node->value.harmonicset.ICch)
          fprintf(fp,"ICCH=1");
          else fprintf(fp,"ICCH=0");
         fprintf(fp,"\n");


          if(node->value.harmonicset.TripValue.valid==true)
          {
             strcpy(strbuf,"<TESTITEMBEGIN>");
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TESTITEMNAME=HARMONIC.TRIPVALUE"); //HARMONIC
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HARMONICSTART=%s",node->value.harmonicset.TripValue.StartValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HARMONICEND=%s",node->value.harmonicset.TripValue.EndValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"STEP=%s",node->value.harmonicset.TripValue.StepValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HOLDTIME=%s",node->value.harmonicset.TripValue.HoldTime.text);
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TRIPVALUE=0.000");//?????????????2016-12-14
             fprintf(fp,"%s\n",strbuf);
             //′￥·￠?μ?à1à
             if(node->value.harmonicset.TripValue.assesstripvalue.relerror.valid==true&&node->value.harmonicset.TripValue.assesstripvalue.abserror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.harmonicset.TripValue.assesstripvalue.relerror.errorvalue);
                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

             else if(node->value.harmonicset.TripValue.assesstripvalue.abserror.valid==true&&node->value.harmonicset.TripValue.assesstripvalue.relerror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
                 node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
                 node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_2);



                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }


             else  if(node->value.harmonicset.TripValue.assesstripvalue.relerror.valid==true&&\
                node->value.harmonicset.TripValue.assesstripvalue.abserror.valid==true)
             {

                 if(node->value.harmonicset.TripValue.assesstripvalue.assessandor==1) //ó?μ?1??μ
                 {
                      sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                      node->value.harmonicset.TripValue.assesstripvalue.relerror.errorvalue,\
                      node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
                      node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_2);


                      fprintf(fp,"%s\n",strbuf);
                      sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                      fprintf(fp,"%s\n",tmpstr1);
                 }
                 else if(node->value.harmonicset.TripValue.assesstripvalue.assessandor==2)
                 {
                     sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                     node->value.harmonicset.TripValue.assesstripvalue.relerror.errorvalue,\
                     node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
                     node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_2);
                     fprintf(fp,"%s\n",strbuf);
                     sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                     fprintf(fp,"%s\n",tmpstr1);
                 }
             }


             strcpy(strbuf,"<TESTITEMEND>");
             fprintf(fp,"%s\n",strbuf);
          }
           sprintf(strbuf,"<TestfunctionEnd>");
          fprintf(fp,"%s\n",strbuf);

    }


}


void Widget::CreatAuthorizeFile(char *Filename)
{


  //  QString AuthorizedSerialNum,AuthorizedFilename,AuthorizedTime,AuthorizedExpireTime;
  //  QString fileinfo_modifiedtime,fileinfo_filesize;

    /*********************
     加密文件 有几个部分组成
     1.序列号
     2.授权的日期：
     3.使用的天数
     4.校验码
     **************************/
    char Tempfilename[512]; //
    char Tempfilenametmp[512];
    FILE *fp;
    fp=fopen(Filename,"w+");
    if(fp==NULL)
    {
        return;
    }
    char strbuf[1024];
    char miwen[1024];

    char key1=0x13;
    unsigned long CrcNum=0;

    int Templatefilelen=0;
    Templatefilelen=strlen(Filename)-strlen(".dat");
    for(int i=0;i<Templatefilelen;i++)
        Tempfilename[i]=Filename[i];


    Tempfilename[Templatefilelen]='\0';
    sprintf(Tempfilenametmp,"%s.mtp",Tempfilename);




    QByteArray ba=authorizedfileinfo.Serailnum.toLatin1();
    char *serialnum=ba.data();
    sprintf(strbuf,"SERIALNUM=%s",serialnum);
    CrcNum+=Check(strbuf);
    jiamiXor(strbuf,key1,miwen);
    fprintf(fp,"%s\n",miwen);

    char newAuthorizeFilename[512];
    int  newAuthorizeFilelen=0;

    if(QString(Filename).contains('/')==true)
    {

        int lastindex=QString(Filename).lastIndexOf('/');
        qDebug()<<__func__<<__LINE__<<Filename<<lastindex;

        QString name=QString(Filename);

        for(int i=lastindex+1;i<name.length();i++)
          {  newAuthorizeFilename[newAuthorizeFilelen]=name.at(i).toLatin1();
             newAuthorizeFilelen++;
          }
        newAuthorizeFilename[newAuthorizeFilelen]='\0';




    }
    else   if(QString(Filename).contains('\\')==true)
    {

        int lastindex=QString(Filename).lastIndexOf('\\');

        QString name=QString(Filename);
        for(int i=lastindex+1;i<name.length();i++)
          {  newAuthorizeFilename[newAuthorizeFilelen]=name.at(i).toLatin1();
              newAuthorizeFilelen++;
          }
        newAuthorizeFilename[newAuthorizeFilelen]='\0';
    }



    sprintf(strbuf,"FILENAME=%s",newAuthorizeFilename);
    CrcNum+=Check(strbuf);
    jiamiXor(strbuf,key1,miwen);
    fprintf(fp,"%s\n",miwen);

    QByteArray baauthtime=authorizedfileinfo.Authorizetime.toLatin1();
    char *strauthtime=baauthtime.data();

    sprintf(strbuf,"AUTHORIZETIME=%s",strauthtime);
    CrcNum+=Check(strbuf);
    jiamiXor(strbuf,key1,miwen);
    fprintf(fp,"%s\n",miwen);

    QByteArray baexpiretime=authorizedfileinfo.Expiretime.toLatin1();
    char *strexpiretime=baexpiretime.data();

    sprintf(strbuf,"EXPIRETIME=%s",strexpiretime);
    CrcNum+=Check(strbuf);
    jiamiXor(strbuf,key1,miwen);
    fprintf(fp,"%s\n",miwen);





    QFileInfo fileinfo(Tempfilenametmp);

    QDateTime lastmodified=fileinfo.lastModified().toUTC();

    unsigned  long filesize=fileinfo.size();
    QString strmodified=QString("MODIFIEDTIME=")+lastmodified.toString("yyyy-M-d h:m:s");



    QByteArray ba2=strmodified.toLatin1();
    char *chstrmodified=ba2.data();
    CrcNum+=Check(chstrmodified);

    jiamiXor(chstrmodified,key1,miwen); //更改时间加密
    fprintf(fp,"%s\n",miwen);

    sprintf(strbuf,"FILESIZE=%ld",filesize);

    CrcNum+=Check(strbuf);
    jiamiXor(strbuf,key1,miwen);
    fprintf(fp,"%s\n",miwen);

    sprintf(strbuf,"CHECKCODE=%ld",CrcNum);
    jiamiXor(strbuf,key1,miwen);
    fprintf(fp,"%s\n",miwen);
    fclose(fp);
}



void Widget::on_pushButton_7_clicked()
{


     FILE *fp;
     int testnum=1;
     QString  savefilename=QFileDialog::getSaveFileName(0,QString(QLatin1String("Save Setting File!")),QString(QLatin1String("")),QString(QLatin1String("*.mtp")));
     if(savefilename.isEmpty())
         return;

     QByteArray ba=savefilename.toLatin1();
     char *filename=ba.data();

     fp=fopen(filename,"w+");
     char strbuf[1024];
     if(fp==NULL)
     {
         return;
     }
     struct SECTMENTSETNODE *next;
     if(nodelink==NULL)
        {
         fclose(fp);
         return;
        }
      setsettinglinknoprint(settinglink);
      next=nodelink->next;


      if(settinglink==NULL)
          return;

      struct SETTINGNODELIST *settingnext=settinglink->next;

      strcpy(strbuf,"<SETTINGBEGIN>");
      fprintf(fp,"%s\n",strbuf);
      while(settingnext!=NULL)
      {
       sprintf(strbuf,"ID=%s,NAME=%s,VALUE=%s",\
                       settingnext->set.ID,\
                       settingnext->set.name,\
                       settingnext->set.value);
                      // settingnext->set.ref);
       fprintf(fp,"%s\n",strbuf);

       settingnext=settingnext->next;
      }
      strcpy(strbuf,"<SETTINGEND>");
      fprintf(fp,"%s\n",strbuf);



      while(next!=NULL)
     {
        strcpy(strbuf,"<SectionsettingBegin>");
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"sectionname:%s",next->value.name);
        fprintf(fp,"%s\n",strbuf);
        if(next->value.curset.valid==true)
        { sprintf(strbuf,"TESTNAME%d:%s",testnum,next->value.curset.name);
          testnum++;
          fprintf(fp,"%s\n",strbuf);
        }
        if(next->value.volset.valid==true)
        { sprintf(strbuf,"TESTNAME%d:%s",testnum,next->value.volset.name);
          testnum++;
          fprintf(fp,"%s\n",strbuf);
        }
        if(next->value.dirset.valid==true)
        { sprintf(strbuf,"TESTNAME%d:%s",testnum,next->value.dirset.name);
          testnum++;
          fprintf(fp,"%s\n",strbuf);
        }
        if(next->value.freset.valid==true)
        { sprintf(strbuf,"TESTNAME%d:%s",testnum,next->value.freset.name);
          testnum++;
          fprintf(fp,"%s\n",strbuf);
        }
        if(next->value.anytestset.valid==true)
        { sprintf(strbuf,"TESTNAME%d:%s",testnum,next->value.anytestset.name);
          testnum++;
          fprintf(fp,"%s\n",strbuf);
        }
        if(next->value.phraseset.valid==true)
        { sprintf(strbuf,"TESTNAME%d:%s",testnum,next->value.phraseset.name);
          testnum++;
          fprintf(fp,"%s\n",strbuf);
        }
        if(next->value.harmonicset.valid==true)
        { sprintf(strbuf,"TESTNAME%d:%s",testnum,next->value.harmonicset.name);
          testnum++;
          fprintf(fp,"%s\n",strbuf);
        }

         Writetestfunc(next,fp);

        strcpy(strbuf,"<SectionsettingEnd>");
        fprintf(fp,"%s\n",strbuf);
       next=next->next;
       testnum=1;
      }
      fclose(fp);



      if(AuthorizedFileValid==true)
    {  char Authfilename[512];
      char Authfilenametmp[512];
      int Templatefilelen=0;
      Templatefilelen=strlen(filename)-strlen(".mtp");
      for(int i=0;i<Templatefilelen;i++)
          Authfilename[i]=filename[i];


      Authfilename[Templatefilelen]='\0';
      sprintf(Authfilenametmp,"%s.dat",Authfilename);
      CreatAuthorizeFile(Authfilenametmp);
      AuthorizedFileValid=false;

    }

      AuthorizedFileValid=false;
      //清楚保存的模板
      nodelink->next=NULL;
      settinglink->next=NULL;
      ui->listWidget_2->clear();
      currentid=0;
      return;
}


//?÷3ìDò  ?üD??ùóDμ?ID

void Widget::updatesettinglink(void *setting)
{
    settinglink=(struct SETTINGNODELIST *)setting;
    struct SETTINGNODELIST *next;
    next=settinglink->next;
    while(next!=NULL)
    {
        next=next->next;
    }

}



void Widget::on_pushButton_10_clicked()
{
    settingmanage=new Settingmanage;
    connect(settingmanage,SIGNAL(savesettingnode(void*)),this,SLOT(updatesettinglink(void*)));
    settingmanage->getsettinglist(settinglink);
    settingmanage->show();
}







void Widget::on_pushButton_8_clicked()
{

    close();
}
void Widget::closeEvent(QCloseEvent *e)
{
    QMessageBox message(QMessageBox::Warning,"Warning","Really want to quit?",QMessageBox::Yes|QMessageBox::No,NULL);
    message.setIcon(QMessageBox::Warning);

    if(message.exec()==QMessageBox::Yes)
    {
    free(nodelink);
    free(settinglink);
    nodelink=NULL;
    settinglink=NULL;
    e->accept();
    }
    else
    {
        e->ignore();
    }
}





void Widget::processrelerror(int type,int subtype,int relabs,float value,float value_2,struct SECTMENTSET &tmpsectset)
{
        switch(type)
        {
        case 1://电流

            switch(subtype)
            {
            case 1:
                if(relabs==1)
                 {  tmpsectset.curset.TripValue.assesstripvalue.relerror.errorvalue=value;
                    tmpsectset.curset.TripValue.assesstripvalue.relerror.valid=true;
                 }
                 else if(relabs==2)
                {
                    tmpsectset.curset.TripValue.assesstripvalue.abserror.errorvalue=value;
                    tmpsectset.curset.TripValue.assesstripvalue.abserror.errorvalue_2=value_2;

                    tmpsectset.curset.TripValue.assesstripvalue.abserror.valid=true;
                }
                else if(relabs>=3)
                    tmpsectset.curset.TripValue.assesstripvalue.assessandor=relabs-2;

                break;
            case 2:
                if(relabs==1) //?à???ó2?
                 {  tmpsectset.curset.TripTime.assesstriptime.relerror.errorvalue=value;
                    tmpsectset.curset.TripTime.assesstriptime.relerror.valid=true;
                 }
                 else if(relabs==2)
                {
                    tmpsectset.curset.TripTime.assesstriptime.abserror.errorvalue=value;
                    tmpsectset.curset.TripTime.assesstriptime.abserror.errorvalue_2=value_2;


                    tmpsectset.curset.TripTime.assesstriptime.abserror.valid=true;

                }
                else if(relabs>=3)
                    tmpsectset.curset.TripTime.assesstriptime.assessandor=relabs-2;
                break;
          }

            break;
        case 2://电压
            switch(subtype)
            {
            case 1:
                if(relabs==1) //?à???ó2?
                 {  tmpsectset.volset.TripValue.assesstripvalue.relerror.errorvalue=value;
                    tmpsectset.volset.TripValue.assesstripvalue.relerror.valid=true;
                 }
                 else if(relabs==2)
                {
                    tmpsectset.volset.TripValue.assesstripvalue.abserror.errorvalue=value;
                    tmpsectset.volset.TripValue.assesstripvalue.abserror.errorvalue_2=value_2;


                    tmpsectset.volset.TripValue.assesstripvalue.abserror.valid=true;

                }
                else if(relabs>=3)
                tmpsectset.volset.TripValue.assesstripvalue.assessandor=relabs-2;
                break;
            case 2:
                if(relabs==1) //?à???ó2?
                 {  tmpsectset.volset.TripTime.assesstriptime.relerror.errorvalue=value;
                    tmpsectset.volset.TripTime.assesstriptime.relerror.valid=true;
                 }
                 else if(relabs==2)
                {
                    tmpsectset.volset.TripTime.assesstriptime.abserror.errorvalue=value;
                    tmpsectset.volset.TripTime.assesstriptime.abserror.errorvalue_2=value_2;


                    tmpsectset.volset.TripTime.assesstriptime.abserror.valid=true;
                }
                else if(relabs>=3)
                tmpsectset.volset.TripTime.assesstriptime.assessandor=relabs-2;
                break;
          }

            break;

        case 3://方向
                if(relabs==1) //?à???ó2?
                 {  tmpsectset.dirset.TripMta.assessmta.relerror.errorvalue=value;
                    tmpsectset.dirset.TripMta.assessmta.relerror.valid=true;
                 }
                else if(relabs==2)
                {
                    tmpsectset.dirset.TripMta.assessmta.abserror.errorvalue=value;
                    tmpsectset.dirset.TripMta.assessmta.abserror.errorvalue_2=value_2;


                    tmpsectset.dirset.TripMta.assessmta.abserror.valid=true;
                }
                else if(relabs>=3)
                     tmpsectset.dirset.TripMta.assessmta.assessandor=relabs-2;

                break;
        case 4://频率
            switch(subtype)
            {
            case 1: //?ˉ×÷?μ?ê
                if(relabs==1)
                {
                 tmpsectset.freset.TripValue.assesstripvalue.relerror.errorvalue=value;
                 tmpsectset.freset.TripValue.assesstripvalue.relerror.valid=true;
                }
                else if(relabs==2)
                {
                tmpsectset.freset.TripValue.assesstripvalue.abserror.errorvalue=value;
                tmpsectset.freset.TripValue.assesstripvalue.abserror.errorvalue_2=value_2;


                tmpsectset.freset.TripValue.assesstripvalue.abserror.valid=true;



                }
                else if(relabs>=3)
                 tmpsectset.freset.TripValue.assesstripvalue.assessandor=relabs-2;
                break;

            case 2: //?ˉ×÷ê±??
                if(relabs==1)
                {
                 tmpsectset.freset.TripTime.assesstriptime.relerror.errorvalue=value;
                 tmpsectset.freset.TripTime.assesstriptime.relerror.valid=true;
                }
                else if(relabs==2)
                {
                tmpsectset.freset.TripTime.assesstriptime.abserror.errorvalue=value;
                tmpsectset.freset.TripTime.assesstriptime.abserror.errorvalue_2=value_2;



                tmpsectset.freset.TripTime.assesstriptime.abserror.valid=true;
                }
                else if(relabs>=3)
                tmpsectset.freset.TripTime.assesstriptime.assessandor=relabs-2;

                break;


           case 4:
                if(relabs==1)
                {
                 tmpsectset.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue=value;
                 tmpsectset.freset.Tripdfdt.assessdfdttrip.relerror.valid=true;
                }
                else if(relabs==2)
                {
                tmpsectset.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue=value;
                tmpsectset.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_2=value_2;


                tmpsectset.freset.Tripdfdt.assessdfdttrip.abserror.valid=true;
                }
                else if(relabs>=3)
                tmpsectset.freset.Tripdfdt.assessdfdttrip.assessandor=relabs-2;

                break;
           case 8:
                 if(relabs==1)
                 {
                  tmpsectset.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue=value;
                  tmpsectset.freset.Tripdfdttime.assessdfdttriptime.relerror.valid=true;
                 }
                 else if(relabs==2)
                 {
                 tmpsectset.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue=value;
                 tmpsectset.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_2=value_2;



                 tmpsectset.freset.Tripdfdttime.assessdfdttriptime.abserror.valid=true;
                 }
                 else if(relabs>=3)
                 tmpsectset.freset.Tripdfdttime.assessdfdttriptime.assessandor=relabs-2;

                 break;

            case 16: //μ??1±????μ
                if(relabs==1)
                {
                 tmpsectset.freset.TripVBlock.assessvblock.relerror.errorvalue=value;
                 tmpsectset.freset.TripVBlock.assessvblock.relerror.valid=true;
                }
                else if(relabs==2)
                {
                tmpsectset.freset.TripVBlock.assessvblock.abserror.errorvalue=value;
                tmpsectset.freset.TripVBlock.assessvblock.abserror.errorvalue_2=value_2;



                tmpsectset.freset.TripVBlock.assessvblock.abserror.valid=true;
                }
                else if(relabs>=3)
                tmpsectset.freset.TripVBlock.assessvblock.assessandor=relabs-2;
                break;
            }
            break;

        case 6://相位

            switch(subtype)
            {
            case 1:
                if(relabs==1) //?à???ó2?
                 {  tmpsectset.phraseset.TripValue.assesstripvalue.relerror.errorvalue=value;
                    tmpsectset.phraseset.TripValue.assesstripvalue.relerror.valid=true;
                 }
                 else if(relabs==2)
                {
                    tmpsectset.phraseset.TripValue.assesstripvalue.abserror.errorvalue=value;
                    tmpsectset.phraseset.TripValue.assesstripvalue.abserror.errorvalue_2=value_2;



                    tmpsectset.phraseset.TripValue.assesstripvalue.abserror.valid=true;
                }
                else if(relabs>=3)
                    tmpsectset.phraseset.TripValue.assesstripvalue.assessandor=relabs-2;

                break;
            case 2:
                if(relabs==1) //?à???ó2?
                 {  tmpsectset.phraseset.TripTime.assesstriptime.relerror.errorvalue=value;
                    tmpsectset.phraseset.TripTime.assesstriptime.relerror.valid=true;
                 }
                 else if(relabs==2)
                {
                    tmpsectset.phraseset.TripTime.assesstriptime.abserror.errorvalue=value;
                    tmpsectset.phraseset.TripTime.assesstriptime.abserror.errorvalue_2=value_2;

                    tmpsectset.phraseset.TripTime.assesstriptime.abserror.valid=true;
                }
                else if(relabs>=3)
                    tmpsectset.phraseset.TripTime.assesstriptime.assessandor=relabs-2;
                break;
          }
            break;
        case 7://谐波
            if(relabs==1) //?à???ó2?
             {  tmpsectset.harmonicset.TripValue.assesstripvalue.relerror.errorvalue=value;
                tmpsectset.harmonicset.TripValue.assesstripvalue.relerror.valid=true;
             }
             else if(relabs==2)
            {
                tmpsectset.harmonicset.TripValue.assesstripvalue.abserror.errorvalue=value;
                tmpsectset.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_2=value_2;



                tmpsectset.harmonicset.TripValue.assesstripvalue.abserror.valid=true;
            }
            else if(relabs>=3)
                tmpsectset.harmonicset.TripValue.assesstripvalue.assessandor=relabs-2;

                break;
        }

}



void Widget::AnalysisAssesschar(char *assesschar,int type,int subtype,struct SECTMENTSET &sectsettmp)
{
    char *p1=assesschar;
    char *p2=assesschar;
    int  pos=0; //'|' ?ò'&'μ?????
    float relerror;
    float abserror; //?y?ò?ó2?
    float abserror_2; //·′?ò?ó2?
    while(*p1!='\0')
    {
        if((*p1=='|')||(*p1=='&'))
        break;
        p1++;
        pos++;
    }
    if(*p1=='\0') //??óDó? ?ò?? |  &
    {
        if(memcmp(p2,"REL(",4)==0)
        {
            p2=p2+4;
            char value[50];
            int i=0;
            while(*p2!=')')
            {
                value[i]=*p2;
                p2++;
                i++;
            }
            value[i]='\0';
            relerror=atof(value);
            processrelerror(type,subtype,1,relerror,0,sectsettmp);
        }
        else if(memcmp(p2,"ABS(",4)==0)
        {
            p2=p2+4;
            char value[50];
            int i=0;
            while(*p2!=',')
            {
                value[i]=*p2;
                p2++;
                i++;
            }
            value[i]='\0';
            abserror=atof(value);
            p2++;
            i=0;
            while(*p2!=')')
            {
                value[i]=*p2;
                p2++;
                i++;
            }
            value[i]='\0';
            abserror_2=atof(value);
            processrelerror(type,subtype,2,abserror,abserror_2,sectsettmp);

        }

    }
    else if(*p1=='|')
    {
      p1=p1+1;
      if(memcmp(p2,"REL(",4)==0)
      {
          p2=p2+4;
          char value[50];
          int i=0;
          while(*p2!=')')
          {
              value[i]=*p2;
              p2++;
              i++;
          }
          value[i]='\0';
          relerror=atof(value);

      }
      else if(memcmp(p2,"ABS(",4)==0)
      {
          p2=p2+4;
          char value[50];
          int i=0;
          while(*p2!=',')
          {
              value[i]=*p2;
              p2++;
              i++;
          }
          value[i]='\0';
          abserror=atof(value);
          p2++;
          i=0;
          while(*p2!=')')
          {
              value[i]=*p2;
              p2++;
              i++;
          }
          value[i]='\0';
          abserror_2=atof(value);

      }

      if(memcmp(p1,"REL(",4)==0)
      {
          p1=p1+4;
          char value[50];
          int i=0;
          while(*p1!=')')
          {
              value[i]=*p1;
              p1++;
              i++;
          }
          value[i]='\0';
          relerror=atof(value);

      }
      else if(memcmp(p1,"ABS(",4)==0)
      {
          p1=p1+4;
           char value[50];
           int i=0;
           while(*p1!=',')
           {
               value[i]=*p1;
               p1++;
               i++;
           }
           value[i]='\0';
           abserror=atof(value);
           p1++;
           i=0;
           while(*p1!=')')
           {
               value[i]=*p1;
               p1++;
               i++;
           }
           value[i]='\0';
           abserror_2=atof(value);


      }

      processrelerror(type,subtype,1,relerror,0,sectsettmp);
      processrelerror(type,subtype,2,abserror,abserror_2,sectsettmp);
      processrelerror(type,subtype,4,abserror,abserror_2,sectsettmp);




    }
    else if(*p1=='&')
    {
      p1=p1+1;
      if(memcmp(p2,"REL(",4)==0)
      {
          p2=p2+4;
          char value[50];
          int i=0;
          while(*p2!=')')
          {
              value[i]=*p2;
              p2++;
              i++;
          }
          value[i]='\0';
          relerror=atof(value);
      }
      else if(memcmp(p2,"ABS(",4)==0)
      {
          p2=p2+4;
          char value[50];
          int i=0;
          while(*p2!=',')
          {
              value[i]=*p2;
              p2++;
              i++;
          }
          value[i]='\0';
          abserror=atof(value);
          p2++;
          i=0;
          while(*p2!=')')
          {
              value[i]=*p2;
              p2++;
              i++;
          }
          value[i]='\0';
          abserror_2=atof(value);
      }

      if(memcmp(p1,"REL(",4)==0)
      {
          p1=p1+4;
          char value[50];
          int i=0;
          while(*p1!=')')
          {
              value[i]=*p1;
              p1++;
              i++;
          }
          value[i]='\0';
          relerror=atof(value);
      }
      else if(memcmp(p1,"ABS(",4)==0)
      {
          p1=p1+4;
           char value[50];
           int i=0;
           while(*p1!=',')
           {
               value[i]=*p1;
               p1++;
               i++;
           }
           value[i]='\0';
           abserror=atof(value);
           p1++;
           i=0;
           while(*p1!=')')
           {
               value[i]=*p1;
               p1++;
               i++;
           }
           value[i]='\0';
           abserror_2=atof(value);
      }
      processrelerror(type,subtype,1,relerror,0,sectsettmp);
      processrelerror(type,subtype,2,abserror,abserror_2,sectsettmp);
      processrelerror(type,subtype,3,abserror,abserror_2,sectsettmp);
    }

}


void Widget::ReadCurrentVariable(char *buf,struct SECTMENTSET &sectset,int type) //?áμ?á÷μ?±?á?
{
    char *p1=buf;
    int i=0;
    char Testname[128];
    switch(type)
    {
    case 1: //′￥·￠?μ


        if(memcmp(p1,"IENDSTART=",10)==0)
        {
            p1=p1+10;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.curset.TripValue.StartValue.text,Testname);
        }
        else if(memcmp(p1,"IENDEND=",8)==0)
        {
            p1=p1+8;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.curset.TripValue.EndValue.text,Testname);
        }
        else if(memcmp(p1,"ISTEP=",6)==0)
        {
            p1=p1+6;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.curset.TripValue.StepValue.text,Testname);
        }
        else if(memcmp(p1,"HOLDTIME=",9)==0)
        {
            p1=p1+9;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.curset.TripValue.HoldTime.text,Testname);
        }
        else if(memcmp(p1,"ASSESSTRIPVALUE=",16)==0)
        {
            p1=p1+16;
            char Assessmethod[128];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Assessmethod[i]=*p1;
                i++;
                p1++;
            }
            Assessmethod[i]='\0';
            AnalysisAssesschar(Assessmethod,1,1,sectset);

        }
        else if(memcmp(p1,"ASSESSTRIPVALUEID=",18)==0)
        {
            p1=p1+18;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.curset.TripValue.assesstripvalue.CompareID,Testname);
        }

        break;
    case 2: //′￥·￠ê±??

        if(memcmp(p1,"IEND=",5)==0)
        {
            p1=p1+5;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.curset.TripTime.EndVaule.text,Testname);
        }
       else  if(memcmp(p1,"HOLDTIME=",9)==0)
         {
             p1=p1+9;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.curset.TripTime.HoldTime.text,Testname);
         }
       else  if(memcmp(p1,"ASSESSTRIPTIME=",15)==0)
         {
             p1=p1+15;
             char Assessmethod[128];
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Assessmethod[i]=*p1;
                 i++;
                 p1++;
             }
             Assessmethod[i]='\0';
             AnalysisAssesschar(Assessmethod,1,2,sectset);
         }

       else  if(memcmp(p1,"ASSESSTRIPTIMEID=",17)==0)
        {
            p1=p1+17;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.curset.TripTime.assesstriptime.CompareID,Testname);
        }
       break;
    }

}

void Widget::ReadVoltageVariable(char *buf,struct SECTMENTSET &sectset,int type) //?áμ??1μ?±?á?
{
    char *p1=buf;
    int i=0;
    char Testname[128];
    switch(type)
    {
    case 1: //′￥·￠?μ
         if(memcmp(p1,"VENDSTART=",10)==0)
        {
            p1=p1+10;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.volset.TripValue.StartValue.text,Testname);
        }
        else if(memcmp(p1,"VENDEND=",8)==0)
        {
            p1=p1+8;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.volset.TripValue.EndValue.text,Testname);
        }
        else if(memcmp(p1,"VSTEP=",6)==0)
        {
            p1=p1+6;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.volset.TripValue.StepValue.text,Testname);
        }
        else if(memcmp(p1,"HOLDTIME=",9)==0)
        {
            p1=p1+9;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.volset.TripValue.HoldTime.text,Testname);
        }
        else if(memcmp(p1,"ASSESSTRIPVALUE=",16)==0)
        {
            p1=p1+16;
            char Assessmethod[128];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Assessmethod[i]=*p1;
                i++;
                p1++;
            }
            Assessmethod[i]='\0';
            AnalysisAssesschar(Assessmethod,2,1,sectset);
        }
        else if(memcmp(p1,"ASSESSTRIPVALUEID=",18)==0)
        {
            p1=p1+18;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.volset.TripValue.assesstripvalue.CompareID,Testname);
        }

        break;
    case 2: //′￥·￠ê±??




       if(memcmp(p1,"VEND=",5)==0)
        {
            p1=p1+5;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.volset.TripTime.EndVaule.text,Testname);
        }

       else if(memcmp(p1,"USEMEASUREDVALUE=",strlen("USEMEASUREDVALUE="))==0)
        {
            p1=p1+strlen("USEMEASUREDVALUE=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            sectset.volset.TripTime.UseMeasureValue=atoi(Testname);
        }

       else  if(memcmp(p1,"HOLDTIME=",9)==0)
         {
             p1=p1+9;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.volset.TripTime.HoldTime.text,Testname);
         }
       else  if(memcmp(p1,"ASSESSTRIPTIME=",15)==0)
         {
             p1=p1+15;
             char Assessmethod[128];
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Assessmethod[i]=*p1;
                 i++;
                 p1++;
             }
             Assessmethod[i]='\0';
             AnalysisAssesschar(Assessmethod,2,2,sectset);
         }

       else  if(memcmp(p1,"ASSESSTRIPTIMEID=",17)==0)
        {
            p1=p1+17;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.volset.TripTime.assesstriptime.CompareID,Testname);
        }
       break;
    }

}

void Widget::ReadDirectionVariable(char *buf,struct SECTMENTSET &sectset) //?á·??òμ?±?á?
{
    char *p1=buf;
    int i=0;
    char Testname[128];
    if(memcmp(p1,"IFAULT=",7)==0)
    {
        p1=p1+7;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.dirset.TripMta.Ifault.text,Testname);
    }
    else if(memcmp(p1,"VFAULT=",7)==0)
    {
        p1=p1+7;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.dirset.TripMta.vfault.text,Testname);
    }
    else if(memcmp(p1,"MTA=",4)==0)
    {
        p1=p1+4;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.dirset.TripMta.mta.text,Testname);
    }
    else if(memcmp(p1,"FAULTTYPE=",10)==0)
    {
        p1=p1+10;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.dirset.TripMta.faulttype.text,Testname);
    }

    else if(memcmp(p1,"STEP=",5)==0)
    {
        p1=p1+5;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.dirset.TripMta.step.text,Testname);
    }

    else if(memcmp(p1,"HOLDTIME=",9)==0)
    {
        p1=p1+9;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.dirset.TripMta.steptime.text,Testname);
    }



    else if(memcmp(p1,"ASSESSMTA=",10)==0)
    {
        p1=p1+10;
        char Assessmethod[128];
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Assessmethod[i]=*p1;
            i++;
            p1++;
        }
        Assessmethod[i]='\0';
        AnalysisAssesschar(Assessmethod,3,1,sectset);
    }
    else if(memcmp(p1,"ASSESSMTAID=",12)==0)
    {
        p1=p1+12;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.dirset.TripMta.assessmta.CompareID,Testname);
    }

}

void Widget::ReadFrequencyVariable(char *buf,struct SECTMENTSET &sectset,int type) //?á?μ?êμ?±?á?
{
    char *p1=buf;
    int i=0;
    char Testname[128];
    switch(type)
    {
    case 1:

       if(memcmp(p1,"FREQENDSTART=",13)==0)
        {
            p1=p1+13;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripValue.FreEndValueStart.text,Testname);
        }
       else if(memcmp(p1,"FREQENDEND=",11)==0)
        {
            p1=p1+11;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripValue.FreEndValueEnd.text,Testname);
        }
       else if(memcmp(p1,"FREQSTEP=",9)==0)
        {
            p1=p1+9;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripValue.FreStep.text,Testname);
        }
       else if(memcmp(p1,"HOLDTIME=",9)==0)
        {
            p1=p1+9;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripValue.HoldTime.text,Testname);
        }
       else if(memcmp(p1,"ASSESSTRIPVALUE=",16)==0)
       {
           p1=p1+16;
           char Assessmethod[128];
           while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
           {
               Assessmethod[i]=*p1;
               i++;
               p1++;
           }
           Assessmethod[i]='\0';
           AnalysisAssesschar(Assessmethod,4,1,sectset);
       }
       else if(memcmp(p1,"ASSESSTRIPVALUEID=",18)==0)
       {
           p1=p1+18;
           while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
           {
               Testname[i]=*p1;
               i++;
               p1++;
           }
           Testname[i]='\0';
           strcpy(sectset.freset.TripValue.assesstripvalue.CompareID,Testname);
       }
       break;

    case 2:

         if(memcmp(p1,"FREQENDVALUE=",13)==0)
         {
             p1=p1+13;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.freset.TripTime.FreEndValue.text,Testname);
         }

         else if(memcmp(p1,"HOLDTIME=",9)==0)
         {
             p1=p1+9;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.freset.TripTime.HoldTime.text,Testname);
         }

         else  if(memcmp(p1,"ASSESSTRIPTIME=",15)==0)
          {
              p1=p1+15;
              char Assessmethod[128];
              while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
              {
                  Assessmethod[i]=*p1;
                  i++;
                  p1++;
              }
              Assessmethod[i]='\0';
              AnalysisAssesschar(Assessmethod,4,2,sectset);
          }

         else  if(memcmp(p1,"ASSESSTRIPTIMEID=",17)==0)
         {
             p1=p1+17;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.freset.TripTime.assesstriptime.CompareID,Testname);
         }
        break;

    case 4:
        if(memcmp(p1,"FREQENDVALUE=",13)==0)
        {
            p1=p1+13;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.Tripdfdt.FreEndValue.text,Testname);
        }
        else  if(memcmp(p1,"DFDTSTART=",10)==0)
        {
            p1=p1+10;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.Tripdfdt.DfdtStart.text,Testname);
        }
        else  if(memcmp(p1,"DFDTEND=",8)==0)
         {
             p1=p1+8;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.freset.Tripdfdt.DfdtEnd.text,Testname);
         }
        else  if(memcmp(p1,"DFDTSTEP=",9)==0)
         {
             p1=p1+9;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.freset.Tripdfdt.DfdtStep.text,Testname);
         }

        else  if(memcmp(p1,"ASSESSTRIPDFDT=",15)==0)
         {
             p1=p1+15;
             char Assessmethod[128];
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Assessmethod[i]=*p1;
                 i++;
                 p1++;
             }
             Assessmethod[i]='\0';
             AnalysisAssesschar(Assessmethod,4,4,sectset);
         }

        else  if(memcmp(p1,"ASSESSTRIPDFDTID=",17)==0)
         {
             p1=p1+17;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.freset.Tripdfdt.assessdfdttrip.CompareID,Testname);
         }

        break;

   case 8: //??2?±???ê±??
        if(memcmp(p1,"DFDT=",5)==0)
        {
            p1=p1+5;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.Tripdfdttime.dfdt.text,Testname);
        }

        else if(memcmp(p1,"FREQENDVALUE=",13)==0)
        {
            p1=p1+13;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.Tripdfdttime.stopfreq.text,Testname);
        }

        else if(memcmp(p1,"DURATION=",9)==0)
        {
            p1=p1+9;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.Tripdfdttime.Duration.text,Testname);
        }



        else  if(memcmp(p1,"ASSESSDFDTTRIPTIME=",19)==0)
         {
             p1=p1+19;
             char Assessmethod[128];
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Assessmethod[i]=*p1;
                 i++;
                 p1++;
             }
             Assessmethod[i]='\0';
             AnalysisAssesschar(Assessmethod,4,8,sectset);
         }

        else  if(memcmp(p1,"ASSESSDFDTTRIPTIMEID=",21)==0)
         {
             p1=p1+21;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 Testname[i]=*p1;
                 i++;
                 p1++;
             }
             Testname[i]='\0';
             strcpy(sectset.freset.Tripdfdttime.assessdfdttriptime.CompareID,Testname);
         }

        break;
    case 16:
        if(memcmp(p1,"DFDT=",5)==0)
        {
            p1=p1+5;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripVBlock.Dfdt.text,Testname);
        }
        else if(memcmp(p1,"FREQENDVALUE=",13)==0)
        {
            p1=p1+13;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripVBlock.FreEndValue.text,Testname);
        }
        else if(memcmp(p1,"VSTART=",7)==0)
        {
            p1=p1+7;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripVBlock.VStart.text,Testname);
        }
        else if(memcmp(p1,"VEND=",5)==0)
        {
            p1=p1+5;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripVBlock.VEnd.text,Testname);
        }
        else if(memcmp(p1,"VSTEP=",6)==0)
        {
            p1=p1+6;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripVBlock.VStep.text,Testname);
        }
        else if(memcmp(p1,"HOLDTIME=",9)==0)
        {
            p1=p1+9;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripVBlock.HoldTime.text,Testname);
        }
        else if(memcmp(p1,"VSELECT=",8)==0)
        {
            p1=p1+8;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            sectset.freset.TripVBlock.VSelect=atoi(Testname);

        }

        else if(memcmp(p1,"ASSESSVBLOCK=",13)==0)
        {
            p1=p1+13;
            char Assessmethod[50];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Assessmethod[i]=*p1;
                i++;
                p1++;
            }
            Assessmethod[i]='\0';
            AnalysisAssesschar(Assessmethod,4,16,sectset);
        }

        else if(memcmp(p1,"ASSESSVBLOCKID=",15)==0)
        {
            p1=p1+15;
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                Testname[i]=*p1;
                i++;
                p1++;
            }
            Testname[i]='\0';
            strcpy(sectset.freset.TripVBlock.assessvblock.CompareID,Testname);
        }



        break;
    }



}

void Widget::ReadAnytestVariable(char *buf,struct SECTMENTSET &sectset) //?áè?òa2aê?μ?±?á?
{
    char *p2=buf;
    char idname[128];
    if(memcmp(buf,"VAValue=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
       strcpy(sectset.anytestset.VAAP.text,idname);

    }
    else if(memcmp(buf,"VAPhase=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.VAPH);
       strcpy(sectset.anytestset.VAPH.text,idname);
    }
    else if(memcmp(buf,"VAFrec=",7)==0)
    {
       p2=p2+7;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.VAFR);
        strcpy(sectset.anytestset.VAFR.text,idname);
    }
    //VB
    else if(memcmp(buf,"VBValue=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.VBAP);
        strcpy(sectset.anytestset.VBAP.text,idname);
    }
    else if(memcmp(buf,"VBPhase=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.VBPH);
       strcpy(sectset.anytestset.VBPH.text,idname);
    }
    else if(memcmp(buf,"VBFrec=",7)==0)
    {
       p2=p2+7;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.VBFR);
         strcpy(sectset.anytestset.VBFR.text,idname);
    }
    //VC
    else if(memcmp(buf,"VCValue=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.VCAP);
       strcpy(sectset.anytestset.VCAP.text,idname);
    }
    else if(memcmp(buf,"VCPhase=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.VCPH);
         strcpy(sectset.anytestset.VCPH.text,idname);
    }
    else if(memcmp(buf,"VCFrec=",7)==0)
    {
       p2=p2+7;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.VCFR);
        strcpy(sectset.anytestset.VCFR.text,idname);
    }
    //VZ
    else if(memcmp(buf,"VZValue=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
       //getsettingfromid(settinglink,idname,sectset.anytestset.VZAP);
        strcpy(sectset.anytestset.VZAP.text,idname);
    }
    else if(memcmp(buf,"VZPhase=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.VZPH);
        strcpy(sectset.anytestset.VZPH.text,idname);
    }
    else if(memcmp(buf,"VZFrec=",7)==0)
    {
       p2=p2+7;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.VZFR);
         strcpy(sectset.anytestset.VZFR.text,idname);
    }
    //IA
    else if(memcmp(buf,"IAValue=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.IAAP);
      strcpy(sectset.anytestset.IAAP.text,idname);
    }
    else if(memcmp(buf,"IAPhase=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     // getsettingfromid(settinglink,idname,sectset.anytestset.IAPH);
        strcpy(sectset.anytestset.IAPH.text,idname);
    }
    else if(memcmp(buf,"IAFrec=",7)==0)
    {
       p2=p2+7;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.IAFR);
          strcpy(sectset.anytestset.IAFR.text,idname);
    }
    //IB
    else if(memcmp(buf,"IBValue=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.IBAP);
         strcpy(sectset.anytestset.IBAP.text,idname);
    }
    else if(memcmp(buf,"IBPhase=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.IBPH);
       strcpy(sectset.anytestset.IBPH.text,idname);
    }
    else if(memcmp(buf,"IBFrec=",7)==0)
    {
       p2=p2+7;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.IBFR);
        strcpy(sectset.anytestset.IBFR.text,idname);
    }
    //IC
    else if(memcmp(buf,"ICValue=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.ICAP);
       strcpy(sectset.anytestset.ICAP.text,idname);
    }
    else if(memcmp(buf,"ICPhase=",8)==0)
    {

       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.ICPH);
        strcpy(sectset.anytestset.ICPH.text,idname);
    }
    else if(memcmp(buf,"ICFrec=",7)==0)
    {
       p2=p2+7;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';

     //  getsettingfromid(settinglink,idname,sectset.anytestset.ICFR);
        strcpy(sectset.anytestset.ICFR.text,idname);
    }

    //μ?á÷2?3¤
    else if(memcmp(buf,"CurStep=",8)==0)
    {
       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.Istep);
        strcpy(sectset.anytestset.Istep.text,idname);
    }
    //μ??12?3¤
    else if(memcmp(buf,"VolStep=",8)==0)
    {
       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.Vstep);
         strcpy(sectset.anytestset.Vstep.text,idname);
    }
    //?à??2?3¤
    else if(memcmp(buf,"PhaStep=",8)==0)
    {
       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.Pstep);
         strcpy(sectset.anytestset.Pstep.text,idname);
    }
    //?μ?ê2?3¤
    else if(memcmp(buf,"FreStep=",8)==0)
    {
       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.Fstep);
        strcpy(sectset.anytestset.Fstep.text,idname);
    }
    //2?3¤ê±??
    else if(memcmp(buf,"StepTime=",9)==0)
    {
       p2=p2+9;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.Steptime);
           strcpy(sectset.anytestset.Steptime.text,idname);
    }
    //2?êy
    else if(memcmp(buf,"StepNum=",8)==0)
    {
       p2=p2+8;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.Stepnum);
         strcpy(sectset.anytestset.Stepnum.text,idname);
    }
    //1ê???°ê±??
    else if(memcmp(buf,"PreFaultTime=",13)==0)
    {
       p2=p2+13;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.Prefault);
         strcpy(sectset.anytestset.Prefault.text,idname);
    }
    //1ê??ê±??
    else if(memcmp(buf,"FaultTime=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.Infault);
        strcpy(sectset.anytestset.Infault.text,idname);
    }
    //1ê??oóê±??
    else if(memcmp(buf,"PostFaultTime=",14)==0)
    {
       p2=p2+14;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.Postfault);
        strcpy(sectset.anytestset.Postfault.text,idname);
    }
    //±?á?VA

    else if(memcmp(buf,"Faultduration=",14)==0)
    {
        p2=p2+14;
        int i=0;
         while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
        {
            idname[i++]=*p2;
            p2++;
        }
        idname[i]='\0';
         strcpy(sectset.anytestset.Faultduraion.text,idname);
    }






    else if(memcmp(buf,"VariatyVA=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.VariatyVA);
          strcpy(sectset.anytestset.VariatyVA.text,idname);
    }
    //±?á?VB
    else if(memcmp(buf,"VariatyVB=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.VariatyVB);
          strcpy(sectset.anytestset.VariatyVB.text,idname);
    }
    //±?á?VC
    else if(memcmp(buf,"VariatyVC=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.VariatyVC);
           strcpy(sectset.anytestset.VariatyVC.text,idname);
    }
    //±?á?VZ
    else if(memcmp(buf,"VariatyVZ=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
      // getsettingfromid(settinglink,idname,sectset.anytestset.VariatyVZ);
            strcpy(sectset.anytestset.VariatyVZ.text,idname);
    }
    //±?á?3U0
    else if(memcmp(buf,"VariatyVz3U0=",13)==0)
    {
       p2=p2+13;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.VZ3U0);
         strcpy(sectset.anytestset.VZ3U0.text,idname);
    }
    //±?á?IA
    else if(memcmp(buf,"VariatyIA=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
     //  getsettingfromid(settinglink,idname,sectset.anytestset.VariatyIA);
            strcpy(sectset.anytestset.VariatyIA.text,idname);
    }
    //±?á?IB
    else if(memcmp(buf,"VariatyIB=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
    //   getsettingfromid(settinglink,idname,sectset.anytestset.VariatyIB);
              strcpy(sectset.anytestset.VariatyIB.text,idname);
    }
    //±?á?IC
    else if(memcmp(buf,"VariatyIC=",10)==0)
    {
       p2=p2+10;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';

        strcpy(sectset.anytestset.VariatyIC.text,idname);
    }
    //2aê??￡ê?
    else if(memcmp(buf,"TestMode=",9)==0)
    {
       p2=p2+9;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
       strcpy(sectset.anytestset.Testmode.text,idname);
    }

    //±??ˉ????
    else if(memcmp(buf,"Variableitem=",13)==0)
    {
       p2=p2+13;
       int i=0;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           idname[i++]=*p2;
           p2++;
       }
       idname[i]='\0';
       strcpy(sectset.anytestset.ChangeItem.text,idname);
   }


}


void Widget::ReadPhraseVariable(char *buf,struct SECTMENTSET &sectset,int type) //?á?à??2aê?μ?±?á?
{
       char *p1=buf;
       int i=0;
       char Testname[128];
       switch(type)
       {
       case 1: //′￥·￠?μ

            if(memcmp(p1,"PENDSTART=",10)==0)
           {
               p1=p1+10;
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Testname[i]=*p1;
                   i++;
                   p1++;
               }
               Testname[i]='\0';
               strcpy(sectset.phraseset.TripValue.StartValue.text,Testname);
           }
           else if(memcmp(p1,"PENDEND=",8)==0)
           {
               p1=p1+8;
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Testname[i]=*p1;
                   i++;
                   p1++;
               }
               Testname[i]='\0';
               strcpy(sectset.phraseset.TripValue.EndValue.text,Testname);
           }
           else if(memcmp(p1,"PSTEP=",6)==0)
           {
               p1=p1+6;
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Testname[i]=*p1;
                   i++;
                   p1++;
               }
               Testname[i]='\0';
               strcpy(sectset.phraseset.TripValue.StepValue.text,Testname);
           }
           else if(memcmp(p1,"HOLDTIME=",9)==0)
           {
               p1=p1+9;
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Testname[i]=*p1;
                   i++;
                   p1++;
               }
               Testname[i]='\0';
               strcpy(sectset.phraseset.TripValue.HoldTime.text,Testname);
           }
           else if(memcmp(p1,"ASSESSTRIPVALUE=",16)==0)
           {

               p1=p1+16;

               char Assessmethod[128];
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Assessmethod[i]=*p1;
                   i++;
                   p1++;
               }
               Assessmethod[i]='\0';
               AnalysisAssesschar(Assessmethod,6,1,sectset);
           }
           else if(memcmp(p1,"ASSESSTRIPVALUEID=",18)==0)
           {
               p1=p1+18;
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Testname[i]=*p1;
                   i++;
                   p1++;
               }
               Testname[i]='\0';
               strcpy(sectset.phraseset.TripValue.assesstripvalue.CompareID,Testname);
           }

           break;
       case 2: //′￥·￠ê±??

           if(memcmp(p1,"PEND=",5)==0)
           {
               p1=p1+5;
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Testname[i]=*p1;
                   i++;
                   p1++;
               }
               Testname[i]='\0';
               strcpy(sectset.phraseset.TripTime.EndVaule.text,Testname);
           }
          else  if(memcmp(p1,"HOLDTIME=",9)==0)
            {
                p1=p1+9;
                while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                {
                    Testname[i]=*p1;
                    i++;
                    p1++;
                }
                Testname[i]='\0';
                strcpy(sectset.phraseset.TripTime.HoldTime.text,Testname);
            }
          else  if(memcmp(p1,"ASSESSTRIPTIME=",15)==0)
            {
                p1=p1+15;
                char Assessmethod[128];
                while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                {
                    Assessmethod[i]=*p1;
                    i++;
                    p1++;
                }
                Assessmethod[i]='\0';
                AnalysisAssesschar(Assessmethod,6,2,sectset);
            }

          else  if(memcmp(p1,"ASSESSTRIPTIMEID=",17)==0)
           {
               p1=p1+17;
               while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
               {
                   Testname[i]=*p1;
                   i++;
                   p1++;
               }
               Testname[i]='\0';
               strcpy(sectset.phraseset.TripTime.assesstriptime.CompareID,Testname);
           }
          break;
       }


}

void Widget::ReadHarmonicVariable(char *buf,struct SECTMENTSET &sectset) //读谐波的变量
{
    char *p1=buf;
    int i=0;
    char Testname[128];
    if(memcmp(p1,"HARMONICSTART=",14)==0)
    {
        p1=p1+14;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.harmonicset.TripValue.StartValue.text,Testname);
    }
    else if(memcmp(p1,"HARMONICEND=",12)==0)
    {
        p1=p1+12;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.harmonicset.TripValue.EndValue.text,Testname);
    }
    else if(memcmp(p1,"STEP=",5)==0)
    {
        p1=p1+5;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.harmonicset.TripValue.StepValue.text,Testname);
    }
    else if(memcmp(p1,"HOLDTIME=",9)==0)
    {
        p1=p1+9;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.harmonicset.TripValue.HoldTime.text,Testname);
    }



    else if(memcmp(p1,"ASSESSTRIPVALUE=",16)==0)
    {
        p1=p1+16;
        char Assessmethod[128];
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Assessmethod[i]=*p1;
            i++;
            p1++;
        }
        Assessmethod[i]='\0';
        AnalysisAssesschar(Assessmethod,7,1,sectset);//2017-1-6教训呀，相对绝对误差没写入的问题

    }
    else if(memcmp(p1,"ASSESSTRIPVALUEID=",18)==0)
    {
        p1=p1+18;
        while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
        {
            Testname[i]=*p1;
            i++;
            p1++;
        }
        Testname[i]='\0';
        strcpy(sectset.harmonicset.TripValue.assesstripvalue.CompareID,Testname);
    }
    sectset.dirset.TripMta.phrasemta=-2;//-1000?
}




void Widget::Processsection(char *filename)
{

    FILE *file= NULL;
    int sequnence=0;
    struct SECTMENTSET sectset;
    int currentfunctype=0; //正在读测试功能定值
    int currestsubtestitem=0; //当前测试项目
    char buf[BUFFSIZE];
    if((file=fopen(filename,"r")) == NULL)
      {
          printf("fopen error\n");
          return;
      }
    memset(&sectset,0,sizeof(struct SECTMENTSET));

    while(fgets(buf,BUFFSIZE,file)!=NULL)
    {

        if(memcmp(buf,"<SectionsettingBegin>",strlen("<SectionsettingBegin>"))==0)
        {
           sequnence=1;
        }
        else if(memcmp(buf,"sectionname:",strlen("sectionname:"))==0&&sequnence==1)
        {
         char *p2=buf;
         char secttionname[120];
         p2=p2+12;
         int i=0;
         while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
         {
             secttionname[i]=*p2;
             i++;
             p2++;
         }
         secttionname[i]='\0';
         strcpy(sectset.name,secttionname);

        }
        else if(memcmp(buf,"<TestfunctionBegin>",strlen("<TestfunctionBegin>"))==0&&sequnence==1)
        {

           sequnence=2;
        }
        else if(sequnence==2&&memcmp(buf,"<TESTITEMBEGIN>",strlen("<TESTITEMBEGIN>"))!=0)
          {

            char *p1;
            p1=buf;
            if(memcmp(p1,"TESTTYPE=",9)==0)
           {
                p1=p1+9;
                if(memcmp(p1,"OVERCURRENT.COM",strlen("OVERCURRENT.COM"))==0) //表示测试的是电流
               {  sectset.curset.valid=true;
                  sectset.curset.done=true;
                  currentfunctype=1;
                }
                else if(memcmp(p1,"UNDERVOLTAGE.COM",strlen("UNDERVOLTAGE.COM"))==0) //表示测试的是电压
               { sectset.volset.valid=true;
                 sectset.volset.done=true;

                 currentfunctype=2;
                }
                else if(memcmp(p1,"DIRECTION.COM",strlen("DIRECTION.COM"))==0)
               { sectset.dirset.valid=true;
                 sectset.dirset.done=true;

                 currentfunctype=3;
                }
                else if(memcmp(p1,"UNDERFREQUENCY.COM",strlen("UNDERFREQUENCY.COM"))==0)
               { sectset.freset.valid=true;
                 sectset.freset.done=true;

                 currentfunctype=4;
               }
                else if(memcmp(p1,"ANYTEST.COM",strlen("ANYTEST.COM"))==0)
               { sectset.anytestset.valid=true;
                 sectset.anytestset.done=true;
                 currentfunctype=5;
               }

                else if(memcmp(p1,"VECTORSHIFT.COM",strlen("VECTORSHIFT.COM"))==0)
                {
                    sectset.phraseset.valid=true;
                    sectset.phraseset.done=true;
                    currentfunctype=6;

                }
                //2016-12-13 新增加harmonic
                else if(memcmp(p1,"HARMONIC.COM",strlen("HARMONIC.COM"))==0)
               { sectset.harmonicset.valid=true;
                 sectset.harmonicset.done=true;
                 currentfunctype=7;
               }
           }

            switch(currentfunctype)
            {
            case 1:
                if(memcmp(p1,"TESTNAME=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.curset.name,Testname);
                }
                else if(memcmp(p1,"TESTITEM=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.curset.Testitem.text,Testname);
                }

                else if(memcmp(p1,"OUTPUTSEL=",10)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+10;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.curset.output.text,Testname);
                }
                else if(memcmp(p1,"FAULTDURATION=",14)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+14;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.curset.Faultduraion.text,Testname);
                }
                else if(memcmp(p1,"VFAULT=",7)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+7;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.curset.Vfault.text,Testname);
                }

                else if(memcmp(p1,"ANGLE=",6)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+6;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.curset.Angle.text,Testname);
                }


                break;
            case 2:
                if(memcmp(p1,"TESTNAME=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.volset.name,Testname);
                }

                else if(memcmp(p1,"TESTITEM=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.volset.Testitem.text,Testname);
                }

                else if(memcmp(p1,"FAULTTYPE=",10)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+10;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.volset.FaultType.text,Testname);
                    sectset.volset.FaultType.value=atof(Testname);
                }
                else if(memcmp(p1,"FAULTDURATION=",14)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+14;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.volset.Faultduraion.text,Testname);
                }
                else if(memcmp(p1,"VPNVPP=",7)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+7;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.volset.Vpn_Vpp=atoi(Testname);
                }
                break;
            case 3:
                if(memcmp(p1,"TESTNAME=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.dirset.name,Testname);
                }
                break;

            case 4:
                if(memcmp(p1,"TESTNAME=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.freset.name,Testname);
                }
                else if(memcmp(p1,"TESTITEM=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.freset.TestItem.text,Testname);
                }
                else if(memcmp(p1,"FAULTDURATION=",14)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+14;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.freset.Faultduraion.text,Testname);
                }
                break;
            case 5:
                if(memcmp(p1,"TESTNAME=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.anytestset.name,Testname);
                }
                break;
            case 6:

                if(memcmp(p1,"TESTNAME=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.phraseset.name,Testname);
                }
                else if(memcmp(p1,"TESTITEM=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.phraseset.Testitem.text,Testname);
                }

                else if(memcmp(p1,"FAULTDURATION=",14)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+14;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.phraseset.Faultduraion.text,Testname);
                }

                else if(memcmp(p1,"FIXVOLTAGE=",11)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+11;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.phraseset.FixVoltage.text,Testname);
                }

                else if(memcmp(p1,"FIXCURRENT=",11)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+11;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.phraseset.FixCurrent.text,Testname);
                }

                else if(memcmp(p1,"IACH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.phraseset.IAch=atoi(Testname);
                }
                else if(memcmp(p1,"IBCH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.phraseset.IBch=atoi(Testname);
                }
                else if(memcmp(p1,"ICCH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.phraseset.ICch=atoi(Testname);
                }
                else if(memcmp(p1,"VACH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.phraseset.ICch=atoi(Testname);
                }
                else if(memcmp(p1,"VBCH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.phraseset.VBch=atoi(Testname);
                }
                else if(memcmp(p1,"VCCH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.phraseset.VCch=atoi(Testname);
                }
                break;
            case 7://2016-12-13 新增加harmonic
                if(memcmp(p1,"TESTNAME=",9)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+9;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.harmonicset.name,Testname);
                }
                else if(memcmp(p1,"COUNT=",6)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+6;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.harmonicset.HarmonicCount.text,Testname);
                }
                else if(memcmp(p1,"CURRENT=",8)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+8;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.harmonicset.Current.text,Testname);
                }

                else if(memcmp(p1,"PHASE=",6)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+6;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    strcpy(sectset.harmonicset.Phase.text,Testname);
                }

                else if(memcmp(p1,"IACH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.harmonicset.IAch=atoi(Testname);
                }
                else if(memcmp(p1,"IBCH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.harmonicset.IBch=atoi(Testname);
                }
                else if(memcmp(p1,"ICCH=",5)==0)
                {
                    int i=0;
                    char Testname[128];
                    p1=p1+5;
                    while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                        Testname[i]=*p1;
                        i++;
                        p1++;
                    }
                    Testname[i]='\0';
                    sectset.harmonicset.ICch=atoi(Testname);
                }

                break;


            }

        }

        else if(memcmp(buf,"<TESTITEMBEGIN>",strlen("<TESTITEMBEGIN>"))==0)
        {

          sequnence=3;

        }

        else if(sequnence==3&&memcmp(buf,"<TESTITEMEND>",strlen("<TESTITEMEND>"))!=0)
        {

           char *p1=buf;
           if(memcmp(p1,"TESTITEMNAME=",13)==0)
           {
            p1=p1+13;

            switch(currentfunctype)
            {
            case 1:
                if(memcmp(p1,"OVERCURRENT.TRIPVALUE",strlen("OVERCURRENT.TRIPVALUE"))==0)
                {
                 sectset.curset.TripValue.valid=true;
                 currestsubtestitem=1;
                }
                else if(memcmp(p1,"OVERCURRENT.TRIPTIME",strlen("OVERCURRENT.TRIPTIME"))==0)
                {
                sectset.curset.TripTime.valid=true;
                currestsubtestitem=2;
                }
                break;
            case 2:
                if(memcmp(p1,"UNDERVOLTAGE.TRIPVALUE",strlen("UNDERVOLTAGE.TRIPVALUE"))==0)
                {
                 sectset.volset.TripValue.valid=true;
                 currestsubtestitem=1;
                }
                else if(memcmp(p1,"UNDERVOLTAGE.TRIPTIME",strlen("UNDERVOLTAGE.TRIPTIME"))==0)
                {
                sectset.volset.TripTime.valid=true;
                currestsubtestitem=2;
                }
                break;
            case 3:
                if(memcmp(p1,"DIRECTION.MTA",strlen("DIRECTION.MTA"))==0)
                {
                 sectset.dirset.TripMta.valid=true;
                 currestsubtestitem=1;
                }
                break;

            case 4:
                if(memcmp(p1,"UNDERFREQUENCY.TRIPVALUE",strlen("UNDERFREQUENCY.TRIPVALUE"))==0)
                {
                 sectset.freset.TripValue.valid=true;
                 currestsubtestitem=1;
                }

                else if(memcmp(p1,"UNDERFREQUENCY.TRIPTIME",strlen("UNDERFREQUENCY.TRIPTIME"))==0)
                 {
                  sectset.freset.TripTime.valid=true;
                  currestsubtestitem=2;
                 }
                else if(memcmp(p1,"UNDERFREQUENCY.TRIPDFDT",strlen("UNDERFREQUENCY.TRIPDFDT"))==0)
              {
                  sectset.freset.Tripdfdt.valid=true;
                  currestsubtestitem=4;
              }
                else if(memcmp(p1,"UNDERFREQUENCY.DFDTTRIPTIME",strlen("UNDERFREQUENCY.DFDTTRIPTIME"))==0)
             {
                    sectset.freset.Tripdfdttime.valid=true;
                    currestsubtestitem=8;
             }

                else if(memcmp(p1,"UNDERFREQUENCY.VOLTAGEBLOCKING",strlen("UNDERFREQUENCY.VOLTAGEBLOCKING"))==0)
                 {
                  sectset.freset.TripVBlock.valid=true;

                  currestsubtestitem=16;
                 }
                break;
            case 5:

                break;
            case 6:

                if(memcmp(p1,"VECTORSHIFT.TRIPVALUE",strlen("VECTORSHIFT.TRIPVALUE"))==0)
                {
                 sectset.phraseset.TripValue.valid=true;
                 currestsubtestitem=1;
                }
                else if(memcmp(p1,"VECTORSHIFT.TRIPTIME",strlen("VECTORSHIFT.TRIPTIME"))==0)
                {
                sectset.phraseset.TripTime.valid=true;
                currestsubtestitem=2;
                }
                break;
            case 7://2016-12-13
                if(memcmp(p1,"HARMONIC.TRIPVALUE",strlen("HARMONIC.TRIPVALUE"))==0)
                {
                 sectset.harmonicset.TripValue.valid=true;
                 currestsubtestitem=1;
                }
                break;
            }
           }
           else
           {
               switch(currentfunctype)
              {
                case 1:
                   ReadCurrentVariable(p1,sectset,currestsubtestitem);
                   break;
                case 2:
                   ReadVoltageVariable(p1,sectset,currestsubtestitem);
                   break;
                case 3:
                   ReadDirectionVariable(p1,sectset);
                   break;
                case 4:
                   ReadFrequencyVariable(p1,sectset,currestsubtestitem);
                   break;
                case 5:
                   ReadAnytestVariable(p1,sectset);
                   break;
                case 6:

                  ReadPhraseVariable(p1,sectset,currestsubtestitem);
                  break;
               case 7://2016-12-13
                  ReadHarmonicVariable(p1,sectset);
                  break;
              }
           }
        }
        else if(memcmp(buf,"<TESTITEMEND>",strlen("<TESTITEMEND>"))==0)
        {
        sequnence=2;

        }

        else if(memcmp(buf,"<TestfunctionEnd>",strlen("<TestfunctionEnd>"))==0)
         {
         sequnence=1;
         currentfunctype=0;
         currestsubtestitem=0;

         }

    }

    fclose(file);
    if(sectset.curset.valid==true\
       ||sectset.volset.valid==true\
       ||sectset.dirset.valid==true\
       ||sectset.freset.valid==true\
       ||sectset.anytestset.valid==true\
       ||sectset.phraseset.valid==true\
            ||sectset.harmonicset.valid==true)//2016-12-13
       {
       //段有效插入节点中
        insertnode(nodelink,sectset,sumsectnum);
        memset(&sectset,0,sizeof(struct SECTMENTSET));
        sumsectnum++;
       }

}


void Widget::strrpl(char *pSrcIn,char *pDstOut,const char* pSrcRpl, const char* pDstRpl)
{
char* pi = pSrcIn;
char* po = pDstOut;

int nSrcRplLen = strlen(pSrcRpl);
int nDstRplLen = strlen(pDstRpl);

char *p = NULL;
int nLen = 0;

do
{
// ?òμ???ò???ì???μ?
p = strstr(pi, pSrcRpl);

if(p != NULL)
{
// ??±′é?ò???ì???μ?oí??ò???ì???μ??D??μ?×?·?′?
nLen = p - pi;
memcpy(po, pi, nLen);

// ??±′Dèòaì???μ?×?·?′?
memcpy(po + nLen, pDstRpl, nDstRplLen);
}
else
{
strcpy(po, pi);

// è?1???óDDèòa??±′μ?×?·?′?,?μ?÷?-?·ó|???áê?
break;
}

pi = p + nSrcRplLen;
po = po + nLen + nDstRplLen;

} while (p != NULL);
}

float Widget::RelaceExp(char *exp) //±í′?ê??DID3???μ?′?êy
{
    if(settinglink==NULL||settinglink->next==NULL)
    {
       return -9999;
    }
 struct SETTINGNODELIST *nextsetting=settinglink->next;
 char Expin[1024];
 char Expout[1024];
 SETTING setting;
 strcpy(Expin,exp);

 while(nextsetting!=NULL)
 {

     if(strstr(Expin,nextsetting->set.ID)!=NULL)
     {

         getsettingfromid(settinglink,nextsetting->set.ID,setting);
         strrpl(Expin,Expout,nextsetting->set.ID,setting.value);
         memset(Expin,'\0',1024);
         strcpy(Expin,Expout);

     }
     nextsetting=nextsetting->next;
 }
 return CaculatefromExp(QString(Expin));
}

void Widget::CaculateVariable(VARIABLE &exp)
{
    bool ok;
    QString str=QString(exp.text);
    str.toFloat(&ok);
    if(ok)
    {
     exp.value=str.toFloat(&ok);
    }
    else
    {
        char buffer[1024]={'\0'};
        strcpy(buffer,exp.text);
        exp.value=RelaceExp(buffer);
    }
}




void  Widget::Analyzetemplatefile(char *filename)
 {
     FILE *file= NULL;
     char buf[BUFFSIZE];
     if((file=fopen(filename,"r")) == NULL)
       {
           printf("fopen error\n");
           return ;
       }

     memset(buf,'\0',BUFFSIZE);
     nodelink->next=NULL;
     settinglink->next=NULL;

     updatesetting(filename);
     char tmpfilename[255];
     int num=1;
     while(fgets(buf,BUFFSIZE,file)!=NULL)
     {

         if(memcmp(buf,"<SectionsettingBegin>",strlen("<SectionsettingBegin>"))==0)
         {
             FILE *fp= NULL;
             sprintf(tmpfilename,"setting%d.mtp.tmp",num++);
             if((fp=fopen(tmpfilename, "w+")) == NULL)
               {
                   printf("fopen error\n");
                   return ;
               }
             fwrite(buf,strlen(buf),1,fp);
             while(fgets(buf,BUFFSIZE,file)!=NULL)
             {
             fwrite(buf,strlen(buf),1,fp);
             if(memcmp(buf,"<SectionsettingEnd>",strlen("<SectionsettingEnd>"))==0)
             {
                 fclose(fp);
                 break;
             }
           }
            if(memcmp(buf,"<SectionsettingEnd>",strlen("<SectionsettingEnd>"))==0) //è?1?×?oóò?DD?aTestfunctionEnd è?oó′|àísetting.tmp ???t
             Processsection(tmpfilename);
             QFile fl;
             fl.setFileName(tmpfilename);
             fl.remove();
         }
     }
     fclose(file);

 }




int Widget::CheckAuthrizeFile(char *templatefile)
{

    unsigned long Checksum=0;
    FILE *fp=NULL;
    fp=fopen(templatefile,"r");
    if(fp==NULL)
    {  printf("fopen error\n");
       return -1;
    }
    char key=0x13;
    char miwen[1024];
    char buf[1024];
    int sequnence=0;
    char *p2=NULL;
    char miwen_2[1024];
    char length=0;

    while(fgets(miwen,1024,fp)!=NULL)
    {
        length=0;
        memset(buf,0,1024);
        p2=miwen;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
        {
            miwen_2[length]=*p2;
            p2++;
            length++;
        }
        miwen_2[length]='\0';
        jiemiXor(miwen_2,key,buf);


        if(memcmp(buf,"SERIALNUM=",strlen("SERIALNUM="))==0&&sequnence==0)
        {
            Checksum+=Check(buf);

            char *p1=buf+strlen("SERIALNUM=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
            temp[len]='\0';
            authorizedfileinfo.Serailnum=QString(temp);
            sequnence=1;
        }
        else if(sequnence==1&&memcmp(buf,"FILENAME=",strlen("FILENAME="))==0)
        {

            Checksum+=Check(buf);

            char *p1=buf+strlen("FILENAME=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
                len++;
            }
           temp[len]='\0';
           authorizedfileinfo.filename=QString(temp);
           sequnence=2;
        }
        else if(sequnence==2&&memcmp(buf,"AUTHORIZETIME=",strlen("AUTHORIZETIME="))==0)
        {
            Checksum+=Check(buf);
            int len=0;
            char temp[512];
            char *p1=buf+strlen("AUTHORIZETIME=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';

          authorizedfileinfo.Authorizetime=QString(temp);

          sequnence=3;
        }

        else if(sequnence==3&&memcmp(buf,"EXPIRETIME=",strlen("EXPIRETIME="))==0)
        {
            Checksum+=Check(buf);
            int len=0;
            char temp[512];
            char *p1=buf+strlen("EXPIRETIME=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';
          authorizedfileinfo.Expiretime=QString(temp);
          sequnence=4;


        }

         else if(sequnence==4&&memcmp(buf,"MODIFIEDTIME=",strlen("MODIFIEDTIME="))==0)
        {
            Checksum+=Check(buf);
            int len=0;
            char temp[512];
            char *p1=buf+strlen("MODIFIEDTIME=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';
          authorizedfileinfo.Modifiedtime=QString(temp);
          sequnence=5;

        }

        else if(sequnence==5&&memcmp(buf,"FILESIZE=",strlen("FILESIZE="))==0)
       {
           Checksum+=Check(buf);
           int len=0;
           char temp[512];
           char *p1=buf+strlen("FILESIZE=");
           while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
           {
               temp[len]=*p1;
               p1++;
              len++;
           }
         temp[len]='\0';
         authorizedfileinfo.filesize=QString(temp);
         sequnence=6;

       }



        else if(sequnence==6&&memcmp(buf,"CHECKCODE=",strlen("CHECKCODE="))==0)
        {
         char *p1=buf+strlen("CHECKCODE=");

         unsigned long CRC=atol(p1);
         if(CRC!=Checksum)
         {
             fclose(fp);
             return -1;
         }
         }
    }
    fclose(fp);
    return 1;
}





void Widget::on_pushButton_9_clicked()//打开模板文件
{

    QString filename=QFileDialog::getOpenFileName(this,tr("Open template file"),"./",tr("*.mtp"));
    if(filename.isEmpty())
    {
          QMessageBox::warning(this, "warning", "please select a template file", QMessageBox::Ok);
          return;
    }
    QByteArray ba=filename.toLatin1();
    char *mm = ba.data();


    char AuthorizedFilename[512];
    char AuthorizedFilenametmp[512];// 授权文件的名字


    //若授权文件不匹配，则弹出对话框提示不能编辑
    int Templatefilelen=0;
    Templatefilelen=strlen(mm)-strlen(".mtp");
    for(int i=0;i<Templatefilelen;i++)
        AuthorizedFilename[i]=mm[i];

    AuthorizedFilename[Templatefilelen]='\0';
    sprintf(AuthorizedFilenametmp,"%s.dat",AuthorizedFilename);

    if(QFile::exists(AuthorizedFilenametmp)==true)
    {
     QMessageBox::information(this,"Tip","You are editing  a authorized template file!",QMessageBox::Ok);
     if(CheckAuthrizeFile(AuthorizedFilenametmp)==-1)
     {
         AuthorizedFileValid=false;
         QMessageBox::warning(this,"Warning","The authorized file is damaged!",QMessageBox::Ok);
     }
     else //授权文件有效
     {

         QFileInfo templatefileinfo(mm);
         unsigned long filesize=templatefileinfo.size();
         // 模板文件更改日期(GMT)
         QDateTime modifiedtime=templatefileinfo.lastModified().toUTC();

         int year,month,day,hour,minute,second;
         QByteArray ba2=authorizedfileinfo.Modifiedtime.toLatin1();
         char *modifiedtime_str=ba2.data();

         sscanf(modifiedtime_str,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&minute,&second);

         bool ok;
         bool timevalid=modifiedtime.date().year()==year&&modifiedtime.date().month()==month&&modifiedtime.date().day()==day&&\
            modifiedtime.time().hour()==hour&&modifiedtime.time().minute()==minute&&modifiedtime.time().second()==second;

         unsigned long fileinfo_filesize=authorizedfileinfo.filesize.toInt(&ok);

         if(ok==false||fileinfo_filesize!=filesize||timevalid==false)
         {
             AuthorizedFileValid=false;
             QMessageBox::warning(this,"Warning","The authorized file is invalid!",QMessageBox::Ok);

         }
         else //否则授权文件有效
         {
            AuthorizedFileValid=true;
         }
     }

    }

    else
        AuthorizedFileValid=false;


    Analyzetemplatefile(mm);
    updatesectionlist();
    memset(&sectmentset,0,sizeof(struct SECTMENTSET));
    updatefunclist();

}



void Widget::on_pushButton_11_clicked()//Edit section
{

    QListWidgetItem *itemtmp=ui->listWidget_2->currentItem();
    if(itemtmp==NULL)
    return;

    QString str=itemtmp->text();
    QByteArray ba=str.toLatin1();
    char *strchar=ba.data();
    if(getnodevaluefromname(nodelink,strchar,sectmentset))
    {
        updatefunclist();
    }

}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_listWidget_2_itemDoubleClicked(QListWidgetItem* item)
{

    QListWidgetItem *itemtmp=item;
    if(itemtmp==NULL)
    return;

    QString str=itemtmp->text();
    QByteArray ba=str.toLatin1();
    char *strchar=ba.data();
    if(getnodevaluefromname(nodelink,strchar,sectmentset))
    {
        updatefunclist();
    }
}

void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{

   // QListWidgetItem *item=ui->listWidget->currentItem();
    if(item==NULL)
    return;

  QString str=item->text();

  if(str==QString(QLatin1String(sectmentset.curset.name)))
  {

      iseditting=true;
      curset=new Currentset;
      connect(curset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(curset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      curset->show();
      curset->settinglink=settinglink;
      curset->setsectmentset(sectmentset);
      curset->Setcurrentset(sectmentset.curset);

  }
  else if(str==QString(QLatin1String(sectmentset.volset.name)))
  {
      iseditting=true;
      volset=new Voltageset;
      connect(volset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(volset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));

      volset->settinglink=settinglink;
      volset->setsectmentset(sectmentset);
      volset->Setvoltageset(sectmentset.volset);
      volset->show();

  }
   else if(str==QString(QLatin1String(sectmentset.dirset.name)))
  {
      iseditting=true;
      dirset=new Direction;
      connect(dirset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(dirset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      dirset->setsectmentset(sectmentset);
      dirset->SetDirectionset(sectmentset.dirset);
      dirset->settinglink=settinglink;
      dirset->show();

  }

 else if(str==QString(QLatin1String(sectmentset.freset.name)))
  {   //??ê?μ??1????
      iseditting=true;
      freset=new Frequency;
      connect(freset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(freset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      freset->show();
      freset->settinglink=settinglink;
      freset->setsectmentset(sectmentset);
      freset->Setfrequencyset(sectmentset.freset);

  }

 /*
 else if(str==QString(QLatin1String(sectmentset.anytestset.name)))
  {   //??ê?μ??1????
      iseditting=true;
      anytestset=new Anytest;
      connect(anytestset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(anytestset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      anytestset->show();
      anytestset->setsectmentset(sectmentset);
      anytestset->Setanytestset(sectmentset.anytestset);
  }


 */

 else if(str==QString(QLatin1String(sectmentset.phraseset.name)))
  {
      iseditting=true;
      phraseset=new Phrase;
      connect(phraseset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
      connect(phraseset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
      phraseset->show();
      phraseset->settinglink=settinglink;
      phraseset->setsectmentset(sectmentset);
      phraseset->Setphraseset(sectmentset.phraseset);
  }
  else if(str==QString(QLatin1String(sectmentset.harmonicset.name)))
   {
       iseditting=true;
       harmonicset=new Harmonic;
       connect(harmonicset,SIGNAL(sendsettingvalue(void *,int)),this,SLOT(addtestfunc(void *,int)));
       connect(harmonicset,SIGNAL(sendclose(int)),this,SLOT(closefuncwin(int)));
       harmonicset->show();
       harmonicset->settinglink=settinglink;
       harmonicset->setsectmentset(sectmentset);
       harmonicset->Setharmonicset(sectmentset.harmonicset);
   }

}

void Widget::on_listWidget_2_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem* curItem = ui->listWidget_2->itemAt( pos );
        if( curItem == NULL )
            return;

        QMenu *popMenu = new QMenu( this );
        QAction *UpSeed = new QAction(tr("Up"), this);
        QAction *DownSeeds = new QAction(tr("Down"), this);
        popMenu->addAction( UpSeed );
        popMenu->addAction( DownSeeds );
        connect( UpSeed, SIGNAL(triggered() ), this, SLOT( UpSeedSlot()) );
        connect( DownSeeds, SIGNAL(triggered() ), this, SLOT( DownSeedsSlot()) );
        popMenu->exec( QCursor::pos() );
        delete popMenu;
        delete UpSeed;
        delete DownSeeds;
}
void Widget::UpSeedSlot()
{
    QListWidgetItem * item = ui->listWidget_2->currentItem();
    if( item == NULL )
        return;

    //ui->listWidget_2->clear();
    if(nodelink==NULL||nodelink->next==NULL)
        return;



    int curIndex = ui->listWidget_2->row(item);
    int Index = curIndex-1;
    if(Index==0)
        return;

    QListWidgetItem *itemtmp=ui->listWidget_2->currentItem();
    if(itemtmp==NULL)
    return;
    //
    QString str=itemtmp->text();
    QByteArray ba=str.toLatin1();
    char *strchar=ba.data();
    on_pushButton_11_clicked();
    deletenode(nodelink,strchar); //é?3y??á′±í?Dμ??úμ?

    //on_pushButton_3_clicked();
    insertnodeIndex(nodelink,sectmentset,Index);
    updatesectionlist();
}
void Widget::DownSeedsSlot()
{
    QListWidgetItem * item = ui->listWidget_2->currentItem();
    if( item == NULL )
        return;

    //ui->listWidget_2->clear();
    if(nodelink==NULL||nodelink->next==NULL)
        return;



    int curIndex = ui->listWidget_2->row(item);
    int Index = curIndex+1;
    if(Index == ui->listWidget_2->count())
        return;

    QListWidgetItem *itemtmp=ui->listWidget_2->currentItem();
    if(itemtmp==NULL)
    return;
    //
    QString str=itemtmp->text();
    QByteArray ba=str.toLatin1();
    char *strchar=ba.data();
    on_pushButton_11_clicked();
    deletenode(nodelink,strchar); //é?3y??á′±í?Dμ??úμ?

    //on_pushButton_3_clicked();
    insertnodeIndex(nodelink,sectmentset,Index);
    updatesectionlist();
}
