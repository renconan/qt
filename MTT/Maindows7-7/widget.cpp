#include "widget.h"
#include "ui_widget.h"
#include <stdlib.h>
#include <stdio.h>
#include "Nodelinkmanage.h"
#include "Defpam.h"
#include "Config.h"
#include <QTextCodec>
#include <QDebug>
#include <QProcess>
#include <QTableWidgetItem>
#include <QStyledItemDelegate>
#include <QListWidgetItem>
#include <QWSServer>
#include <QMouseDriverFactory>
#include <QFile>
#include <QFileInfo>
#include <QMoveEvent>
#include <Gpio.h>
#include <em335x_drivers.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopWidget>

#define RUNSTATE

#define Template_dir "./"

#include "qtranslator.h"
QTranslator *tor;
int current_language=1;  //1中文  2英文
const int W_ICONSIZE =90; //图片宽度
const int H_ICONSIZE =90; //图片高度
const int H_ITEMSIZE =150; //单元项高度(因为单元项既包括图片又包括文本)
const int W_ITEMSIZE =115; //单元项kuan度(因为单元项既包括图片又包括文本)

 int basicfuncfile=0;  //功能测试文件个数
 int templatefunc=0;   //模板个数
 struct NODELINK *basicfunclink=NULL; //基本测试功能的链表
 struct NODELINK *templatelink=NULL;  //模板测试功能的聊表
 char SerialNumber[512];

 bool threadrunstate=false;  //线程是否正在运行
 bool SystemclockCorrect=true;
QString str1="Serial No:";
QString str2="DSP:";
QString str3="Software:";
int sys[4];
QStringList ModelList;
extern QString serialnumbers,dspnumbers,datenumbers;
QString softwarenumber;
QString AuthorizedExpireTime;
QString SerialRead;//readfunctionfile() //读功能配置文件读取的序列号
QString serials[100] = {};//CheckAuthrizeFile(char *templatefile)//校验授权文件读取的序列号
int softnumber;
extern QString a1,a2,a3,a4,a5,a6;

