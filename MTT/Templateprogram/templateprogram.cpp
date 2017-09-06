#include "templateprogram.h"
#include "ui_templateprogram.h"
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include "Uartset.h"
#include "nodeprocess.h"
#include "settingnodelink.h"
#include "myweitu.h"
#include <QMoveEvent>
#include <ShowTestItemPara/showtestitempara.h>



int runmode=1;
struct SECTMENTSET sectset;//一段的设置
int  sumsectnum;  //段的个数
bool protecttiontype[7];  //段保护类型标识 true 该保护类型有效 false; 该保护类型无效 增加保护类型时没，更改数组大小
int  currentsectid; //当前段的ID
int  currentprotecttype;  //当前的显示的保护类型
char ReportName[1024];
bool TestResultValid=false;
bool Saveflag=false;
int starttest=0;
int AuthorizeFileValid=1;  //1  有效
bool Threadrunstate=true;
bool SoftStartEndflag=false;


Templateprogram::Templateprogram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Templateprogram)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);


    ui->label_4->setText(("<font color=blue>"+QString("Template Test(2.0.0)")+"</font>"));

    showFullScreen();
    if(runmode==1)
      {  ui->radioButton->setChecked(true);
         ui->radioButton_2->setChecked(false);
      }
    else if(runmode==2)
      {  ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(true);
      }
    ui->tableWidget->setRowCount(8);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单个被选中

    QStringList str;
    str<<tr("name")<<tr("Item")<<tr("setting")<<tr("result")<<tr("Sel");
    ui->tableWidget->setHorizontalHeaderLabels(str);
    ui->tableWidget->verticalHeader()->setVisible(false);
    for(int i=0;i<8;i++)
    for(int j=0;j<5;j++)
    ui->tableWidget->setItem(i,j,new QTableWidgetItem);

    ui->tableWidget->setItemDelegate(new NoFocusDelegate());//设置鼠标行时，不显示单元格虚框
    ui->tableWidget->item(0,4)->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setContextMenuPolicy(Qt::NoContextMenu);
  //  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);





    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_5->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_6->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_7->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_8->setFocusPolicy(Qt::NoFocus);

    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);



    ui->label->hide();
    currentprotecttype=0; //当前保护类型，电流、电压、方向、频率、相位
    sumsectnum=0;
    runmode=1;

}



void Templateprogram::temprorarydatainit()
{
   //EM3352   S2 S1
   //MPF300   S1 S3
    memset(temprorarydata,0,sizeof(temprorarydata));
    int i=0;
    {
    temprorarydata->UartDev[i].valid=1;
    temprorarydata->UartDev[i].uartfd=0;
    temprorarydata->UartDev[i].baud=19200;
    temprorarydata->UartDev[i].datanum=8;
    temprorarydata->UartDev[i].parity='N';
    temprorarydata->UartDev[i].stopbit=1;
    sprintf(temprorarydata->UartDev[i].describ,"/dev/ttyS1");
    sprintf(temprorarydata->UartDev[i].devname,"/dev/ttyS1");
    temprorarydata->UartDev[i].rs485Mode=0;
    }
    i=1;
    {
    temprorarydata->UartDev[i].valid=1;
    temprorarydata->UartDev[i].uartfd=0;
    temprorarydata->UartDev[i].baud=9600;
    temprorarydata->UartDev[i].datanum=8;
    temprorarydata->UartDev[i].parity='N';
    temprorarydata->UartDev[i].stopbit=1;
    sprintf(temprorarydata->UartDev[i].describ,"/dev/ttyS3");
    sprintf(temprorarydata->UartDev[i].devname,"/dev/ttyS3");
    temprorarydata->UartDev[i].rs485Mode=0;
    }


    for(i=0;i<MAX_UARTDEV_NUM;i++)
    {
    temprorarydata->UartRcv[i].readsequence=0;
    temprorarydata->UartRcv[i].writesequence=0;
    }
    temprorarydata->setmanage_on=false;
    temprorarydata->Wholekeypadon=false;
    temprorarydata->ShowParaOn=false;
    temprorarydata->helpopenflag=false;
}

void Templateprogram::UartInit(int uartno)
{


  int i;
  i=uartno;
 {
    temprorarydata->UartDev[i].uartfd=-1;
    if((temprorarydata->UartDev[i].valid=='1')||(temprorarydata->UartDev[i].valid==1))
    {
    temprorarydata->UartDev[i].uartfd=OpenDev(temprorarydata->UartDev[i].devname);
    if(temprorarydata->UartDev[i].uartfd>0)
    {
    set_speed(temprorarydata->UartDev[i].uartfd,temprorarydata->UartDev[i].baud);
    set_Parity(temprorarydata->UartDev[i].uartfd,temprorarydata->UartDev[i].datanum,temprorarydata->UartDev[i].parity,temprorarydata->UartDev[i].stopbit);
    }
    }

  }
}

void Templateprogram::UartsInit()
{
    int i;
    for(i=0;i<MAX_UARTDEV_NUM;i++)
    {
     UartInit(i);
    }

}




void  Templateprogram::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len<0)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}

int Templateprogram::is_digit(char ch)
{
    if((ch>='0'&&ch<='9')||ch=='.')
        return 1;
    else
        return 0;
}


void Templateprogram::updatesetting(char *filename) //读取文件的ID name value
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

                                  if(strlen(setvalue.ID)!=0)
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
                                  if(strlen(setvalue.ID)!=0)
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