void callBackExample(TypeOfCOM typeofcom,int Value, string notice, void* userParam)
{
        if (userParam == NULL)//可能是控制台程序
        {
                //本例子的处理

                //cout << typeofcom << ":" << Value << ":" << notice << endl;


                //额外的处理示例
                switch (typeofcom)
                {
                case TypeOfCOM_UDP_Client:
                        if (Value == TypeOfMSG_Com_RecvOK)//消息应该是来自UDP服务器的广播信息,其格式是ip;name
                        {
                                //192.168.1.100;1#测试仪
                                vector<string> result;
                                mms_tokenize(result, notice, ";");
                                if (result.size() == 2)
                                {
                                        //如果没有在列表，插入成功，那么返回ture
                                        //bool sucdessInsert = 	g_OnlineTesientList.insert(make_pair(result.at(0), result.at(1))).second;
                                }
                        }
                        break;
                case TypeOfCOM_UDP_Server:
                {
                        if (Value == TypeOfMSG_Com_SendERR)//发送失败，可能没有设置有效的网关
                        {

                        }
                }
                        break;
                case TypeOfCOM_TCP_Client:
                {
                        switch (Value)
                        {
                        case TypeOfMSG_Com_NormalMsg:
                                break;
                        case	TypeOfMSG_Com_SendERR:	//发送失败
                                break;
                        case	TypeOfMSG_Com_SendOK://发送成功
                                break;
                        case 		TypeOfMSG_Com_RecvERR://收到的错误
                                break;
                        case 		TypeOfMSG_Com_RecvOK://收到的消息
                                break;
                        default:
                                break;
                        }
                }
                        break;
                case TypeOfCOM_TCP_Server:
                {
                        switch (Value)
                        {
                        case TypeOfMSG_Com_NormalMsg:
                                break;
                        case	TypeOfMSG_Com_SendERR:	//发送失败
                                break;
                        case	TypeOfMSG_Com_SendOK://发送成功
                                break;
                        case 		TypeOfMSG_Com_RecvERR://收到的错误
                                break;
                        case 		TypeOfMSG_Com_RecvOK://收到的消息
                        {
                                //消息格式为ip:消息内容
                                //处理新的客户端
                                vector<string> newMsg;
                                mms_tokenize(newMsg, notice, ":");
                                if (newMsg.size() == 2)
                                {
                                        string ip = newMsg.at(0);
                                        string msg = newMsg.at(1);
                                }
                        }
                                break;
                        case	TypeOfMSG_Com_TCPServer_NewClientConnecting://有新的客户端加入,格式为ip:port
                        {
                                //处理新的客户端
                                vector<string> newClient;
                                mms_tokenize(newClient, notice, ":");
                                if (newClient.size() == 2)
                                {
                                        string ip = newClient.at(0);
                                        int port = atoi(newClient.at(1).c_str());
                                }
                        }
                                break;
                        case	TypeOfMSG_Com_TCPServer_NewClientDisConnected://客户端断开连接,格式为ip:port
                        {
                                //处理新的客户端
                                vector<string> disConnectedClient;
                                mms_tokenize(disConnectedClient, notice, ":");
                                if (disConnectedClient.size() == 2)
                                {
                                        string ip = disConnectedClient.at(0);
                                        int port = atoi(disConnectedClient.at(1).c_str());
                                }
                        }
                        break;
                        default:
                                break;
                        }
                }
                        break;
                case TypeOfCOM_SERIAL_Client:
                case TypeOfCOM_SERIAL_Server:
                {
                        switch (Value)
                        {
                        case TypeOfMSG_Com_NormalMsg:
                                break;
                        case	TypeOfMSG_Com_SendERR:	//发送失败
                                break;
                        case	TypeOfMSG_Com_SendOK://发送成功
                                break;
                        case 		TypeOfMSG_Com_RecvERR://收到的错误
                                break;
                        case 		TypeOfMSG_Com_RecvOK://收到的消息
                                break;
                        default:
                                break;
                        }
                }
                        break;
                default:
                        break;
                }
        }
        else
        {
            printf("%d:%s\r\n",Value,notice.c_str());
                //如果是类,比如 窗体类 CQTMAINWINDOW,一下仅为示例
                Widget *pQTmainWindos = (Widget*) userParam;
                //if(pQTmainWindos->is)
                pQTmainWindos->INcallBackExample(typeofcom,Value,notice);

        }
        if ("exit" == notice)
        {
                //mainIsRunning = false;
        }
}

 class NoFocusDelegate :public QStyledItemDelegate
 {
     void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const;
 };

 void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
 {
      QStyleOptionViewItem itemOption(option);
      if (itemOption.state & QStyle::State_HasFocus)
      {
         itemOption.state = itemOption.state^QStyle::State_HasFocus;
      }
      QStyledItemDelegate::paint(painter, itemOption, index);
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

 void Widget::init_par()
   {

//     FILE *fp;
//     if((fp=fopen("sysset.txt","a+"))==NULL)
//     return;
//     fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
//            &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language,&softnumber);
//     fclose(fp);
//    int high = softnumber/100;
//    int mid = (softnumber -high*100)/10;
//    int low = softnumber%10;
//    softwarenumber = QString::number(high)+'.'+QString::number(mid)+'.'+QString::number(low);
//    softwarenumber = "2.4.0";
   }


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_TCPServer(TypeOfCOM_TCP_Server)
{
    ui->setupUi(this);    
    init_par();
    printf("1\n");
   m_TCPServer.InstallBackFunction(callBackExample, this);//回调函数安装
    printf("2\n");
     gpiofd=open("/dev/em335x_gpio", O_RDWR);
    int rc = GPIO_OutEnable(gpiofd, 0xffffffff);	//set all GPIO as output
    if(rc < 0)
    {
         printf("GPIO_OutEnable::failed %d\n", rc);
         gpiofd=-1;
    }
    //下面是显示当前日期和截止日期的dateTimeEdit，调试的时候显示出来
//    ui->dateTimeEdit->hide();
//    ui->dateTimeEdit_2->hide();

    GPIO_OutSet(gpiofd,GPIO6);
    GPIO_OutClear(gpiofd,GPIO9);//clear beep2016-12-20
    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    basicfuncfile=0;  //功能测试文件个数
    templatefunc=0;
    setFixedSize(800,480);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    setAutoFillBackground(true);
    showFullScreen();

    this->setStyleSheet("background-image: url(:/pic/backgound.JPG);");


    ui->listWidget->setStyleSheet("background-image: url(:/pic/backgound.png);");
    ui->listWidget->setItemDelegate(new NoFocusDelegate());
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->listWidget_2->setStyleSheet("background-image: url(:/pic/backgound.png);");
    ui->listWidget_2->setItemDelegate(new NoFocusDelegate());
    ui->listWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);







    QFont font("wenquanyi",16);
    ui->listWidget->setFont(font);
    ui->listWidget_2->setFont(font);


    fileWatcher = new QFileSystemWatcher(this);
    fileWatcher->addPath("/dev/input/");
  //  connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(slotFileChanged(QString)));


    bMouse = false;


    basicfunclink=(struct NODELINK *)malloc(sizeof(struct NODELINK));
    basicfunclink->next=NULL;
    templatelink=(struct NODELINK *)malloc(sizeof(struct NODELINK));
    templatelink->next=NULL;
    //readfunctionfile();  // mainconfig.cfg
    uartthread=new thread1;
    //connect(uartthread,SIGNAL(SIG_showmainface()),this,SLOT(sysinit()));
    sysinit();
    uartthread->connectdsp();
    //ui->label->setText(str1+serialnumbers);
    ui->label_2->setText(str2+dspnumbers);
    softwarenumber ="2.1.0";
    ui->label_3->setText(str3+softwarenumber);
    ui->label_4->hide();


     //显示DSP最后修改时间
    //ui->lineEdit->setText(datenumbers);

    printf("2.5\n");
    printf("Port===%d\r\n",m_TCPServer.GetPort());
    m_TCPServer.SetPort(6808,"");
    m_TCPServer.SetPollTime(300000);
    m_TCPServer.Start();//TCP  开始
    printf("3\n");
    QTimer *time = new QTimer();
    time->start(1000);
    connect(time,SIGNAL(timeout()),this,SLOT(sendDSPstate()));
    printf("4\n");
}

void Widget::sendDSPstate()
{ //printf("5\n");
    QTextCodec *code =QTextCodec::codecForName("GBK");

    QString strcmp = QString(conDeviceStatus)+QString("正常");
    QByteArray ba = code->fromUnicode(strcmp);
        char *mm = ba.data();
        m_TCPServer.SendDataAll(mm, strlen(mm));
}