//type 1 电流 2 电压                                             相对/绝对     正向误差     负向误差                      段的值
void Templateprogram::processrelerror(int type,int subtype,int relabs,float value,float value_2,struct SECTMENTSET &tmpsectset)
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
                tmpsectset.curset.TripValue.assesstripvalue.abserror.errorvalue_Negtive=value_2;

                tmpsectset.curset.TripValue.assesstripvalue.abserror.valid=true;
            }
            else if(relabs>=3)
                tmpsectset.curset.TripValue.assesstripvalue.assessandor=relabs-2;

            break;
        case 2:
            if(relabs==1) //相对误差
             {  tmpsectset.curset.TripTime.assesstriptime.relerror.errorvalue=value;
                tmpsectset.curset.TripTime.assesstriptime.relerror.valid=true;
             }
             else if(relabs==2)
            {
                tmpsectset.curset.TripTime.assesstriptime.abserror.errorvalue=value;
                tmpsectset.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive=value_2;


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
            if(relabs==1) //相对误差
             {  tmpsectset.volset.TripValue.assesstripvalue.relerror.errorvalue=value;
                tmpsectset.volset.TripValue.assesstripvalue.relerror.valid=true;
             }
             else if(relabs==2)
            {
                tmpsectset.volset.TripValue.assesstripvalue.abserror.errorvalue=value;
                tmpsectset.volset.TripValue.assesstripvalue.abserror.errorvalue_Negtive=value_2;


                tmpsectset.volset.TripValue.assesstripvalue.abserror.valid=true;

            }
            else if(relabs>=3)
            tmpsectset.volset.TripValue.assesstripvalue.assessandor=relabs-2;
            break;
        case 2:
            if(relabs==1) //相对误差
             {  tmpsectset.volset.TripTime.assesstriptime.relerror.errorvalue=value;
                tmpsectset.volset.TripTime.assesstriptime.relerror.valid=true;
             }
             else if(relabs==2)
            {
                tmpsectset.volset.TripTime.assesstriptime.abserror.errorvalue=value;
                tmpsectset.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive=value_2;


                tmpsectset.volset.TripTime.assesstriptime.abserror.valid=true;
            }
            else if(relabs>=3)
            tmpsectset.volset.TripTime.assesstriptime.assessandor=relabs-2;
            break;
      }

        break;

    case 3://方向
            if(relabs==1) //相对误差
             {  tmpsectset.dirset.TripMta.assessmta.relerror.errorvalue=value;
                tmpsectset.dirset.TripMta.assessmta.relerror.valid=true;
             }
            else if(relabs==2)
            {
                tmpsectset.dirset.TripMta.assessmta.abserror.errorvalue=value;
                tmpsectset.dirset.TripMta.assessmta.abserror.errorvalue_Negtive=value_2;


                tmpsectset.dirset.TripMta.assessmta.abserror.valid=true;
            }
            else if(relabs>=3)
                 tmpsectset.dirset.TripMta.assessmta.assessandor=relabs-2;

            break;
    case 4://频率
        switch(subtype)
        {
        case 1: //动作频率
            if(relabs==1)
            {
             tmpsectset.freset.TripValue.assesstripvalue.relerror.errorvalue=value;
             tmpsectset.freset.TripValue.assesstripvalue.relerror.valid=true;
            }
            else if(relabs==2)
            {
            tmpsectset.freset.TripValue.assesstripvalue.abserror.errorvalue=value;
            tmpsectset.freset.TripValue.assesstripvalue.abserror.errorvalue_Negtive=value_2;


            tmpsectset.freset.TripValue.assesstripvalue.abserror.valid=true;



            }
            else if(relabs>=3)
             tmpsectset.freset.TripValue.assesstripvalue.assessandor=relabs-2;
            break;

        case 2: //动作时间
            if(relabs==1)
            {
             tmpsectset.freset.TripTime.assesstriptime.relerror.errorvalue=value;
             tmpsectset.freset.TripTime.assesstriptime.relerror.valid=true;
            }
            else if(relabs==2)
            {
            tmpsectset.freset.TripTime.assesstriptime.abserror.errorvalue=value;
            tmpsectset.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive=value_2;

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
            tmpsectset.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive=value_2;


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
             tmpsectset.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive=value_2;


             tmpsectset.freset.Tripdfdttime.assessdfdttriptime.abserror.valid=true;
             }
             else if(relabs>=3)
             tmpsectset.freset.Tripdfdttime.assessdfdttriptime.assessandor=relabs-2;

             break;

        case 16: //电压闭锁值
            if(relabs==1)
            {
             tmpsectset.freset.TripVBlock.assessvblock.relerror.errorvalue=value;
             tmpsectset.freset.TripVBlock.assessvblock.relerror.valid=true;
            }
            else if(relabs==2)
            {
            tmpsectset.freset.TripVBlock.assessvblock.abserror.errorvalue=value;
            tmpsectset.freset.TripVBlock.assessvblock.abserror.errorvalue_Negtive=value_2;



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
            if(relabs==1) //相对误差
             {  tmpsectset.phraseset.TripValue.assesstripvalue.relerror.errorvalue=value;
                tmpsectset.phraseset.TripValue.assesstripvalue.relerror.valid=true;
             }
             else if(relabs==2)
            {
                tmpsectset.phraseset.TripValue.assesstripvalue.abserror.errorvalue=value;
                tmpsectset.phraseset.TripValue.assesstripvalue.abserror.errorvalue_Negtive=value_2;



                tmpsectset.phraseset.TripValue.assesstripvalue.abserror.valid=true;
            }
            else if(relabs>=3)
                tmpsectset.phraseset.TripValue.assesstripvalue.assessandor=relabs-2;

            break;
        case 2:
            if(relabs==1) //相对误差
             {  tmpsectset.phraseset.TripTime.assesstriptime.relerror.errorvalue=value;
                tmpsectset.phraseset.TripTime.assesstriptime.relerror.valid=true;
             }
             else if(relabs==2)
            {
                tmpsectset.phraseset.TripTime.assesstriptime.abserror.errorvalue=value;
                tmpsectset.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive=value_2;

                tmpsectset.phraseset.TripTime.assesstriptime.abserror.valid=true;
            }
            else if(relabs>=3)
                tmpsectset.phraseset.TripTime.assesstriptime.assessandor=relabs-2;
            break;
      }
        break;

    case 7://谐波//2016-12-13
            if(relabs==1) //相对误差
             {  tmpsectset.harmonicset.TripValue.assesstripvalue.relerror.errorvalue=value;
                tmpsectset.harmonicset.TripValue.assesstripvalue.relerror.valid=true;
             }
            else if(relabs==2)
            {
                tmpsectset.harmonicset.TripValue.assesstripvalue.abserror.errorvalue=value;
                tmpsectset.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_Negtive=value_2;


                tmpsectset.harmonicset.TripValue.assesstripvalue.abserror.valid=true;
            }
            else if(relabs>=3)
                 tmpsectset.harmonicset.TripValue.assesstripvalue.assessandor=relabs-2;

            break;
    }

}
//解析 评估 字符
void Templateprogram::AnalysisAssesschar(char *assesschar,int type,int subtype,struct SECTMENTSET &sectsettmp)
{
    char *p1=assesschar;
    char *p2=assesschar;
    int  pos=0; //'|' 或'&'的位置
    float relerror;
    float abserror; //正向误差
    float abserror_2; //反向误差
    while(*p1!='\0')
    {
        if((*p1=='|')||(*p1=='&'))
        break;
        p1++;
        pos++;
    }
    if(*p1=='\0') //没有与 或者 |  &
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

void Templateprogram::ReadCurrentVariable(char *buf,struct SECTMENTSET &sectset,int type) //读电流的变量
{
    char *p1=buf;
    int i=0;
    char Testname[128];
    switch(type)
    {
    case 1: //触发值


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

        sectset.curset.TripValue.tripvalue=-2;


        break;
    case 2: //触发时间

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

       sectset.curset.TripTime.triptime=-2;
       break;
    }

}

void Templateprogram::ReadVoltageVariable(char *buf,struct SECTMENTSET &sectset,int type) //读电压的变量
{
    char *p1=buf;
    int i=0;
    char Testname[128];
    switch(type)
    {
    case 1: //触发值
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

        sectset.volset.TripValue.tripvalue=-2;

        break;
    case 2: //触发时间

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
       sectset.volset.TripTime.triptime=-2;
       break;
    }

}

void Templateprogram::ReadDirectionVariable(char *buf,struct SECTMENTSET &sectset) //读方向的变量
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

    sectset.dirset.TripMta.phrasemta=-1000;
}

void Templateprogram::ReadFrequencyVariable(char *buf,struct SECTMENTSET &sectset,int type) //读频率的变量
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

       sectset.freset.TripValue.tripfrec=-2;

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

          sectset.freset.TripTime.triptime=-2;

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
        sectset.freset.Tripdfdt.tripdfdt=-2;

        break;

   case 8: //滑差闭锁时间
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

        else if(memcmp(p1,"DURATION=",strlen("DURATION="))==0)
                {
                    p1=p1+strlen("DURATION=");
                    i=0;
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

        sectset.freset.Tripdfdttime.tripdfdttime=-2;

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

         sectset.freset.TripVBlock.VBlockTrip=-2;

        break;
    }



}
//2016-12-13 新增加harmonic
void Templateprogram::ReadHarmonicVariable(char *buf,struct SECTMENTSET &sectset) //读谐波的变量
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
        AnalysisAssesschar(Assessmethod,7,1,sectset);//2017-16-6 教训啊

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



void Templateprogram::ReadAnytestVariable(char *buf,struct SECTMENTSET &sectset) //读任意测试的变量
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

    //电流步长
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
    //电压步长
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
    //相位步长
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
    //频率步长
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
    //步长时间
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
    //步数
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
    //故障前时间
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
    //故障时间
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
    //故障后时间
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
    //变量VA

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
    //变量VB
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
    //变量VC
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
    //变量VZ
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
    //变量3U0
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
    //变量IA
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
    //变量IB
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
    //变量IC
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
    //测试模式
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

    //变化项目
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

  sectset.anytestset.triptime=-2;


}

void Templateprogram::ReadPhraseVariable(char *buf,struct SECTMENTSET &sectset,int type) //读相位测试的变量
{
       char *p1=buf;
       int i=0;
       char Testname[128];
       switch(type)
       {
       case 1: //触发值

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

           sectset.phraseset.TripValue.tripvalue=-1000;
           break;
       case 2: //触发时间

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
          sectset.phraseset.TripTime.triptime=-2;
          break;
       }


}


void Templateprogram::Processsection(char *filename)
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

       insertnode(sectmentlink_Auto,sectset,sumsectnum);
       memset(&sectset,0,sizeof(struct SECTMENTSET));
       sumsectnum++;
       }

}

void  Templateprogram::Analyzetemplatefile(char *filename)//解析模板文件
 {
     FILE *file= NULL;
     char buf[BUFFSIZE];
     sumsectnum=0;
     currentsectid=0;
     if((file=fopen(filename,"r")) == NULL)
       {
           printf("fopen error\n");
           return ;
       }

     memset(buf,'\0',BUFFSIZE);
     sectmentlink_Auto=Nodecreate();//创建段节点
     settinglink=(struct SETTINGNODELIST *)malloc(sizeof(struct SETTINGNODELIST)); //所有的定值链表
     settinglink->next=NULL;
     updatesetting(filename); //读所有的定值信息
     char tmpfilename[255];
     int num=1;
     while(fgets(buf,BUFFSIZE,file)!=NULL)
     {
         //将模板拷贝到另外一个同名的临时文件中处理
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
            if(memcmp(buf,"<SectionsettingEnd>",strlen("<SectionsettingEnd>"))==0) //如果最后一行为TestfunctionEnd 然后处理setting.tmp 文件
             Processsection(tmpfilename);
         }
     }
     fclose(file);
   //   printfsetting(settinglink);
    // printnode(sectmentlink_Auto);

     system("rm -rf *.mtp.tmp");

 }