void Widget::INcallBackExample(TypeOfCOM typeofcom,int Value, string notice)
{
    ui->label->setText(QString(notice.c_str()));
    //额外的处理示例
    switch (typeofcom)
    {
    case TypeOfCOM_UDP_Client:
            if (Value == TypeOfMSG_Com_RecvOK)//消息应该是来自UDP服务器的广播信息,其格式是ip;name
            {
                    //192.168.1.100;1#测试仪
                    vector<string> result;
                    mms_tokenize(result, notice, ";");
                    if (result.size() == 2)
                    {
                            //如果没有在列表，插入成功，那么返回ture
                            //bool sucdessInsert = 	g_OnlineTesientList.insert(make_pair(result.at(0), result.at(1))).second;
                    }
            }
            break;
    case TypeOfCOM_UDP_Server:
    {
            if (Value == TypeOfMSG_Com_SendERR)//发送失败，可能没有设置有效的网关
            {

            }
    }
            break;
    case TypeOfCOM_TCP_Client:
    {
            switch (Value)
            {
            case TypeOfMSG_Com_NormalMsg:
                    break;
            case	TypeOfMSG_Com_SendERR:	//发送失败
                    break;
            case	TypeOfMSG_Com_SendOK://发送成功
                    break;
            case 		TypeOfMSG_Com_RecvERR://收到的错误
                    break;
            case 		TypeOfMSG_Com_RecvOK://收到的消息
                    break;
            default:
                    break;
            }
    }
            break;
    case TypeOfCOM_TCP_Server:
    {
            switch (Value)
            {
            case TypeOfMSG_Com_NormalMsg:
                    break;
            case	TypeOfMSG_Com_SendERR:	//发送失败
                    break;
            case	TypeOfMSG_Com_SendOK://发送成功
                    break;
            case 		TypeOfMSG_Com_RecvERR://收到的错误
                    break;
            case 		TypeOfMSG_Com_RecvOK://收到的消息
            {
                    //消息格式为ip:消息内容
                    //处理新的客户端
                    vector<string> newMsg;
                    mms_tokenize(newMsg, notice, ":");
                    if (newMsg.size() == 2)
                    {
                            string ip = newMsg.at(0);
                            string msg = newMsg.at(1);
                            //ui->label->setText(QString(msg.c_str()));
                            if(QString(msg.c_str()).contains(conDeviceConfig))
                            {
                                string cmp = string(conDeviceConfig)+"1";
                                m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                            }
                            if(QString(msg.c_str())==conGetLEDBinValue)
                            {
                                string cmp = string(conGetLEDBinValue)+"1";
                                m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                            }
                    }
            }
                    break;
            case	TypeOfMSG_Com_TCPServer_NewClientConnecting://有新的客户端加入,格式为ip:port
            {
                    //处理新的客户端
                    vector<string> newClient;
                    mms_tokenize(newClient, notice, ":");
                    if (newClient.size() == 2)
                    {
                            string ip = newClient.at(0);
                            int port = atoi(newClient.at(1).c_str());
                             m_TCPServer.SendData(ip.c_str(), conConnectPC, strlen(conConnectPC));
                    }
            }
                    break;
            case	TypeOfMSG_Com_TCPServer_NewClientDisConnected://客户端断开连接,格式为ip:port
            {
                    //处理新的客户端
                    vector<string> disConnectedClient;
                    mms_tokenize(disConnectedClient, notice, ":");
                    if (disConnectedClient.size() == 2)
                    {
                            string ip = disConnectedClient.at(0);
                            int port = atoi(disConnectedClient.at(1).c_str());
                    }
            }
            break;
            default:
                    break;
            }
    }
            break;
    case TypeOfCOM_SERIAL_Client:
    case TypeOfCOM_SERIAL_Server:
    {
            switch (Value)
            {
            case TypeOfMSG_Com_NormalMsg:
                    break;
            case	TypeOfMSG_Com_SendERR:	//发送失败
                    break;
            case	TypeOfMSG_Com_SendOK://发送成功
                    break;
            case 		TypeOfMSG_Com_RecvERR://收到的错误
                    break;
            case 		TypeOfMSG_Com_RecvOK://收到的消息
                    break;
            default:
                    break;
            }
    }
            break;
    default:
            break;
    }

}




void Widget::sysinit()
{


     ui->listWidget_2->setIconSize(QSize(W_ICONSIZE, W_ICONSIZE));
     ui->listWidget_2->setViewMode(QListView::IconMode);
       //设置Qui->listWidget中的单元项不可被拖动
     ui->listWidget_2->setMovement(QListView::Static);
       //设置Qui->listWidget中的单元项的间距
     ui->listWidget_2->setSpacing(5);
     ui->listWidget_2->setFocusPolicy(Qt::NoFocus);


    //显示基本的测试模块
    struct NODELINK *next=basicfunclink->next;




        //if(memcmp(next->name,"ANYTEST",strlen("ANYTEST"))==0)
        {
            QString str = "任意测试";
            QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Anytest.png"), QString(str));
            pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
            pItem->setTextAlignment(Qt::AlignCenter);
            //pItem->setToolTip("<font color=blue>"+QString("Any Test")+"</font>");
            ModelList.append("Anytest");
            ui->listWidget_2->addItem(pItem);
        }
       // else if(memcmp(next->name,"CURRENTPROTECT",strlen("CURRENTPROTECT"))==0)
        {
            QString str = "交流继电器";
            QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Current.png"), QString(str));
            pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
            pItem->setTextAlignment(Qt::AlignCenter);
         //   pItem->setToolTip("Current Relay");
            //pItem->setToolTip("<font color=blue>"+QString("Current Relay")+"</font>");
            ModelList.append("Sin");
            ui->listWidget_2->addItem(pItem);
        }

       //else if(memcmp(next->name,"VOLTAGEPROTECT",strlen("VOLTAGEPROTECT"))==0)
         {
            QString str = "直流继电器";
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Voltage.png"), QString(str));
             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
             pItem->setTextAlignment(Qt::AlignCenter);
          //   pItem->setToolTip("Voltage Relay");
            // pItem->setToolTip("<font color=blue>"+QString("Voltage Relay")+"</font>");
             ModelList.append("DC");
             ui->listWidget_2->addItem(pItem);
         }
//       else if(memcmp(next->name,"PHRASEPROTECT",strlen("PHRASEPROTECT"))==0)      //还没有图标
//         {
//            QString str = "低周减载";
//             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Phase.png"), QString(str));
//             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
//             pItem->setTextAlignment(Qt::AlignCenter);

//             pItem->setToolTip("<font color=blue>"+QString("Phase Test")+"</font>");

//             ui->listWidget_2->addItem(pItem);
//         }
      // else if(memcmp(next->name,"FREQPROTECT",strlen("FREQPROTECT"))==0)          //还没有图标
         {
            QString str = "低周减载";
              QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Freq.png"), QString(str));
              pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
              pItem->setTextAlignment(Qt::AlignCenter);
            //  pItem->setToolTip("Frequency Test");
              //pItem->setToolTip("<font color=blue>"+QString("Frequency Test")+"</font>");
              ModelList.append("df/dt");
              ui->listWidget_2->addItem(pItem);
         }
       //else if(memcmp(next->name,"TIMEPROTECT",strlen("TIMEPROTECT"))==0)
         {
            QString str = "低压减载";
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/DC.png"), QString(str));
             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
             pItem->setTextAlignment(Qt::AlignCenter);
             //pItem->setToolTip("<font color=blue>"+QString("DC Relay")+"</font>");
             ModelList.append("dv/dt");
             ui->listWidget_2->addItem(pItem);
         }

      // else if(memcmp(next->name,"POWERDIRPROTECT",strlen("POWERDIRPROTECT"))==0)
         {
            QString str = "功率方向";
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Power.png"), QString(str));
             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
             pItem->setTextAlignment(Qt::AlignCenter);
           //  pItem->setToolTip("Power Direction");
            // pItem->setToolTip("<font color=blue>"+QString("Directional")+"</font>");
             ModelList.append("PowerDirection");
             ui->listWidget_2->addItem(pItem);
         }
      /*
       else if(memcmp(next->name,"LOWVOLTAGEPROTECT",strlen("LOWVOLTAGEPROTECT"))==0)    //还没有图标
         {
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/LowVoltage.png"), QString(QLatin1String("dvdt")));
             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
             pItem->setTextAlignment(Qt::AlignCenter);
         //    pItem->setToolTip("dvdt");
             pItem->setToolTip("<font color=blue>"+QString("dvdt")+"</font>");

             ui->listWidget_2->addItem(pItem);
         }

       */

      // else if(memcmp(next->name,"FANSHIXIANPROTECT",strlen("FANSHIXIANPROTECT"))==0)
         {
            QString str = "反时限";
              QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Inversetime.png"), QString(str));
              pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
              pItem->setTextAlignment(Qt::AlignCenter);
            //  pItem->setToolTip("Inverse Time");
             // pItem->setToolTip("<font color=blue>"+QString("Inverse Time")+"</font>");
              ModelList.append("Inversetime");
              ui->listWidget_2->addItem(pItem);
         }

       //else if(memcmp(next->name,"DISTANCETEST",strlen("DISTANCETEST"))==0)
         {
            QString str = "距离定值";
              QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Zone.png"), QString(str));
              pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
              pItem->setTextAlignment(Qt::AlignCenter);
             // pItem->setToolTip("<font color=blue>"+QString("Distance")+"</font>");
              ui->listWidget_2->addItem(pItem);
                ModelList.append("Zone");
         }
      // else if(memcmp(next->name,"WHOLETEST",strlen("WHOLETEST"))==0)
         {
            QString str = "整组测试";
            QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Systemtest.png"), QString(str));
            pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
            pItem->setTextAlignment(Qt::AlignCenter);
          //  pItem->setToolTip("System Test");
           // pItem->setToolTip("<font color=blue>"+QString("Auto-Reclosing")+"</font>");
            ModelList.append("Systemtest");
            ui->listWidget_2->addItem(pItem);
         }

      // else if(memcmp(next->name,"DIFFERTEST",strlen("DIFFERTEST"))==0)
         {
            QString str = "差动比率制动";
            QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Differ.png"), QString(str));
            pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
            pItem->setTextAlignment(Qt::AlignCenter);
          //  pItem->setToolTip("Ir/Id Differ");
           // pItem->setToolTip("<font color=blue>"+QString("Differential")+"</font>");
            ModelList.append("Differ");
            ui->listWidget_2->addItem(pItem);
         }
      // else if(memcmp(next->name,"HARMONICTEST",strlen("HARMONICTEST"))==0) //
         {
            QString str = "差动谐波制动";
              QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Harmonic.png"), QString(str));
              pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
              pItem->setTextAlignment(Qt::AlignCenter);
            //  pItem->setToolTip("Ir/Id Differ");
             // pItem->setToolTip("<font color=blue>"+QString("Harmonic")+"</font>");
              ModelList.append("Harmonic");
              ui->listWidget_2->addItem(pItem);
         }
      //  else if(memcmp(next->name,"ZEROTEST",strlen("ZEROTEST"))==0) //
          {
            QString str = "零序定值检验";
               QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Zero.png"), QString(str));
               pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
               pItem->setTextAlignment(Qt::AlignCenter);
             //  pItem->setToolTip("Ir/Id Differ");
               //pItem->setToolTip("<font color=blue>"+QString("Harmonic")+"</font>");
               ModelList.append("Zero");
               ui->listWidget_2->addItem(pItem);
          }
       // else if(memcmp(next->name,"MIDDLETEST",strlen("MIDDLETEST"))==0) //
          {
            QString str = "中间继电器";
               QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Middle.png"), QString(str));
               pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
               pItem->setTextAlignment(Qt::AlignCenter);
             //  pItem->setToolTip("Ir/Id Differ");
               //pItem->setToolTip("<font color=blue>"+QString("Harmonic")+"</font>");
               ModelList.append("Middle");
               ui->listWidget_2->addItem(pItem);
          }
       // else if(memcmp(next->name,"SYSCONFIGTEST",strlen("SYSCONFIGTEST"))==0) //
          {
            QString str = "系统设置";
               QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Sysconfig.png"), QString(str));
               pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
               pItem->setTextAlignment(Qt::AlignCenter);
             //  pItem->setToolTip("Ir/Id Differ");
               //pItem->setToolTip("<font color=blue>"+QString("Harmonic")+"</font>");
               ModelList.append("Sysconfig");
               ui->listWidget_2->addItem(pItem);
          }
       // else if(memcmp(next->name,"REPORTTEST",strlen("REPORTTEST"))==0) //
          {
            QString str = "文件传输";
               QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Report.png"), QString(str));
               pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
               pItem->setTextAlignment(Qt::AlignCenter);
             //  pItem->setToolTip("Ir/Id Differ");
               //pItem->setToolTip("<font color=blue>"+QString("Harmonic")+"</font>");
               ModelList.append("Export");
               ui->listWidget_2->addItem(pItem);
          }


    ui->listWidget_2->show();
    ui->listWidget->hide();
}