void  Templateprogram::settemplatefilename(char *filename) //设置模板的文件名
{

    if(filename==NULL)
        return;
    QFile file;
    char tmpfilename[512];
    sprintf(tmpfilename,"./AuthorizeDir/%s",filename);

    if(file.exists(QString(QLatin1String(tmpfilename)))==false)
    {
    return;
    }
    strcpy(templatefilename,tmpfilename);
    Analyzetemplatefile(templatefilename); //解析模板文件
    temprorarydata=new TEMPRORARYDATA;
    memset(temprorarydata,0,sizeof(TEMPRORARYDATA));
    temprorarydatainit();
    UartsInit();
    showfirstpage(0); //显示第一页
    uartthread=new thread1;
    uartthread->settemprorarydata(temprorarydata);
    uartthread->setsettingnodelink(settinglink);
    uartthread->setsectmentnodelink(sectmentlink_Auto);


    connect(uartthread,SIGNAL(SIG_showtip(int)),this,SLOT(showtip(int)));
    connect(uartthread,SIGNAL(SIG_showfirstfuncofsect()),this,SLOT(showfuncofsect()));
    connect(uartthread,SIGNAL(SIG_showcurrenttestpos(int)),this,SLOT(showcurrenttestpos(int)));
    connect(uartthread,SIGNAL(SIG_cleartestitemcolor()),this,SLOT(cleartestitemcolor()));
    connect(uartthread,SIGNAL(SIG_SendProcessStr(char*)),this,SLOT(getthreadoutputstr(char *)));
    connect(uartthread,SIGNAL(SIG_AuthorizedFileExpire()),this,SLOT(ShowFileExpired()));
    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
    connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
    uartthread->start();
    //然后才是创建线程
}



void jiamiXor(char *str, char key2,char *result)
{

    for(int i=0;i<strlen(str); i++)
    result[i]= ~str[i]+key2;
    result[strlen(str)]='\0';
}

void jiemiXor(char *str, char key2,char *result)
{   char tmpchar;
    for(int i=0;i<strlen(str); i++)
     {
        tmpchar=str[i]-key2;
        result[i]=~tmpchar;
     }
    result[strlen(str)]='\0';
}

unsigned long Check(char *str)
{ unsigned int  i=0x01;
  unsigned int  m,j;
  unsigned long  Recheck=0;

  int len=0;
  char *p2=str;
  char temp[512];
  while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
  {
      temp[len]=*p2;
      p2++;
     len++;
  }
  temp[len]='\0';


  char *p1=temp;
  for(m=0;m<strlen(str);m++)
  { i=0x01;
    for(j=0;j<=7;j++)
    { if(*p1&i)
      {
       Recheck++;
      }
      i=i<<1;
    }
    p1++;
  }
  return  Recheck;
}




int  Templateprogram::WriteAuthrizedFile()
{

   char  *AuthrizedFilename;
   struct timeval tpend;
   gettimeofday(&tpend,NULL);
   unsigned long Checksum=0;


   QString str=QString(templatefilename);
   str.replace(QString(".mtp"),QString(".dat"),Qt::CaseSensitive);
   QByteArray ba=str.toLatin1();
   AuthrizedFilename=ba.data();

   FILE *fp=NULL;

   fp=fopen(AuthrizedFilename,"r");
   if(fp==NULL)
   {  printf("fopen error\n");
      return -3;
   }
   char key=0x13;
   char miwen[1024];
   char buf[1024];
   int sequnence=0;
   char *p2=NULL;
   char miwen_2[1024];
   char length=0;

   char SerialNum[512];
   char FileName[512];
   char AuthorizeTime[512];
   char ExpireTime[512];

   while(fgets(miwen,BUFFSIZE,fp)!=NULL)
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

       strcpy(SerialNum,temp);
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
         strcpy(FileName,temp);
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
         strcpy(AuthorizeTime,temp);
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
         strcpy(ExpireTime,temp);
         sequnence=4;
       }
       else if(sequnence==4&&memcmp(buf,"CHECKCODE=",strlen("CHECKCODE="))==0)
       {
        char *p1=buf+strlen("CHECKCODE=");

        unsigned long CRC=atol(p1);
        if(CRC!=Checksum)
        {
            fclose(fp);
            return -7;
        }
       }
   }
   fclose(fp);


    QDateTime Time1=QDateTime::fromString(AuthorizeTime,"yyyy.m.d.h.m.s");
    QDateTime Time2=QDateTime::fromString(ExpireTime,"yyyy.m.d.h.m.s");
    QDateTime currentdatetime=QDateTime::currentDateTime();

    if(currentdatetime>=Time1&&currentdatetime<=Time2)
    AuthorizeFileValid=1;  //1  有效
    else
    AuthorizeFileValid=0;  //1  有效

}




void Templateprogram::showfirstpage(int sectid)
{
    currentsectid=sectid;
    currentprotecttype=0;
    if(getsectfromid(sectmentlink_Auto,sectset,currentsectid)==true)
    {

        protecttiontype[0]=sectset.volset.valid;
        protecttiontype[1]=sectset.curset.valid;
        protecttiontype[2]=sectset.dirset.valid;
        protecttiontype[3]=sectset.freset.valid;
        protecttiontype[4]=sectset.anytestset.valid;
        protecttiontype[5]=sectset.phraseset.valid;
        protecttiontype[6]=sectset.harmonicset.valid;//2016-12-13
        ui->label->setText(QString(QLatin1String(sectset.name)));
        //找到第一个为true的位置
        for(int i=0;i<7;i++)//2016-12-13
            if(protecttiontype[i]==true)
            {
              currentprotecttype=i;
              break;
            }
            showprotection(currentprotecttype,currentsectid);
    }





}

void Templateprogram::showfuncofsect()
{

    ui->label->setText(QString(QLatin1String(sectset.name)));
    //找到第一个为true的位置
    showprotection(currentprotecttype,currentsectid);
}


void Templateprogram::showcurrenttestpos(int pos)
 {
     const QColor blue =QColor(0,0,255);
     const QColor white =QColor(255,255,255);
     ui->tableWidget->item(0,1)->setBackgroundColor(white);
     ui->tableWidget->item(1,1)->setBackgroundColor(white);
     ui->tableWidget->item(2,1)->setBackgroundColor(white);
     ui->tableWidget->item(3,1)->setBackgroundColor(white);
     ui->tableWidget->item(4,1)->setBackgroundColor(white);
     ui->tableWidget->item(5,1)->setBackgroundColor(white);
     ui->tableWidget->item(6,1)->setBackgroundColor(white);
     ui->tableWidget->item(pos-1,1)->setBackgroundColor(blue);//蓝色

 }

void Templateprogram::cleartestitemcolor()
{
    const QColor white =QColor(255,255,255);
    ui->tableWidget->item(0,1)->setBackgroundColor(white);
    ui->tableWidget->item(1,1)->setBackgroundColor(white);
    ui->tableWidget->item(2,1)->setBackgroundColor(white);
    ui->tableWidget->item(3,1)->setBackgroundColor(white);
    ui->tableWidget->item(4,1)->setBackgroundColor(white);
    ui->tableWidget->item(5,1)->setBackgroundColor(white);
    ui->tableWidget->item(6,1)->setBackgroundColor(white);
}




void Templateprogram::getreportname(char *name)
{

    if(name!=NULL&&(strlen(name)>0))
      {
          strcpy(ReportName,name);
          Saveflag=true;
      }
}


void Templateprogram::closereportkeypad()
{
temprorarydata->Wholekeypadon=false;
}



void Templateprogram::processpresect() //按下上一段按钮的设置
{
    if(currentsectid==0)
        return;
    currentsectid--;

    memset(&sectset,0,sizeof(struct SECTMENTSET));
    //通过ID返回该段,并显示 第一个有效的保护类型
    if(getsectfromid(sectmentlink_Auto,sectset,currentsectid)==true)
    {

        ui->label->setText(QString(QLatin1String(sectset.name)));
        protecttiontype[0]=sectset.volset.valid;
        protecttiontype[1]=sectset.curset.valid;
        protecttiontype[2]=sectset.dirset.valid;
        protecttiontype[3]=sectset.freset.valid;
        protecttiontype[4]=sectset.anytestset.valid;
        protecttiontype[5]=sectset.phraseset.valid;
        protecttiontype[6]=sectset.harmonicset.valid;//2016-12-13

        //找到第一个为true的位置
        for(int i=6;i>=0;i--)//2016-12-13
            if(protecttiontype[i]==true)
            {
              currentprotecttype=i;
              break;
            }
           showprotection(currentprotecttype,currentsectid);
    }


}