void Widget::moveEvent(QMoveEvent *event)
{

    QPoint oldpos=event->oldPos();
    this->move(oldpos);

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

unsigned   int Widget::TemplateCrcNum(char *Filename)
{

    unsigned  long CRC=0;
    QFileInfo fileinfo(Filename);
    QDateTime LastModified=fileinfo.lastModified();
    QString str=LastModified.toString("yyyy:MM:dd:mm:ss"); //不要小时 小时在嵌入式linux始终晚8个小时
    QByteArray  ba= str.toLatin1();
    char *TimeModified=ba.data();
    CRC=Check(TimeModified);
    return CRC;
}




int Widget::CheckAuthrizeFile(char *templatefile)//校验授权文件
{
    char TemplateFile[512];
    char AuthorizeFile[512];
    sprintf(TemplateFile,"./AuthorizeDir/%s.mtp",templatefile);
    sprintf(AuthorizeFile,"./AuthorizeDir/%s.dat",templatefile);

    unsigned long Checksum=0;
    if(!QFile::exists(TemplateFile))
        return -1;
    if(!QFile::exists(AuthorizeFile))
        return -2;


    QFileInfo templatefileinfo(TemplateFile);
    unsigned long filesize=templatefileinfo.size();
    QDateTime modifiedtime=templatefileinfo.lastModified().toUTC();//模板文件的最后修改时间


    FILE *fp=NULL;
    fp=fopen(AuthorizeFile,"r");
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
    int i=0;
     int j=0;
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


        if(/*sequnence==1&&*/memcmp(buf,"FILENAME=",strlen("FILENAME="))==0)
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

          char cmpchar[512];
          sprintf(cmpchar,"%s.dat",templatefile);
          if(memcmp(cmpchar,temp,strlen(temp))!=0)
          {
            fclose(fp);
            return -5;
          }
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
          QDateTime  currentdatetime=QDateTime::currentDateTime();
          QDateTime Authorizetime=QDateTime::fromString(temp,"yyyy.M.d.h.m.s");
          if(currentdatetime.toUTC()<Authorizetime)
          {
              fclose(fp);  ////授权文件已经过期
              return -6;
          }

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
          QDateTime  currentdatetime=QDateTime::currentDateTime();
          QDateTime Expiretime=QDateTime::fromString(temp,"yyyy.M.d.h.m.s");
          if(currentdatetime.toUTC()>Expiretime)
          {
              fclose(fp);  ////授权文件已经过期
              return -6;
          }
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

         /*
         int year,month,day,hour,minute,second;
         int year_A,month_A,day_A,hour_A,minute_A,second_A;
         sscanf(temp,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&minute,&second);
         year_A=modifiedtime.date().year();
         month_A=modifiedtime.date().month();
         day_A=modifiedtime.date().day();
         hour_A=modifiedtime.time().hour();
         minute_A=modifiedtime.time().minute();
         second_A=modifiedtime.time().second();
         bool timevalid=year==year_A&&month==month_A&&day==day_A&&\
                       hour==hour_A&&minute&&minute_A&&second==second_A;

                       */

         QDateTime timetmp= QDateTime::fromString(temp,"yyyy-M-d h:m:s");//timetmp  授权文件中的最后修改时间

         if(abs(timetmp.secsTo(modifiedtime))>5)//modifiedtime  模板文件的最后修改时间
         //if(timevalid==false)
         {
           fclose(fp);
           return -7;
         }
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
        unsigned long  fileinfo_filesize=atol(temp);
        if(filesize!=fileinfo_filesize)
        {
            fclose(fp);
            return -8;
        }
        sequnence=6;
      }
//        else if(sequnence==6&&memcmp(buf,"CHECKCODE=",strlen("CHECKCODE="))==0)
//        {
//         char *p1=buf+strlen("CHECKCODE=");

//         unsigned long CRC=atol(p1);
//         if(CRC!=Checksum)
//         {
//             fclose(fp);
//             return -9;
//         }
//         sequnence=7;
//        }
        else if(sequnence==6&&memcmp(buf,"SERIALNUM=",strlen("SERIALNUM="))==0)
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
            serials[i]=QString(temp);
            i++;
        /*  if(memcmp(SerialNumber,temp,strlen(SerialNumber))!=0)
          {
            fclose(fp);
            return -4;
          }
     sequnence=1;*/
        }
    }

   //比较序列号

    while(j<100&&(!serials[j].isEmpty()))
    {
     //QString num = serials[j];
     if(SerialRead==serials[j])
     {
         fclose(fp);
         return 1;
     }
     j++;
    }
    if(serials[j].isEmpty())
    {
      fclose(fp);
      return -4;
    }
    fclose(fp);
    return 1;
}