void Templateprogram::processnextsect() //按下下一段按钮的设置
{
    if(currentsectid==(sumsectnum-1))
        return;
    currentsectid++;
    memset(&sectset,0,sizeof(struct SECTMENTSET));

    if(getsectfromid(sectmentlink_Auto,sectset,currentsectid)==true)
    {
        ui->label->setText(QString(QLatin1String(sectset.name)));
        protecttiontype[0]=sectset.volset.valid;
        protecttiontype[1]=sectset.curset.valid;
        protecttiontype[2]=sectset.dirset.valid;
        protecttiontype[3]=sectset.freset.valid;
        protecttiontype[4]=sectset.anytestset.valid;
        protecttiontype[5]=sectset.phraseset.valid;
        protecttiontype[6]=sectset.harmonicset.valid;//2016-12-13
        //找到第一个为true的位置
        for(int i=0;i<=6;i++)//2016-12-13
            if(protecttiontype[i]==true)
            {
              currentprotecttype=i;
              break;
            }
        showprotection(currentprotecttype,currentsectid);
    }
}

void Templateprogram::processprepage()
{
    if(currentprotecttype==0)
    {
        processpresect();
        return;
    }

    int i;
    for(i=currentprotecttype-1;i>=0;i--)
       if(protecttiontype[i]==true)
       {   currentprotecttype=i;
            showprotection(currentprotecttype,currentsectid);
            return;
       }
    if(i<0)
    {
       processpresect();
    }
}

void Templateprogram::processnextpage()
{
    if(currentprotecttype==6)//2016-12-13
    {
        processnextsect();
        return;
    }
      int i=0;
      for(i=currentprotecttype+1;i<=6;i++)//2016-12-13
       if(protecttiontype[i]==true)
       { currentprotecttype=i;
         showprotection(currentprotecttype,currentsectid);
         return;
       }
      if(i==7) //此段已经没有任何其他测试功能//2016-12-13
      {
          processnextsect();
      }
}


void Templateprogram::showtip(int num)
{
    if(num==1)
    {
      tip=new Tip;
      tip->settip(1);
      tip->show();
      tip->setModal(true);
    }
    else if(num==2)
    {
        tip=new Tip;
        tip->settip(2);
        tip->show();
        tip->setModal(true);
    }
    else if(num==0)
    {
        if(tip!=NULL)
        {
            tip->close();
        }
    }


}

void Templateprogram::returnsetlink(void *set)
{
settinglink=(struct SETTINGNODELIST *)set;
uartthread->setsettingnodelink(settinglink);
//更新显示
}

void Templateprogram::Closesectmange()
{
  temprorarydata->setmanage_on=false;

}

void Templateprogram::Closeshowpara()
{
 temprorarydata->ShowParaOn=false;
 showprotection(currentprotecttype,currentsectid);
}

void Templateprogram::GetNewSectset(void *value) //获取新的段设置
{
    struct SECTMENTSETNODE *sectmentsettmp=getsectnodefromid(sectmentlink_Auto,currentsectid);
    if(sectmentsettmp!=NULL)
    {
      struct SECTMENTSET sectvalue=*((struct SECTMENTSET *)value);
      memcpy(&sectmentsettmp->value,&sectvalue,sizeof(struct SECTMENTSET));
      uartthread->setsectmentnodelink(sectmentlink_Auto);
      memcpy(&sectset,&sectvalue,sizeof(struct SECTMENTSET));
    }


}