bool Widget::CheckSerailNumber(char *templatefile)  //检查是序列是否匹配
{
    char TemplateFile[512];
    char AuthorizeFile[512];
    sprintf(TemplateFile,"./AuthorizeDir/%s.mtp",templatefile);
    sprintf(AuthorizeFile,"./AuthorizeDir/%s.dat",templatefile);

    unsigned long Checksum=0;
    if(!QFile::exists(TemplateFile))
        return false;
    if(!QFile::exists(AuthorizeFile))
        return false;

    FILE *fp=NULL;
    fp=fopen(AuthorizeFile,"r");
    if(fp==NULL)
    {  printf("fopen error\n");
       return false;
    }
    char key=0x13;
    char miwen[1024];
    char buf[1024];
    int sequnence=0;
    char *p2=NULL;
    char miwen_2[1024];
    char length=0;
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
          if(memcmp(SerialNumber,temp,strlen(SerialNumber))!=0)
          {
            fclose(fp);
            return false;
          }

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
          char cmpchar[512];
          sprintf(cmpchar,"%s.dat",templatefile);
          if(memcmp(cmpchar,temp,strlen(temp))!=0)
          {
            fclose(fp);
            return false;
          }
           sequnence=2;
        }
    }
    fclose(fp);
    return true;
}



void Widget::readfunctionfile() //读功能配置文件
 {
  FILE *fp=NULL;
  int sequnence=0;
  int ID=0;
  fp=fopen(MAINCONFIGFILE,"r");
  if(fp==NULL)
  {  printf("fopen mainconfig.cfg error\n");
     return;
  }

  char key=0x2a;
  char miwen[1024];
  char buf[1024];

  char *p2=NULL;
  char miwen_2[1024];
  char length=0;
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




      if(memcmp(buf,"<MAININTERFACECONFIGBEGIN>",strlen("<MAININTERFACECONFIGBEGIN>"))==0&&sequnence==0)
      {
         sequnence=1;

      }
      else if(memcmp(buf,"<BASICTESTFUNCBEGIN>",strlen("<BASICTESTFUNCBEGIN>"))!=0&&sequnence==1)
      {
         if(memcmp(buf,"SERIALNUMBER=",strlen("SERIALNUMBER="))==0)
         {
             char *p1=buf+strlen("SERIALNUMBER=");
             int len=0;
             while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
             {
                 SerialNumber[len]=*p1;
                 p1++;
                 len++;
             }
            SerialNumber[len]='\0';
            SerialRead=QString(SerialNumber);
         }

      }

      else if(memcmp(buf,"<BASICTESTFUNCBEGIN>",strlen("<BASICTESTFUNCBEGIN>"))==0&&sequnence==1)
      {
       sequnence=2;

      }
      else if(memcmp(buf,"<BASICTESTFUNCEND>",strlen("<BASICTESTFUNCEND>"))!=0&&sequnence==2)
      {

          //解析基本的测试功能项目
          if(memcmp(buf,"ANYTEST=",strlen("ANYTEST="))==0) //任意测试
          {
              char *p1=buf+strlen("ANYTEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"ANYTEST",ID);
              ID++;
              }
          }
          else if(memcmp(buf,"CURRENTPROTECT=",strlen("CURRENTPROTECT="))==0) //电流
          {
              char *p1=buf+strlen("CURRENTPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"CURRENTPROTECT",ID);
              ID++;
              }
          }
          else if(memcmp(buf,"VOLTAGEPROTECT=",strlen("VOLTAGEPROTECT="))==0) //电压
          {
              char *p1=buf+strlen("VOLTAGEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"VOLTAGEPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"PHRASEPROTECT=",strlen("PHRASEPROTECT="))==0) //相位
          {
              char *p1=buf+strlen("PHRASEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"PHRASEPROTECT",ID);
              ID++;
              }


          }

          else if(memcmp(buf,"FREQPROTECT=",strlen("FREQPROTECT="))==0) //频率
          {
              char *p1=buf+strlen("FREQPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"FREQPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"TIMEPROTECT=",strlen("TIMEPROTECT="))==0) //时间继电器
          {
              char *p1=buf+strlen("TIMEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"TIMEPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"POWERDIRPROTECT=",strlen("POWERDIRPROTECT="))==0) //功率方向
          {
              char *p1=buf+strlen("POWERDIRPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"POWERDIRPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"LOWVOLTAGEPROTECT=",strlen("LOWVOLTAGEPROTECT="))==0)  //低压减载
          {
              char *p1=buf+strlen("LOWVOLTAGEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"LOWVOLTAGEPROTECT",ID);
              ID++;
              }


          }

          else if(memcmp(buf,"FANSHIXIANPROTECT=",strlen("FANSHIXIANPROTECT="))==0) //反时限
          {
              char *p1=buf+strlen("FANSHIXIANPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"FANSHIXIANPROTECT",ID);
              ID++;
              }


          }

          else if(memcmp(buf,"DISTANCETEST=",strlen("DISTANCETEST="))==0)   //距离保护
          {
              char *p1=buf+strlen("DISTANCETEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"DISTANCETEST",ID);
              ID++;
              }
          }
          else if(memcmp(buf,"WHOLETEST=",strlen("WHOLETEST="))==0)   //整组测试
          {
              char *p1=buf+strlen("WHOLETEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"WHOLETEST",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"DIFFERTEST=",strlen("DIFFERTEST="))==0)   //差动比率制动
          {
              char *p1=buf+strlen("DIFFERTEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"DIFFERTEST",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"HARMONICTEST=",strlen("HARMONICTEST="))==0)   //差动谐波制动
          {
              char *p1=buf+strlen("HARMONICTEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"HARMONICTEST",ID);
              ID++;
              }
          }


      }
      else if(memcmp(buf,"<BASICTESTFUNCEND>",strlen("<BASICTESTFUNCEND>"))==0&&sequnence==2)
      {
         sequnence=3;

      }
      ////基本测试功能读取
      else if(memcmp(buf,"<TEMPLATECONFIGBEGIN>",strlen("<TEMPLATECONFIGBEGIN>"))==0&&sequnence==3)
      {
            sequnence=4;
            ID=0;

      }
      else if(sequnence==4&&memcmp(buf,"<TEMPLATECONFIGEND>",strlen("<TEMPLATECONFIGEND>"))!=0)
      {
          //测试模板
          char *tmp=buf;
          char buffer[512];
          int len=0;
           while((*tmp!='\0')&&(*tmp!=0x0d)&&(*tmp!=0x0a))
          {
              buffer[len]=*tmp;
              tmp++;
              len++;
          }
          buffer[len]='\0';  // 这个模板名 并不含后缀
          //if(CheckSerailNumber(buffer)==true)  //检查是序列是否匹配,检查是序列是否匹配,匹配则可以在主界面显示
          if(CheckAuthrizeFile(buffer)==true)  //检查是序列是否匹配,匹配则可以在主界面显示,2016-12-21
         { insertnodelink(templatelink,buffer,ID);
           ID++;
         }

      }
      else if(memcmp(buf,"<TEMPLATECONFIGEND>",strlen("<TEMPLATECONFIGEND>"))==0&&sequnence==4)
      {
         //结束
          sequnence=5;

      }
      else if(memcmp(buf,"<MAININTERFACECONFIGEND>",strlen("<MAININTERFACECONFIGEND>"))==0&&sequnence==5)
      {
          sequnence=6;


      }
      memset(miwen,0,1024);
  }


  fclose(fp);
 }









void Widget::slotFileChanged(const QString& path)
{
     if(path=="/dev/input/")
     {
         bool mouse1=QFile::exists("/dev/input/mouse1");
         bool mouse2=QFile::exists("/dev/input/mouse2");
         if(mouse1==true&&mouse2==false)
         {
              QWSServer::setMouseHandler(QMouseDriverFactory::create("IntelliMouse","/dev/input/mouse1"));
              bMouse = true;
         }
         else if(mouse1==false&&mouse2==true)
         {
             QWSServer::setMouseHandler(QMouseDriverFactory::create("IntelliMouse","/dev/input/mouse2"));
             bMouse = true;

         }
         else if(mouse1==true&&mouse2==true)
         {
             QWSServer::setMouseHandler(QMouseDriverFactory::create("IntelliMouse","/dev/input/mouse2"));
             bMouse = true;
         }
         else bMouse = false;
     }
}

Widget::~Widget()
{
    m_TCPServer.Stop();
    sleep(1);
    delete ui;
}





int Widget::CheckFile(char *templatefile)//获取授权截止时间
{
    char TemplateFile[512];
    char AuthorizeFile[512];
    sprintf(TemplateFile,"./AuthorizeDir/%s.mtp",templatefile);
    sprintf(AuthorizeFile,"./AuthorizeDir/%s.dat",templatefile);

    if(!QFile::exists(TemplateFile))
        return -1;
    if(!QFile::exists(AuthorizeFile))
        return -2;


    unsigned long Checksum=0;
    FILE *fp=NULL;
    fp=fopen(AuthorizeFile,"r");
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

    int i=0;

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


        if(memcmp(buf,"FILENAME=",strlen("FILENAME="))==0)
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
           //AuthorizedFilename=QString(temp);
           //sequnence=2;
        }
        else if(/*sequnence==2&&*/memcmp(buf,"AUTHORIZETIME=",strlen("AUTHORIZETIME="))==0)
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

          //AuthorizedTime=QString(temp);

//          sequnence=3;
        }

        else if(/*sequnence==3&&*/memcmp(buf,"EXPIRETIME=",strlen("EXPIRETIME="))==0)
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
          AuthorizedExpireTime=QString(temp);
//          sequnence=4;


        }



    }
    fclose(fp);
    return 1;
}