void Templateprogram::showprotection(int type,int sectmentid)  //显示某一保护类型的数据
{
    int row=ui->tableWidget->rowCount();
    int col=ui->tableWidget->columnCount();


    ui->tableWidget->clear();
    QStringList str;
    str<<tr("name")<<tr("Item")<<tr("setting")<<tr("result")<<tr("Sel");
    ui->tableWidget->setHorizontalHeaderLabels(str);
    ui->tableWidget->verticalHeader()->setVisible(false);
    for(int i=0;i<row;i++)
    for(int j=0;j<col;j++)
    ui->tableWidget->setItem(i,j,new QTableWidgetItem);




    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,270);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,80);


    int Testitem=0;
    int testitemvaidnum=0; //测试项目有效个数
    switch(type)
    {
    case 0: //电压

        Testitem=atoi(sectset.volset.Testitem.text);
        ui->tableWidget->item(0,0)->setText(QString(QLatin1String(sectset.volset.name)));
        if((Testitem&0x01)==0x01)
        {
        ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Voltage"));
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.volset.TripValue.assesstripvalue.CompareID));
        if(sectset.volset.TripValue.tripvalue!=-2)
        {
           if(sectset.volset.TripValue.tripvalue==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.volset.TripValue.tripvalue,'g',6));
        }

        {
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            if(sectset.volset.TripValue.valid==true)
            checkBox->setIcon(QIcon(":/dagou.png"));
            else
            checkBox->setIcon(QIcon(":/dacha.png"));
            ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
        }

        testitemvaidnum++;
        }


        if((Testitem&0x02)==0x02)
        {

            {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                if(sectset.volset.TripTime.valid==true)
                checkBox->setIcon(QIcon(":/dagou.png"));
                else
                checkBox->setIcon(QIcon(":/dacha.png"));
                ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
            }

        if(sectset.volset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
           sectset.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)  //稳定性测试
       {
             ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Stability"));
             char temp[512];
             CaculateVariable(settinglink,sectset.volset.TripTime.EndVaule);
             CaculateVariable(settinglink,sectset.volset.TripTime.HoldTime);
             sprintf(temp,"Vol(V):%.3fV Time(s):%.3f",\
                     sectset.volset.TripTime.EndVaule.value,\
                     sectset.volset.TripTime.HoldTime.value);
             ui->tableWidget->item(testitemvaidnum,2)->setText(temp);
            if(sectset.volset.TripTime.triptime!=-2)
            {
             if(sectset.volset.TripTime.triptime==-1)
             ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
             else
             ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.volset.TripTime.triptime,'g',6));
            }
            testitemvaidnum++;
          //  sprintf(temp,"Duration:%.3f",sectset.volset.TripTime.HoldTime.value);
           // ui->tableWidget->item(testitemvaidnum,2)->setText(temp);
          //  testitemvaidnum++;
       }
       else
        {
        ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Time"));
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.volset.TripTime.assesstriptime.CompareID));
        if(sectset.volset.TripTime.triptime!=-2)
        {
           if(sectset.volset.TripTime.triptime==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.volset.TripTime.triptime,'g',6));
        }
        testitemvaidnum++;
        }

      }

        break;
    case 1: //电流
        Testitem=atoi(sectset.curset.Testitem.text);
        ui->tableWidget->item(0,0)->setText(QString(QLatin1String(sectset.curset.name)));
        if((Testitem&0x01)==0x01)
        {
        ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Current"));
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.curset.TripValue.assesstripvalue.CompareID));
        if(sectset.curset.TripValue.tripvalue!=-2)
        {
           if(sectset.curset.TripValue.tripvalue==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.curset.TripValue.tripvalue,'g',6));
        }

        {
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            if(sectset.curset.TripValue.valid==true)
            checkBox->setIcon(QIcon(":/dagou.png"));
            else checkBox->setIcon(QIcon(":/dacha.png"));
            ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
        }
        testitemvaidnum++;
        }
        if((Testitem&0x02)==0x02)
      {

          {
              QTableWidgetItem *checkBox = new QTableWidgetItem();
              if(sectset.curset.TripTime.valid==true)
              checkBox->setIcon(QIcon(":/dagou.png"));
              else checkBox->setIcon(QIcon(":/dacha.png"));
              ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
          }

        if(sectset.curset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
           sectset.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
        {
         ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Stability"));
         char temp[512];
         CaculateVariable(settinglink,sectset.curset.TripTime.HoldTime);
         CaculateVariable(settinglink,sectset.curset.TripTime.EndVaule);

        sprintf(temp,"Cur(A):%.3f Time(s):%.3f",\
                sectset.curset.TripTime.EndVaule.value,\
                sectset.curset.TripTime.HoldTime.value);
        ui->tableWidget->item(testitemvaidnum,2)->setText(temp);
        if(sectset.curset.TripTime.triptime!=-2)
        {
           if(sectset.curset.TripTime.triptime==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.curset.TripTime.triptime,'g',6));
        }
        testitemvaidnum++;
      //  sprintf(temp,"Duration:%.3f",sectset.curset.TripTime.HoldTime.value);
      //  ui->tableWidget->item(testitemvaidnum,2)->setText(temp);
     //   testitemvaidnum++;
        }
        else
        {
        ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Time"));
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.curset.TripTime.assesstriptime.CompareID));
        if(sectset.curset.TripTime.triptime!=-2)
        {
           if(sectset.curset.TripTime.triptime==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.curset.TripTime.triptime,'g',6));
        }
        testitemvaidnum++;
        }
     }

        break;
    case 2: //方向
        ui->tableWidget->item(0,0)->setText(QString(QLatin1String(sectset.dirset.name)));
        ui->tableWidget->item(testitemvaidnum,1)->setText("MTA");
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.dirset.TripMta.assessmta.CompareID));
        if(sectset.dirset.TripMta.phrasemta!=-1000)
        {
           if(sectset.dirset.TripMta.phrasemta==-500)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.dirset.TripMta.phrasemta,'g',6));
        }


    {
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            if(sectset.dirset.valid==true)
            checkBox->setIcon(QIcon(":/dagou.png"));
            else
            checkBox->setIcon(QIcon(":/dacha.png"));
            ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
    }

        testitemvaidnum++;
        break;
    case 3: //频率
        Testitem=atoi(sectset.freset.TestItem.text);

        ui->tableWidget->item(0,0)->setText(QString(QLatin1String(sectset.freset.name)));
        if((Testitem&0x01)==0x01)
        {
            ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Freq"));
            ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.freset.TripValue.assesstripvalue.CompareID));
            if(sectset.freset.TripValue.tripfrec!=-2)
            {
               if(sectset.freset.TripValue.tripfrec==-1)
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
               else
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.freset.TripValue.tripfrec,'g',6));
            }

            {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                if(sectset.freset.TripValue.valid==true)
                checkBox->setIcon(QIcon(":/dagou.png"));
                else checkBox->setIcon(QIcon(":/dacha.png"));
                ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
            }
            testitemvaidnum++;
        }
        if((Testitem&0x02)==0x02)
        {


            {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                if(sectset.freset.TripTime.valid==true)
                checkBox->setIcon(QIcon(":/dagou.png"));
                else checkBox->setIcon(QIcon(":/dacha.png"));
                ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
            }

            if(sectset.freset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
               sectset.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
           {
            ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Freq Stability"));
            char temp[512];
            CaculateVariable(settinglink,sectset.freset.TripTime.FreEndValue);
            CaculateVariable(settinglink,sectset.freset.TripTime.HoldTime);


            sprintf(temp,"Freq(Hz):%.3f Time(s):%.3f",\
            sectset.freset.TripTime.FreEndValue.value,\
            sectset.freset.TripTime.HoldTime.value);
            ui->tableWidget->item(testitemvaidnum,2)->setText(QString(temp));
            if(sectset.freset.TripTime.triptime!=-2)
            {
               if(sectset.freset.TripTime.triptime==-1)
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
               else
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.freset.TripTime.triptime,'g',6));
            }
            testitemvaidnum++;
         //   sprintf(temp,"Duration:%.3f",sectset.freset.TripTime.HoldTime.value);
         //   ui->tableWidget->item(testitemvaidnum,2)->setText(QString(temp));
         //   testitemvaidnum++;
           }
           else
           {
            ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Time"));
            ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.freset.TripTime.assesstriptime.CompareID));
            if(sectset.freset.TripTime.triptime!=-2)
            {
               if(sectset.freset.TripTime.triptime==-1)
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
               else
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.freset.TripTime.triptime,'g',6));
            }

            testitemvaidnum++;
        }
     }

        if((Testitem&0x04)==0x04)
        {
            ui->tableWidget->item(testitemvaidnum,1)->setText(tr("dfdt trip value"));
            ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.freset.Tripdfdt.assessdfdttrip.CompareID));
            if(sectset.freset.Tripdfdt.tripdfdt!=-2)
            {
               if(sectset.freset.Tripdfdt.tripdfdt==-1)
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
               else
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.freset.Tripdfdt.tripdfdt,'g',6));
            }

            {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                if(sectset.freset.Tripdfdt.valid==true)
                checkBox->setIcon(QIcon(":/dagou.png"));
                else checkBox->setIcon(QIcon(":/dacha.png"));
                ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
            }
            testitemvaidnum++;
        }
        if((Testitem&0x08)==0x08)
        {

            {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                if(sectset.freset.Tripdfdttime.valid==true)
                checkBox->setIcon(QIcon(":/dagou.png"));
                else checkBox->setIcon(QIcon(":/dacha.png"));
                ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
            }

           if(sectset.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue==-1&&\
              sectset.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive==-1)
           {
             ui->tableWidget->item(testitemvaidnum,1)->setText(tr("dfdt stability"));
             char temp[512];
             CaculateVariable(settinglink,sectset.freset.Tripdfdttime.dfdt);
             CaculateVariable(settinglink,sectset.freset.Tripdfdttime.Duration);
             sprintf(temp,"dfdt(Hz/s):%.3f Time(s):%.3f",\
             sectset.freset.Tripdfdttime.dfdt.value,\
             sectset.freset.Tripdfdttime.Duration.value);
             ui->tableWidget->item(testitemvaidnum,2)->setText(QString(temp));
             if(sectset.freset.Tripdfdttime.tripdfdttime!=-2)
             {
                if(sectset.freset.Tripdfdttime.tripdfdttime==-1)
                ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
                else
                ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.freset.Tripdfdttime.tripdfdttime,'g',6));
             }
             testitemvaidnum++;
            // sprintf(temp,"Duration:%.3f",\
          //   sectset.freset.Tripdfdttime.Duration.value);
          //   ui->tableWidget->item(testitemvaidnum,2)->setText(QString(temp));
          //   testitemvaidnum++;
           }
           else
           {
            ui->tableWidget->item(testitemvaidnum,1)->setText(tr("dfdt trip time"));
            ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.freset.Tripdfdttime.assessdfdttriptime.CompareID));
            if(sectset.freset.Tripdfdttime.tripdfdttime!=-2)
            {
               if(sectset.freset.Tripdfdttime.tripdfdttime==-1)
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
               else
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.freset.Tripdfdttime.tripdfdttime,'g',6));
            }
            testitemvaidnum++;
           }


        }

        if((Testitem&0x10)==0x10)
        {
            ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Voltage Blocking"));
            ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.freset.TripVBlock.assessvblock.CompareID));
            if(sectset.freset.TripVBlock.VBlockTrip!=-2)
            {
               if(sectset.freset.TripVBlock.VBlockTrip==-1)
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
               else
               ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.freset.TripVBlock.VBlockTrip,'g',6));
            }

            {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                if(sectset.freset.TripVBlock.valid==true)
                checkBox->setIcon(QIcon(":/dagou.png"));
                else checkBox->setIcon(QIcon(":/dacha.png"));
                ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
            }

            testitemvaidnum++;
        }


        break;
    case 4: //任意测试

        /*
        ui->tableWidget->item(0,0)->setText(QString(QLatin1String(sectset.anytestset.name)));
        ui->tableWidget->item(0,1)->setText(QString("Trip Time"));
        if(sectset.anytestset.triptime!=-2)
        {
           if(sectset.anytestset.triptime==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.anytestset.triptime,'g',6));
        }


        if(sectset.anytestset.valid==true)
        {
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            if(sectset.anytestset.done==true)
            checkBox->setIcon(QIcon(":/dagou.png"));
            else checkBox->setIcon(QIcon(":/dacha.png"));
            ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
       }
       */

        break;
    case 5: //相位测试
        Testitem=atoi(sectset.phraseset.Testitem.text);
        ui->tableWidget->item(0,0)->setText(QString(QLatin1String(sectset.phraseset.name)));
        if((Testitem&0x01)==0x01)
        {
        ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Phase"));
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.phraseset.TripValue.assesstripvalue.CompareID));
        if(sectset.phraseset.TripValue.tripvalue!=-1000)
        {
           if(sectset.phraseset.TripValue.tripvalue==-500)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.phraseset.TripValue.tripvalue,'g',6));
        }

        {
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            if(sectset.phraseset.TripValue.valid==true)
            checkBox->setIcon(QIcon(":/dagou.png"));
            else checkBox->setIcon(QIcon(":/dacha.png"));
            ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
        }

        testitemvaidnum++;
        }
        if((Testitem&0x02)==0x02)
        {

            {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                if(sectset.phraseset.TripTime.valid==true)
                checkBox->setIcon(QIcon(":/dagou.png"));
                else
                checkBox->setIcon(QIcon(":/dacha.png"));
                ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
            }

        if(sectset.phraseset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
           sectset.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
        {
        ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Stability"));
        char temp[512];
        CaculateVariable(settinglink,sectset.phraseset.TripTime.EndVaule);
        CaculateVariable(settinglink,sectset.phraseset.TripTime.HoldTime);

        sprintf(temp,"Pha(Deg):%.3f Time(s):%.3f",\
        sectset.phraseset.TripTime.EndVaule.value,\
        sectset.phraseset.TripTime.HoldTime.value);

        ui->tableWidget->item(testitemvaidnum,2)->setText(temp);
        if(sectset.phraseset.TripTime.triptime!=-2)
        {
           if(sectset.phraseset.TripTime.triptime==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.phraseset.TripTime.triptime,'g',6));
        }
        testitemvaidnum++;
     //   sprintf(temp,"Duration:%.3f",sectset.phraseset.TripTime.HoldTime.value);
     //   ui->tableWidget->item(testitemvaidnum,2)->setText(temp);
    //    testitemvaidnum++;
        }
        else
        {
        ui->tableWidget->item(testitemvaidnum,1)->setText(tr("Trip Time"));
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.phraseset.TripTime.assesstriptime.CompareID));
        if(sectset.phraseset.TripTime.triptime!=-2)
        {
           if(sectset.phraseset.TripTime.triptime==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.phraseset.TripTime.triptime,'g',6));
        }
        testitemvaidnum++;
        }
     }

        break;
   case 6: //谐波//2016-12-13 新增谐波
        ui->tableWidget->item(0,0)->setText(QString(QLatin1String(sectset.harmonicset.name)));
        ui->tableWidget->item(testitemvaidnum,1)->setText("Trip Value");
        ui->tableWidget->item(testitemvaidnum,2)->setText(QString(sectset.harmonicset.TripValue.assesstripvalue.CompareID));
        if(sectset.harmonicset.TripValue.tripvalue!=-2)
        {
           if(sectset.harmonicset.TripValue.tripvalue==-1)
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString("no trip"));
           else
           ui->tableWidget->item(testitemvaidnum,3)->setText(QString::number(sectset.harmonicset.TripValue.tripvalue,'g',6));
        }


    {
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            if(sectset.harmonicset.valid==true)
            checkBox->setIcon(QIcon(":/dagou.png"));
            else
            checkBox->setIcon(QIcon(":/dacha.png"));
            ui->tableWidget->setItem(testitemvaidnum,4,checkBox);
    }

        testitemvaidnum++;
        break;

}

}

Templateprogram::~Templateprogram()
{
    delete ui;
}

void Templateprogram::on_pushButton_clicked() //上一页
{
    if(starttest==1)
        return;
    processprepage();
}


void Templateprogram::on_pushButton_2_clicked() //下一页
{
    if(starttest==1)
        return;
    processnextpage();
}

void Templateprogram::on_pushButton_5_clicked() //定值管理
{

    if(starttest==1)
        return;

    if(temprorarydata->setmanage_on==false)
  {  settingmanage=new SettingManage;
    settingmanage->Setsettinglink(settinglink);
    temprorarydata->setmanage_on=true;
    connect(settingmanage,SIGNAL(SIG_returnsetlink(void*)),this,SLOT(returnsetlink(void *)));
    connect(settingmanage,SIGNAL(SIG_Close()),this,SLOT(Closesectmange()));
    settingmanage->show();
    settingmanage->raise();
    settingmanage->activateWindow();
    }
}

void Templateprogram::on_radioButton_clicked() //自动
{
    if(starttest==1)
    {
     if(runmode==1)
     {
         ui->radioButton->setChecked(true);
         ui->radioButton_2->setChecked(false);
     }
    else
     {
         ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(true);
     }
     return;
    }



    ui->radioButton->setChecked(true);
    ui->radioButton_2->setChecked(false);
    uartthread->setsumfucos(8);
    setdone(sectmentlink_Auto);
    if(getsectfromid(sectmentlink_Auto,sectset,currentsectid)==true)
    {
        protecttiontype[0]=sectset.volset.valid;
        protecttiontype[1]=sectset.curset.valid;
        protecttiontype[2]=sectset.dirset.valid;
        protecttiontype[3]=sectset.freset.valid;
        protecttiontype[4]=sectset.anytestset.valid;
        protecttiontype[5]=sectset.phraseset.valid;
        protecttiontype[6]=sectset.harmonicset.valid;//2016-12-13
        //找到第一个为true的位置
        for(int i=0;i<=6;i++)//2016-12-13
            if(protecttiontype[i]==true)
            {
              currentprotecttype=i;
            }
    showprotection(currentprotecttype,currentsectid);
    }
    //将所有的done设置为true
    runmode=1;

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);







}

void Templateprogram::on_radioButton_2_clicked() //手动
{

    if(starttest==1)
    {
     if(runmode==1)
     {
         ui->radioButton->setChecked(true);
         ui->radioButton_2->setChecked(false);
     }
    else
     {
         ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(true);
     }
     return;
    }



    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(true);
    uartthread->setsumfucos(9);
    runmode=2;
   // ui->tableWidget->item(0,4)->setTextAlignment(Qt::AlignCenter);
   // ui->tableWidget->setEditTriggers(QAbstractItemView::CurrentChanged);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


}

void Templateprogram::on_pushButton_6_clicked() //帮助
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helpopenflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
helpdialog->SetHelpfilename("Templateprogram.txt");
helpdialog->show();
}

void Templateprogram::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

}

void Templateprogram::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}

void Templateprogram::on_pushButton_7_clicked() //保存报告
{
    if(starttest==1)
        return;


    if(TestResultValid==true)
   {
    keypad=new  KeyPadDialog;
    temprorarydata->Wholekeypadon=true;
    connect(keypad,SIGNAL(SIG_sendtext(char*)),this,SLOT(getreportname(char*)));
    connect(keypad,SIGNAL(SIG_closeflag()),this,SLOT(closereportkeypad()));

    keypad->activateWindow();
    keypad->raise();
    keypad->show();
   }

}

void delaymsec(int msec)
{
    QTime n=QTime::currentTime();
    QTime now;
    do{
      now=QTime::currentTime();
    }while (n.msecsTo(now)<=msec);

}


void Templateprogram::CloseWidget()
{


}


void Templateprogram::on_pushButton_8_clicked() //退出
{

   if(starttest==1)
    return;

   uartthread->stoprun();
   while(uartthread->isRunning())
   {
       uartthread->stoprun();
       sleep(1);
       break;
   }
   free(sectmentlink_Auto); //段的聊表
   free(settinglink);  // 定值的聊表 ID NAME VALUE
   free(temprorarydata);
   sectmentlink_Auto=NULL;
   settinglink=NULL;
   temprorarydata=NULL;
   close();

}