void Widget::on_listWidget_2_itemClicked(QListWidgetItem *item)
{

    if(threadrunstate==true)
       return;


       QListWidget *currentListwidget=item->listWidget();
        int index=currentListwidget->currentRow();
        char tempfile[512];
        switch(index)
        {

        case 0:
        {

           strcpy(tempfile,"./Manual");
           uartthread->setcmd(tempfile);
           uartthread->start();
        }
        break;

        case 1:
        {

             strcpy(tempfile,"./Sin");
             uartthread->setcmd(tempfile);
             uartthread->start();

        }
            break;

        case 2:
        {

            strcpy(tempfile,"./Time");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
          break;


        case 3:
        {

            strcpy(tempfile,"./dizhou");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }

            break;
       case 4:
        {

            strcpy(tempfile,"./Voltage");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        break;
       case 5:

        {

            strcpy(tempfile,"./Power");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
       break;



       case 6:
        {

            strcpy(tempfile,"./fanshixian");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }

            break;

       case 7:
        {

            strcpy(tempfile,"./zone");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
            break;

       case 8:
        {

            strcpy(tempfile,"./Systemtest");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
            break;

        case 9:
        {

            strcpy(tempfile,"./Differ");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
            break;

       case 10:
        {

            strcpy(tempfile,"./Harmonic");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
            break;

        case 11:
        {

            strcpy(tempfile,"./Zero");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
            break;

        case 12:
        {

            strcpy(tempfile,"./Middle");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
         break;

        case 13:
        {

            if(threadrunstate==true)
               return;

            strcpy(tempfile,"./Sysconfig");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
        break;
      case 14:
        {

        if(threadrunstate==true)
               return;
        ExportReport *exportreport=new  ExportReport;
        exportreport->setModal(true);
        exportreport->raise();
        exportreport->show();
        exportreport->move((QApplication::desktop()->width() - exportreport->width())/2-100,(QApplication::desktop()->height() - exportreport->height())/2-20);
        }
            break;





        }
/*
        char buf[512];
        memset(buf,'\0',512);
        if(geditnamefromid(basicfunclink,index,buf)==false)
            return;

        char tempfile[512];
        if(memcmp(buf,"ANYTEST",strlen("ANYTEST"))==0) //任意测试
        {

           strcpy(tempfile,"./Manual");
           uartthread->setcmd(tempfile);
           uartthread->start();
        }
        else if(memcmp(buf,"CURRENTPROTECT",strlen("CURRENTPROTECT"))==0) //电流
        {

             strcpy(tempfile,"./Current");
             uartthread->setcmd(tempfile);
             uartthread->start();

        }
        else if(memcmp(buf,"VOLTAGEPROTECT",strlen("VOLTAGEPROTECT"))==0) //电压
        {

            strcpy(tempfile,"./Voltage");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
        else if(memcmp(buf,"PHRASEPROTECT",strlen("PHRASEPROTECT"))==0) //相位
        {

            strcpy(tempfile,"./Phase");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }

        else if(memcmp(buf,"FREQPROTECT",strlen("FREQPROTECT"))==0) //频率
        {

            strcpy(tempfile,"./dizhou");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"TIMEPROTECT",strlen("TIMEPROTECT"))==0) //时间继电器
        {

            strcpy(tempfile,"./Time");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"POWERDIRPROTECT",strlen("POWERDIRPROTECT"))==0) //功率方向
        {

            strcpy(tempfile,"./Power");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }



        else if(memcmp(buf,"FANSHIXIANPROTECT",strlen("FANSHIXIANPROTECT"))==0) //反时限
        {

            strcpy(tempfile,"./fanshixian");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }

        else if(memcmp(buf,"DISTANCETEST",strlen("DISTANCETEST"))==0)   //距离保护
        {

            strcpy(tempfile,"./zone");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"WHOLETEST",strlen("WHOLETEST"))==0)   //整组测试
        {

            strcpy(tempfile,"./Systemtest");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
        else if(memcmp(buf,"DIFFERTEST",strlen("DIFFERTEST"))==0)   //差动比率制动
        {

            strcpy(tempfile,"./Differ");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"HARMONICTEST",strlen("HARMONICTEST"))==0)   //差动谐波制动
        {

            strcpy(tempfile,"./Harmonic");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
*/

}
void Widget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(threadrunstate==true)
       return;



    QListWidget *currentListwidget=item->listWidget();
    int index=currentListwidget->currentRow();
    int currentindex=index;
    char temp[1024];
    if(geditnamefromid(templatelink,currentindex,temp)==false)
    return;

    CheckFile(temp);
    QDateTime  currentdatetime=QDateTime::currentDateTime();
    QDateTime Expiretime=QDateTime::fromString(AuthorizedExpireTime,"yyyy.M.d.h.m.s");



    int secs = currentdatetime.toUTC().secsTo(Expiretime);
    int DAY= secs/(60*60*24);
    //int HOUR=(secs%(60*60*24))/(60*60);
    //int MIN=(secs%(60*60))/60;
    //int SEC=secs%(60);
    QString TIME = QString("Remaining Time:")+QString::number(DAY)+"Days";
    if(Expiretime>currentdatetime.toUTC())
    {
    if(DAY<10&&DAY>=0)
        ui->label_4->setText(("<font color=red>"+TIME+"</font>"));
    else if(DAY>=10&&DAY<20)
        ui->label_4->setText(("<font color=blue>"+TIME+"</font>"));
    else if(DAY>=20&&DAY<=30)
        ui->label_4->setText(("<font color=green>"+TIME+"</font>"));
    else if(DAY>30)
        ui->label_4->setText((" "));
    }
    else
    {
        QString strs= "The authorized file has expired!";
        ui->label_4->setText(("<font color=red>"+strs+"</font>"));
    }
}

void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(threadrunstate==true)
       return;



    QListWidget *currentListwidget=item->listWidget();
    int index=currentListwidget->currentRow();
    int currentindex=index;
    char cmdstr[1024];
    char temp[1024];
    if(geditnamefromid(templatelink,currentindex,temp)==false)
    return;

    switch(CheckAuthrizeFile(temp))
    {
      case -1:
        item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The template file does not exist!"),QMessageBox::Ok);
        return;
        break;
      case -2:
        item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The authorized file does not exist!"),QMessageBox::Ok);
        return;
        break;
      case -3:
        item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The authorized file can not be opened!"),QMessageBox::Ok);
        return;
        break;
     case -4:
        item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The authorized file can not be used in this test instrument!"),QMessageBox::Ok);
      return;
      break;
    case -5:
        item->setBackgroundColor(Qt::gray);
     QMessageBox::warning(this,tr("warning"),tr("The authorized file can not be used in this test template file!"),QMessageBox::Ok);
     return;
     break;
    case -6:
          item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The authorized file has expired!"),QMessageBox::Ok);
        return;
        break;
    case -7:
        item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The original template  file is modified!"),QMessageBox::Ok);
        return;
        break;
    case -8:
        item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The original template  file is modified!"),QMessageBox::Ok);
        return;
        break;
    case -9:
        item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The authorized file is damaged!"),QMessageBox::Ok);
        return;
        break;
    }

    strcat(temp,".mtp");
    sprintf(cmdstr,"./Templateprogram %s",temp);
    uartthread->setcmd(cmdstr);
    uartthread->start();
}
QString Widget::GetSerialNumber()
{
    QFile filetime("/etc/tmp/SerialNum");
    if(!filetime.open(QIODevice::ReadOnly | QIODevice::Text ))
    {
        qDebug() << "Open failed.";
    }
    QTextStream readnumber(&filetime);
    QString line;
    while( !readnumber.atEnd())
    {
        line = readnumber.readLine();
        //qDebug() << line;
    }
    //int number = line.toInt();
    filetime.flush();
    filetime.close();
    return line;
}