void Templateprogram::on_tableWidget_cellClicked(int row, int column) //测试 选择
{
    if(starttest==1)
        return;


    if(column==4)//点击sel这一列的操作
   {
    struct SECTMENTSETNODE *sectmentsettmp=getsectnodefromid(sectmentlink_Auto,currentsectid);
    if(sectmentsettmp!=NULL)
    {

        QTableWidgetItem *checkbox=ui->tableWidget->item(row,column);
        int CurrentTestitem=row;
        int Testitem;
        switch(currentprotecttype)
        {
         case 0: //电压
         {

            Testitem=atoi(sectmentsettmp->value.volset.Testitem.text);
            if(Testitem==0x01)
            {
                if(CurrentTestitem==0)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.volset.TripValue.valid=!sectmentsettmp->value.volset.TripValue.valid;
                     if(sectmentsettmp->value.volset.TripValue.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
            }
            else if(Testitem==0x02)
            {
                if(CurrentTestitem==0)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.volset.TripTime.valid=!sectmentsettmp->value.volset.TripTime.valid;
                     if(sectmentsettmp->value.volset.TripTime.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
            }
            else if(Testitem==0x03)
            {
                if(CurrentTestitem==1)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.volset.TripTime.valid=!sectmentsettmp->value.volset.TripTime.valid;
                     if(sectmentsettmp->value.volset.TripTime.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
                else if(CurrentTestitem==0)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.volset.TripValue.valid=!sectmentsettmp->value.volset.TripValue.valid;
                     if(sectmentsettmp->value.volset.TripValue.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
            }

         }
            break;
         case 1: //电流
           {
            Testitem=atoi(sectmentsettmp->value.curset.Testitem.text);
            if(Testitem==0x01)
            {
                if(CurrentTestitem==0)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.curset.TripValue.valid=!sectmentsettmp->value.curset.TripValue.valid;
                     if(sectmentsettmp->value.curset.TripValue.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
            }
            else if(Testitem==0x02)
            {
                if(CurrentTestitem==0)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.curset.TripTime.valid=!sectmentsettmp->value.curset.TripTime.valid;
                     if(sectmentsettmp->value.curset.TripTime.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
            }
            else if(Testitem==0x03)
            {
                if(CurrentTestitem==1)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.curset.TripTime.valid=!sectmentsettmp->value.curset.TripTime.valid;
                     if(sectmentsettmp->value.curset.TripTime.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
                else if(CurrentTestitem==0)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.curset.TripValue.valid=!sectmentsettmp->value.curset.TripValue.valid;
                     if(sectmentsettmp->value.curset.TripValue.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
            }
           }
            break;
         case 2: //方向
             if(runmode==2)
             sectmentsettmp->value.dirset.valid=!sectmentsettmp->value.dirset.valid;
             if(sectmentsettmp->value.dirset.valid)
                 checkbox->setIcon(QIcon(":/dagou.png"));
             else checkbox->setIcon(QIcon(":/dacha.png"));


            break;
         case 3: //频率
        {

         int FreqTestitem=(int)(atof(sectmentsettmp->value.freset.TestItem.text));
         bool ValidArray[5]={FreqTestitem&0x01,\
                             FreqTestitem&0x02,\
                             FreqTestitem&0x04,\
                             FreqTestitem&0x08,\
                             FreqTestitem&0x10};
         int ValidNum=0; //有效的个数
         for(int i=0;i<5;i++)
         {
             if(ValidArray[i]==true)
              ValidNum++;
         }


         if((CurrentTestitem+1)<=ValidNum)
          {
            int Pos=0;
            int i=0;
            for(i=0;i<5;i++)
            { if(ValidArray[i]==true)
              Pos++;
              if(Pos==(CurrentTestitem+1))
              break;
            }
            switch(i)
            {
             case 0:
                if(runmode==2)
                sectmentsettmp->value.freset.TripValue.valid=!sectmentsettmp->value.freset.TripValue.valid;
                if(sectmentsettmp->value.freset.TripValue.valid)
                    checkbox->setIcon(QIcon(":/dagou.png"));
                else checkbox->setIcon(QIcon(":/dacha.png"));

                break;
             case 1:
                if(runmode==2)
                sectmentsettmp->value.freset.TripTime.valid=!sectmentsettmp->value.freset.TripTime.valid;
               if(sectmentsettmp->value.freset.TripTime.valid)
                    checkbox->setIcon(QIcon(":/dagou.png"));
                else checkbox->setIcon(QIcon(":/dacha.png"));

               break;

             case 2:
                if(runmode==2)
                sectmentsettmp->value.freset.Tripdfdt.valid=!sectmentsettmp->value.freset.Tripdfdt.valid;

                if(sectmentsettmp->value.freset.Tripdfdt.valid)
                    checkbox->setIcon(QIcon(":/dagou.png"));
                else checkbox->setIcon(QIcon(":/dacha.png"));

                break;
             case 3:
                if(runmode==2)
                sectmentsettmp->value.freset.Tripdfdttime.valid=!sectmentsettmp->value.freset.Tripdfdttime.valid;
                if(sectmentsettmp->value.freset.Tripdfdttime.valid)
                    checkbox->setIcon(QIcon(":/dagou.png"));
                else checkbox->setIcon(QIcon(":/dacha.png"));

                break;

             case 4:
                if(runmode==2)
                sectmentsettmp->value.freset.TripVBlock.valid=!sectmentsettmp->value.freset.TripVBlock.valid;

                if(sectmentsettmp->value.freset.TripVBlock.valid)
                    checkbox->setIcon(QIcon(":/dagou.png"));
                else checkbox->setIcon(QIcon(":/dacha.png"));


                break;

            }
          }

        }


            break;
         case 4: //任意测试

          break;
        case 5:
          {
            Testitem=atoi(sectmentsettmp->value.phraseset.Testitem.text);

           if(Testitem==0x01)
           {
               if(CurrentTestitem==0)
                {
                    if(runmode==2)
                    sectmentsettmp->value.phraseset.TripValue.valid=!sectmentsettmp->value.phraseset.TripValue.valid;
                    if(sectmentsettmp->value.phraseset.TripValue.valid)
                         checkbox->setIcon(QIcon(":/dagou.png"));
                    else checkbox->setIcon(QIcon(":/dacha.png"));
                }
           }
           else if(Testitem==0x02)
           {
               if(CurrentTestitem==0)
                {
                    if(runmode==2)
                    sectmentsettmp->value.phraseset.TripTime.valid=!sectmentsettmp->value.phraseset.TripTime.valid;
                    if(sectmentsettmp->value.phraseset.TripTime.valid)
                         checkbox->setIcon(QIcon(":/dagou.png"));
                    else checkbox->setIcon(QIcon(":/dacha.png"));
                }
           }
           else if(Testitem==0x03)
           {

                if(CurrentTestitem==0)
                {
                    if(runmode==2)
                    sectmentsettmp->value.phraseset.TripValue.valid=!sectmentsettmp->value.phraseset.TripValue.valid;


                    if(sectmentsettmp->value.phraseset.TripValue.valid)
                         checkbox->setIcon(QIcon(":/dagou.png"));
                    else checkbox->setIcon(QIcon(":/dacha.png"));
                }
               else if(CurrentTestitem==1)
                 {
                     if(runmode==2)
                     sectmentsettmp->value.phraseset.TripTime.valid=!sectmentsettmp->value.phraseset.TripTime.valid;


                     if(sectmentsettmp->value.phraseset.TripTime.valid)
                          checkbox->setIcon(QIcon(":/dagou.png"));
                     else checkbox->setIcon(QIcon(":/dacha.png"));
                 }
           }
        }

        case 6: //谐波//2016-12-13
            if(runmode==2)
            sectmentsettmp->value.harmonicset.valid=!sectmentsettmp->value.harmonicset.valid;
            if(sectmentsettmp->value.harmonicset.valid)
                checkbox->setIcon(QIcon(":/dagou.png"));
            else checkbox->setIcon(QIcon(":/dacha.png"));

           break;
        }
        setsectfromid(sectmentlink_Auto,currentsectid,sectmentsettmp->value);
        memcpy(&sectset,&sectmentsettmp->value,sizeof(struct SECTMENTSET));
        uartthread->setsectmentnodelink(sectmentlink_Auto);

   }
   }
   else if(column==1||column==2)//点击Item或setting这两列的操作
{
    struct SECTMENTSETNODE *sectmentsettmp=getsectnodefromid(sectmentlink_Auto,currentsectid);
    if(sectmentsettmp!=NULL)
    {
     int CurrentTestitem=row;
     switch(currentprotecttype)
     {
     case 0:
     {
         int Testitem=(int)(atof(sectmentsettmp->value.volset.Testitem.text));
         bool ValidArray[2]={Testitem&0x01,\
                             Testitem&0x02};
         int ValidNum=0; //有效的个数
         for(int i=0;i<2;i++)
         {
             if(ValidArray[i]==true)
              ValidNum++;
         }
         if((CurrentTestitem+1)<=ValidNum)
         {
             int Pos=0;
             int i=0;
             for(i=0;i<2;i++)
             { if(ValidArray[i]==true)
               Pos++;
               if(Pos==(CurrentTestitem+1))
               break;
             }
             //送出 i
             if(i==0)  // 动作值
             {
             VolPickup *volpickup=new VolPickup;
             temprorarydata->ShowParaOn=true;
             connect(volpickup,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
             connect(volpickup,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

             volpickup->SetSettingLink(settinglink);
             volpickup->Setshowpara(sectmentsettmp->value);
             volpickup->setModal(true);
             volpickup->activateWindow();
             volpickup->raise();
             volpickup->show();

             }
             else if(i==1) // 动作shijian
             {

                 VolTriptime *voltriptime=new VolTriptime;
                 temprorarydata->ShowParaOn=true;
                 connect(voltriptime,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(voltriptime,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 voltriptime->SetSettingLink(settinglink);
                 voltriptime->Setshowpara(sectmentsettmp->value);
                 voltriptime->setModal(true);
                 voltriptime->activateWindow();
                 voltriptime->raise();
                 voltriptime->show();




             }

         }
     }
         break;

     case 1:

     {
         int Testitem=(int)(atof(sectmentsettmp->value.curset.Testitem.text));
         bool ValidArray[2]={Testitem&0x01,\
                             Testitem&0x02};
         int ValidNum=0; //有效的个数
         for(int i=0;i<2;i++)
         {
             if(ValidArray[i]==true)
              ValidNum++;
         }
         if((CurrentTestitem+1)<=ValidNum)
         {
             int Pos=0;
             int i=0;
             for(i=0;i<2;i++)
             { if(ValidArray[i]==true)
               Pos++;
               if(Pos==(CurrentTestitem+1))
               break;
             }
             //送出 i
             if(i==0)
             {

                 CurPickup *curpickup=new CurPickup;
                 temprorarydata->ShowParaOn=true;
                 connect(curpickup,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(curpickup,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 curpickup->SetSettingLink(settinglink);
                 curpickup->Setshowpara(sectmentsettmp->value);
                 curpickup->setModal(true);
                 curpickup->activateWindow();
                 curpickup->raise();
                 curpickup->show();



             }
             else if(i==1)
             {

                 CurTriptime *Curtriptime=new CurTriptime;
                 temprorarydata->ShowParaOn=true;
                 connect(Curtriptime,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(Curtriptime,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 Curtriptime->SetSettingLink(settinglink);
                 Curtriptime->Setshowpara(sectmentsettmp->value);
                 Curtriptime->setModal(true);
                 Curtriptime->activateWindow();
                 Curtriptime->raise();
                 Curtriptime->show();


             }
         }
     }

         break;

     case 2: //方向
       {

         PowerPara *powerpara=new PowerPara;
         temprorarydata->ShowParaOn=true;
         connect(powerpara,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
         connect(powerpara,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

         powerpara->SetSettingLink(settinglink);
         powerpara->Setshowpara(sectmentsettmp->value);
         powerpara->setModal(true);
         powerpara->activateWindow();
         powerpara->raise();
         powerpara->show();
       }

         break;

     case 3://频率

     {
         int  Testitem=(int)(atof(sectmentsettmp->value.freset.TestItem.text));
         bool ValidArray[5]={Testitem&0x01,\
                             Testitem&0x02,\
                             Testitem&0x04,\
                             Testitem&0x08,\
                             Testitem&0x10};
         int ValidNum=0; //有效的个数
         for(int i=0;i<5;i++)
         {
             if(ValidArray[i]==true)
              ValidNum++;
         }
         if((CurrentTestitem+1)<=ValidNum)
         {
             int Pos=0;
             int i=0;
             for(i=0;i<5;i++)
             { if(ValidArray[i]==true)
               Pos++;
               if(Pos==(CurrentTestitem+1))
               break;
             }
             //送出I
             switch(i)
             {
             case 0://Pick up
             { FreqPickup *freqpickup=new FreqPickup;
                 temprorarydata->ShowParaOn=true;
                 connect(freqpickup,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(freqpickup,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 freqpickup->SetSettingLink(settinglink);
                 freqpickup->Setshowpara(sectmentsettmp->value);

                 freqpickup->setModal(true);
                 freqpickup->activateWindow();
                 freqpickup->raise();
                 freqpickup->show();
             }
                 break;

             case 1: //Trip time
             {   FreqTripTime *freqtriptime=new FreqTripTime;
                 temprorarydata->ShowParaOn=true;
                 connect(freqtriptime,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(freqtriptime,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 freqtriptime->SetSettingLink(settinglink);
                 freqtriptime->Setshowpara(sectmentsettmp->value);

                 freqtriptime->setModal(true);
                 freqtriptime->activateWindow();
                 freqtriptime->raise();
                 freqtriptime->show();
             }

                 break;


             case 2: //  df/dt blocking

             {
                 DfdtBlocking *dfdtblock=new DfdtBlocking;
                 temprorarydata->ShowParaOn=true;
                 connect(dfdtblock,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(dfdtblock,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 dfdtblock->SetSettingLink(settinglink);
                 dfdtblock->Setshowpara(sectmentsettmp->value);

                 dfdtblock->setModal(true);
                 dfdtblock->activateWindow();
                 dfdtblock->raise();
                 dfdtblock->show();

             }
             break;


             case 3://  df/dt blocking time
             {
                 DfdtBlockingTime *dfdtblocktime=new DfdtBlockingTime;
                 temprorarydata->ShowParaOn=true;
                 connect(dfdtblocktime,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(dfdtblocktime,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 dfdtblocktime->SetSettingLink(settinglink);
                 dfdtblocktime->Setshowpara(sectmentsettmp->value);

                 dfdtblocktime->setModal(true);
                 dfdtblocktime->activateWindow();
                 dfdtblocktime->raise();
                 dfdtblocktime->show();

             }

                 break;


             case 4:  // voltage blocking
             {
                 VoltageBlock *voltageblock=new VoltageBlock;
                 temprorarydata->ShowParaOn=true;
                 connect(voltageblock,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(voltageblock,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 voltageblock->SetSettingLink(settinglink);
                 voltageblock->Setshowpara(sectmentsettmp->value);

                 voltageblock->setModal(true);
                 voltageblock->activateWindow();
                 voltageblock->raise();
                 voltageblock->show();

             }

                 break;
             }

         }
     }



         break;

     case 4://任意测试
         break;

     case 5:

     {
         int Testitem=(int)(atof(sectmentsettmp->value.phraseset.Testitem.text));
         bool ValidArray[2]={Testitem&0x01,\
                             Testitem&0x02};
         int ValidNum=0; //有效的个数
         for(int i=0;i<2;i++)
         {
             if(ValidArray[i]==true)
              ValidNum++;
         }
         if((CurrentTestitem+1)<=ValidNum)
         {
             int Pos=0;
             int i=0;
             for(i=0;i<2;i++)
             { if(ValidArray[i]==true)
               Pos++;
               if(Pos==(CurrentTestitem+1))
               break;
             }
           //送出 i
             if(i==0)
             {
                 PhasePickup *phasepickup=new PhasePickup;
                 temprorarydata->ShowParaOn=true;
                 connect(phasepickup,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(phasepickup,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 phasepickup->SetSettingLink(settinglink);
                 phasepickup->Setshowpara(sectmentsettmp->value);
                 phasepickup->setModal(true);
                 phasepickup->activateWindow();
                 phasepickup->raise();
                 phasepickup->show();
             }
             else
             {
                 PhaseTriptime *phasetriptime=new PhaseTriptime;
                 temprorarydata->ShowParaOn=true;
                 connect(phasetriptime,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
                 connect(phasetriptime,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

                 phasetriptime->SetSettingLink(settinglink);
                 phasetriptime->Setshowpara(sectmentsettmp->value);
                 phasetriptime->setModal(true);
                 phasetriptime->activateWindow();
                 phasetriptime->raise();
                 phasetriptime->show();


             }




         }
     }
         break;
     case 6: //谐波2016-12-13
       {

         HarmonicPara *harmonicpara=new HarmonicPara;
         temprorarydata->ShowParaOn=true;
         connect(harmonicpara,SIGNAL(sendflag()),this,SLOT(Closeshowpara()));
         connect(harmonicpara,SIGNAL(Sendpara(void *)),this,SLOT(GetNewSectset(void *)));

         harmonicpara->SetSettingLink(settinglink);
         harmonicpara->Setshowpara(sectmentsettmp->value);
         harmonicpara->setModal(true);
         harmonicpara->activateWindow();
         harmonicpara->raise();
         harmonicpara->show();
       }

         break;
     }


    }
}



}

void Templateprogram::ShowFileExpired()
{
  QMessageBox::warning(this,tr("warning"),tr("The authorized file has expired!"));
  return;
}




void Templateprogram::getthreadoutputstr(char *str)
{
    if(memcmp(str,"WARNING=",strlen("WARNING="))==0) //警告信息
    {
    char *p1=str+strlen("WARNING=");
    ui->label_3->setText(QString(p1));
    }
    else
    ui->label_2->setText(QString(str));
}





void Templateprogram::GetStarttest(int type)
{
if(type==0)
{
ui->pushButton->setEnabled(false);
ui->pushButton_2->setEnabled(false);
ui->pushButton_5->setEnabled(false);
ui->pushButton_6->setEnabled(false);
ui->pushButton_7->setEnabled(false);
ui->pushButton_8->setEnabled(false);
ui->groupBox->setEnabled(false);

}
else if(type==1)
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->groupBox->setEnabled(true);
}


}

void Templateprogram::on_pushButton_3_clicked()
{


    if(SoftStartEndflag==true)
           return;


       if(starttest==1)
       {
           //写入本地文件 让后台的模板文件停止测试

           FILE *pFile=fopen("./TemplateDir/TemplateTestState.txt","w+");
           if(pFile==NULL)
               return;

           char buffer[512]="STOPTEST";
           fwrite(buffer,sizeof(buffer), 1 , pFile );
           fclose(pFile);
       }
       else
         {
             ui->pushButton_3->setText("Stop");
             SoftStartEndflag=true;
         }

}
void Templateprogram::GetTestState(){
    if(starttest==1)
        ui->pushButton_3->setText("Stop");
    else
        ui->pushButton_3->setText("Test");

}
