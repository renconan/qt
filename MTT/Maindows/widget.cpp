
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
#include "Manual/manualtest.h"
#define RUNSTATE

#define Template_dir "./"

#include "qtranslator.h"
QTranslator *tor;
int current_language=1;  //1??墨?  2贸锟犇
const int W_ICONSIZE =90; //????????
const int H_ICONSIZE =90; //????????
const int H_ITEMSIZE =150; //渭?艒??墨????(??墨?渭?艒??墨??掳眉??????贸?掳眉??墨?卤?)
const int W_ITEMSIZE =115; //渭?艒??墨kuan??(??墨?渭?艒??墨??掳眉??????贸?掳眉??墨?卤?)

 int basicfuncfile=0;  //1|?眉2膩?艒墨???????
 int templatefunc=0;   //?锟÷????
 struct NODELINK *basicfunclink=NULL; //??卤?2膩?艒1|?眉渭??鈥溌
 struct NODELINK *templatelink=NULL;  //?锟÷2膩?艒1|?眉渭???卤?
 char SerialNumber[512];

extern bool Threadrunstate;  //??3??膿路艅??艒?艒???
 bool SystemclockCorrect=true;
QString str1="Serial No:";
QString str2="DSP:";
QString str3="Software:";
extern int sys[4];
QStringList ModelList;
extern QString serialnumbers,dspnumbers,datenumbers;
QString softwarenumber;
QString AuthorizedExpireTime;
QString SerialRead;//readfunctionfile() //??1|?眉????墨??????鈥澪??????
QString serials[100] = {};//CheckAuthrizeFile(char *templatefile)//?锟茅????墨??????鈥澪??????
int softnumber;
extern QString a1,a2,a3,a4,a5,a6;

void callBackExample(TypeOfCOM typeofcom,int Value, string notice, void* userParam)
{
        if (userParam == NULL)//?茅?眉?膿??????3???
        {
                //卤???脳贸渭?鈥渱??

                //cout << typeofcom << ":" << Value << ":" << notice << endl;


                //?墨?膩渭?鈥渱??????
                switch (typeofcom)
                {
                case TypeOfCOM_UDP_Client:
                        if (Value == TypeOfMSG_Com_RecvOK)//?奴?锟犆硘???膿?鈥溍椗峌DP路?墨艅?梅渭?1?2????锟???艅???膿ip;name
                        {
                                //192.168.1.100;1#2膩?艒?膿
                                vector<string> result;
                                mms_tokenize(result, notice, ";");
                                if (result.size() == 2)
                                {
                                        //?膿1奴??贸?艒???卤?锟2???3茅1|锟?膿?鈥溌肺?ture
                                        //bool sucdessInsert = 	g_OnlineTesientList.insert(make_pair(result.at(0), result.at(1))).second;
                                }
                        }
                        break;
                case TypeOfCOM_UDP_Server:
                {
                        if (Value == TypeOfMSG_Com_SendERR)//路锟??搂掳眉锟?茅?眉??贸?茅???贸??搂渭???1?
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
                        case	TypeOfMSG_Com_SendERR:	//路锟??搂掳眉
                                break;
                        case	TypeOfMSG_Com_SendOK://路锟?3茅1|
                                break;
                        case 		TypeOfMSG_Com_RecvERR://??渭?渭?鈥墨贸
                                break;
                        case 		TypeOfMSG_Com_RecvOK://??渭?渭??奴?锟
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
                        case	TypeOfMSG_Com_SendERR:	//路锟??搂掳眉
                                break;
                        case	TypeOfMSG_Com_SendOK://路锟?3茅1|
                                break;
                        case 		TypeOfMSG_Com_RecvERR://??渭?渭?鈥墨贸
                                break;
                        case 		TypeOfMSG_Com_RecvOK://??渭?渭??奴?锟
                        {
                                //?奴?锟艅??墨?ip:?奴?锟???
                                //鈥渱???膩渭????搂??
                                vector<string> newMsg;
                                mms_tokenize(newMsg, notice, ":");
                                if (newMsg.size() == 2)
                                {
                                        string ip = newMsg.at(0);
                                        string msg = newMsg.at(1);
                                }
                        }
                                break;
                        case	TypeOfMSG_Com_TCPServer_NewClientConnecting://贸??膩渭????搂???贸??,?艅??墨?ip:port
                        {
                                //鈥渱???膩渭????搂??
                                vector<string> newClient;
                                mms_tokenize(newClient, notice, ":");
                                if (newClient.size() == 2)
                                {
                                        string ip = newClient.at(0);
                                        int port = atoi(newClient.at(1).c_str());
                                }
                        }
                                break;
                        case	TypeOfMSG_Com_TCPServer_NewClientDisConnected://???搂?????????贸,?艅??墨?ip:port
                        {
                                //鈥渱???膩渭????搂??
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
                        case	TypeOfMSG_Com_SendERR:	//路锟??搂掳眉
                                break;
                        case	TypeOfMSG_Com_SendOK://路锟?3茅1|
                                break;
                        case 		TypeOfMSG_Com_RecvERR://??渭?渭?鈥墨贸
                                break;
                        case 		TypeOfMSG_Com_RecvOK://??渭?渭??奴?锟
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
                //?膿1奴?膿??,卤??膿 鈥溌??? CQTMAINWINDOW,???膩??墨?????
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
    m_TCPServer(TypeOfCOM_TCP_Server),
    m_SerialServer(TypeOfCOM_SERIAL_Server),
    p_currentModelWidget(NULL)
{
    ui->setupUi(this);
    init_par();
    initFromPara();
    initDesc();

     gpiofd=open("/dev/em335x_gpio", O_RDWR);
    int rc = GPIO_OutEnable(gpiofd, 0xffffffff);	//set all GPIO as output
    if(rc < 0)
    {
         printf("GPIO_OutEnable::failed %d\n", rc);
         gpiofd=-1;
    }
    //?膩???膿?艒??渭卤膿掳?????????1????渭?dateTimeEdit锟渭梅?艒渭??卤???艒??3??鈥
//    ui->dateTimeEdit->hide();
//    ui->dateTimeEdit_2->hide();

    GPIO_OutSet(gpiofd,GPIO6);
    GPIO_OutClear(gpiofd,GPIO9);//clear beep2016-12-20
    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    basicfuncfile=0;  //1|?眉2膩?艒墨???????
    templatefunc=0;
    setFixedSize(640,480);
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




     //?艒??DSP脳墨?贸?????卤??
    //ui->lineEdit->setText(datenumbers);
    m_TCPServer.InstallBackFunction(callBackExample, this);//??渭梅????掳2脳掳   tcp
    m_SerialServer.InstallBackFunction(callBackExample, this);//??渭梅????掳2脳掳 serial

    //tcp
    m_TCPServer.SetPort(6808,"");
    m_TCPServer.SetPollTime(300000);
    m_TCPServer.Start();//TCP  ????
    //serial
//    m_SerialServer.SetCom(2,115200);
//    m_SerialServer.SetPollTime(1000000);
//    m_SerialServer.Start();//serial  ????




//    QTimer *time = new QTimer();
//    time->start(1000);
//    connect(time,SIGNAL(timeout()),this,SLOT(sendDSPstate()));

connect(uartthread,SIGNAL(SIG_dspstate(int)),this,SLOT(sendDSPstate(int)));

    //uartthread->connectdsp();
    connect(uartthread,SIGNAL(SIG_DspInfoState()),this,SLOT(showDspInfo()));
    connect(this,SIGNAL(SIG_SendIndexToThread(int)),uartthread,SLOT(onRecevieIndex(int)));
    uartthread->start();

    connect(this,SIGNAL(SIG_processPara(QString)),this,SLOT(onProcessPara(QString)));
}
void Widget::sendDSPstate()
{
    QTextCodec *code =QTextCodec::codecForName("GBK");
    QString strcmp =QString(conDeviceStatus)+QString("绯荤粺杩愯姝ｅ父...");
    QByteArray ba = code->fromUnicode(strcmp);
        char *mm = ba.data();
        m_TCPServer.SendDataAll(mm, strlen(mm));

        m_SerialServer.SendDataAll(mm, strlen(mm));
}

void Widget::sendTestResult(QString strResult)
{
    QTextCodec *code =QTextCodec::codecForName("GBK");
    QString strcmp =QString(conGetResult)+strResult;
    QByteArray ba = code->fromUnicode(strcmp);
        char *mm = ba.data();
        m_TCPServer.SendDataAll(mm, strlen(mm));
        m_SerialServer.SendDataAll(mm, strlen(mm));

    strcmp = QString(conTestEnd);
    ba = code->fromUnicode(strcmp);
    mm = ba.data();
    m_TCPServer.SendDataAll(mm, strlen(mm));
    m_SerialServer.SendDataAll(mm, strlen(mm));
}
//鍒濆鍖栧弬鏁
void Widget::initFromPara()
{
    //*******************************************
    //anytest
    p_widget.insert("AnyTest",0);

    QMap<QString,float> Anydate;
    Anydate.insert("VaV",57.735);
    Anydate.insert("VaP",0);
    Anydate.insert("VaF",50);
    Anydate.insert("IaV",3);
    Anydate.insert("IaP",0);
    Anydate.insert("IaF",50);
    Anydate.insert("VaC",1);
    Anydate.insert("IaC",1);

    Anydate.insert("Mod",1);
    Anydate.insert("Var",1);

    Anydate.insert("PfT",3);
    Anydate.insert("fT",5);
    Anydate.insert("EfT",3);

    Anydate.insert("IS",0.5);
    Anydate.insert("VS",1);
    Anydate.insert("PS",1);
    Anydate.insert("FS",1);
    Anydate.insert("TS",0.5);
    Anydate.insert("NS",20);

    Anydate.insert("HT",15);

    mTestPara.insert("AnyTest",Anydate);

    //*******************************************
    //sin1
    p_widget.insert("Sin",1);
    QMap<QString,float> Sindate;
    //.........
    //鍚孉nytest
    //........
    mTestPara.insert("AnyTest",Sindate);

    //*******************************************
    //dc1
    p_widget.insert("Time",2);
    QMap<QString,float> Timedate;
    //.........
    //鍚孉nytest
    //........
    mTestPara.insert("AnyTest",Timedate);
    //*******************************************

}
void Widget::showDspInfo()
{
    ui->label->setText(str1+serialnumbers);
    ui->label_2->setText(str2+dspnumbers);
    softwarenumber ="2.1.0";
    ui->label_3->setText(str3+softwarenumber);
    ui->label_4->hide();
}
void Widget::sendDSPstate(int sate)
{
//    char buff[10]={0};
//    sprintf(buff,"@%d\n",sate);
//    perror(buff);
    QTextCodec *code =QTextCodec::codecForName("GBK");
    QString strcmp =QString(conDeviceStatus)+QString("绯荤粺杩愯姝ｅ父...");
    if(sate<10)
    {
    switch(sate)
    {
//    case 0:
//        strcmp= QString(conDeviceStatus)+QString("?渭?3艒?????3锟..");break;
    case 1:
        strcmp= QString(conDeviceStatus)+QString("鐢垫祦娓╁害鎴栧姛鐜囦繚鎶");break;
    case 2:
        strcmp= QString(conDeviceStatus)+QString("鐢垫祦杩囪浇鎴栨帴绾夸笉鍏");break;
    case 3:
        strcmp= QString(conDeviceStatus)+QString("鐢垫祦闂攣");break;
    case 4:
        strcmp= QString(conDeviceStatus)+QString("鐢靛帇娓╁害淇濇姢");break;
    case 5:
        strcmp= QString(conDeviceStatus)+QString("鐢靛帇杩囪浇");break;

    }
    QByteArray ba = code->fromUnicode(strcmp);
        char *mm = ba.data();
        m_TCPServer.SendDataAll(mm, strlen(mm));

        m_SerialServer.SendDataAll(mm, strlen(mm));
    }
    else
    {
        //conGetLEDBinValue
    }



//    QByteArray ba = code->fromUnicode(strcmp);
//        char *mm = ba.data();
//        m_TCPServer.SendDataAll(mm, strlen(mm));

//        m_SerialServer.SendDataAll(mm, strlen(mm));
}

void Widget::INcallBackExample(TypeOfCOM typeofcom,int Value, string& notice)
{perror("INcallBackExample==============");
    //ui->label->setText(QString(notice.c_str()));
    //?墨?膩渭?鈥渱??????
    switch (typeofcom)
    {
    case TypeOfCOM_UDP_Client:
            if (Value == TypeOfMSG_Com_RecvOK)//?奴?锟犆硘???膿?鈥溍椗峌DP路?墨艅?梅渭?1?2????锟???艅???膿ip;name
            {
                    //192.168.1.100;1#2膩?艒?膿
                    vector<string> result;
                    mms_tokenize(result, notice, ";");
                    if (result.size() == 2)
                    {
                            //?膿1奴??贸?艒???卤?锟2???3茅1|锟?膿?鈥溌肺?ture
                            //bool sucdessInsert = 	g_OnlineTesientList.insert(make_pair(result.at(0), result.at(1))).second;
                    }
            }
            break;
    case TypeOfCOM_UDP_Server:
    {
            if (Value == TypeOfMSG_Com_SendERR)//路锟??搂掳眉锟?茅?眉??贸?茅???贸??搂渭???1?
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
            case	TypeOfMSG_Com_SendERR:	//路锟??搂掳眉
                    break;
            case	TypeOfMSG_Com_SendOK://路锟?3茅1|
                    break;
            case 		TypeOfMSG_Com_RecvERR://??渭?渭?鈥墨贸
                    break;
            case 		TypeOfMSG_Com_RecvOK://??渭?渭??奴?锟
                    break;
            default:
                    break;
            }
    }
            break;
    case TypeOfCOM_TCP_Server:
    {perror("TypeOfCOM_TCP_Server");
        char buff[10]={0};
            sprintf(buff,"@@%d\n",Value);
            perror(buff);
            switch (Value)
            {
            case TypeOfMSG_Com_NormalMsg:
                    break;
            case	TypeOfMSG_Com_SendERR:	//路锟??搂掳眉
                    break;
            case	TypeOfMSG_Com_SendOK://路锟?3茅1|
                    break;
            case 		TypeOfMSG_Com_RecvERR://??渭?渭?鈥墨贸
                    break;
            case 		TypeOfMSG_Com_RecvOK://??渭?渭??奴?锟
            {
                emit SIG_processPara(QString(notice.c_str()));
            }
                    break;
            case	TypeOfMSG_Com_TCPServer_NewClientConnecting://贸??膩渭????搂???贸??,?艅??墨?ip:port
            {
                    //鈥渱???膩渭????搂??
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
            case	TypeOfMSG_Com_TCPServer_NewClientDisConnected://???搂?????????贸,?艅??墨?ip:port
            {
                    //鈥渱???膩渭????搂??
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
            case	TypeOfMSG_Com_SendERR:	//路锟??搂掳眉
                    break;
            case	TypeOfMSG_Com_SendOK://路锟?3茅1|
                    break;
            case 		TypeOfMSG_Com_RecvERR://??渭?渭?鈥墨贸
                    break;
            case 		TypeOfMSG_Com_RecvOK://??渭?渭??奴?锟
            {
                emit SIG_processPara(QString(notice.c_str()));
//                string msg = notice;
//                //ui->label->setText(QString(msg.c_str()));
//                if(QString(msg.c_str()).contains(conDeviceConfig))
//                {
//                    string cmp = string(conDeviceConfig)+"1";
//                    m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
//                }
//                if(QString(msg.c_str())==conGetLEDBinValue)
//                {
//                    string cmp = string(conGetLEDBinValue)+"1";
//                    m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
//                }
            }
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


void Widget::onProcessPara(QString Qnotice)
{
    string notice;
   QByteArray ba = Qnotice.toLocal8Bit();
   notice = ba.data();
                        //?奴?锟艅??墨?ip:?奴?锟???
                        //鈥渱???膩渭????搂??
                        vector<string> newMsg;
                        int TestID = -1;
                        mms_tokenize(newMsg, notice, ":");//?艒??IP??2墨??
                        if (newMsg.size() == 2)
                        {
                                string ip = newMsg.at(0);
                                string msg = newMsg.at(1);
                                //ui->label->setText(QString(msg.c_str()));
                                if(QString(msg.c_str()).contains(conDeviceConfig))
                                {
                                    string cmp = string(conDeviceConfig)+"1";
                                    m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                                    m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
                                }
                                else if(QString(msg.c_str()).contains(conGetLEDBinValue))
                                {
                                    string cmp = string(conGetLEDBinValue)+"1";
                                    m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                                    m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
                                }
                                else if(QString(msg.c_str()).contains(conTestStart))
                                {//perror("010conTestStart");
                                    int tmp = 0;
                                    if(p_currentModelWidget)
                                    {
                                        tmp = p_currentModelWidget->StartTestState();
                                    }
                                    string cmp = string(conTestStart)+(tmp?"1":"0");
                                    m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                                    m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
                                }
                                else if(QString(msg.c_str()).contains(conTestEnd))
                                {//perror("010conTestEnd");
                                    int tmp = 0;
                                    if(p_currentModelWidget)
                                    {
                                        tmp = p_currentModelWidget->StopTestState();
                                    }
                                    string cmp = string(conTestEnd)+(tmp?"1":"0");
                                    m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                                    m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
                                }
                                else if(QString(msg.c_str()).contains(conSetTestParameter))
                                {//perror("010conSetTestParameter");
                                    vector<string> paraMsg;
                                    mms_tokenize(paraMsg, msg, ";");//
                                    if(paraMsg.size()<2)
                                    {
                                        string cmp = string(conSetTestParameter)+"0";
                                        m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                                        m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
                                        return;
                                    }
                                    else
                                    {
                                            QMap<QString,float> *mp = NULL;
                                            vector<string>::iterator iter = paraMsg.begin();
                                            while(iter!=paraMsg.end())
                                            {
                                                vector<string> KeyValue;
                                                mms_tokenize(KeyValue, *iter, "=");
                                                if(KeyValue.size()==2)
                                                {
                                                    if(iter==paraMsg.begin())
                                                    {
                                                        string whichModel = KeyValue.at(1);
                                                        QMap<QString,int>::iterator iterIDfind =\
                                                                p_widget.find(QString(whichModel.c_str()));
                                                        if(iterIDfind != p_widget.end())
                                                        {
                                                            TestID = iterIDfind.value();
                                                        }
                                                        QMap<QString,QMap<QString,float> >::iterator iterfind\
                                                                =mTestPara.find(QString(whichModel.c_str()));
                                                        if(iterfind!=mTestPara.end())
                                                        {
                                                            mp = &iterfind.value();
                                                        }
                                                        else
                                                        {
                                                            string cmp = string(conSetTestParameter)+"0";
                                                            m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                                                            m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
                                                            return;
                                                        }
                                                    }
                                                    else if(mp!=NULL)
                                                    {
                                                        string key = KeyValue.at(0);
                                                        string value = KeyValue.at(1);
                                                        QMap<QString,float> ::iterator iterfind\
                                                                = mp->find(QString(key.c_str()));
                                                        if(iterfind!=mp->end())
                                                        {
                                                            iterfind.value()=atof(value.c_str());
                                                        }
                                                    }
                                                }
                                                iter++;
                                            }
                                            bool sucess = false;
                                            if(TestID>-1)
                                            sucess=OpenTestWidget(TestID);
                                            if(sucess&&mp)
                                            {
                                                p_currentModelWidget->UpDatePara(mp);
                                            }

                                            string cmp = string(conSetTestParameter)+(sucess&&mp?"1":"0");
                                        m_TCPServer.SendData(ip.c_str(), cmp.c_str(), cmp.size());
                                        m_SerialServer.SendDataAll(cmp.c_str(), cmp.size());
                                    }

                                }
                        }
}

void Widget::sysinit()
{


     ui->listWidget_2->setIconSize(QSize(W_ICONSIZE, W_ICONSIZE));
     ui->listWidget_2->setViewMode(QListView::IconMode);
       //茅???Qui->listWidget??渭?渭?艒??墨2??茅卤?????
     ui->listWidget_2->setMovement(QListView::Static);
       //茅???Qui->listWidget??渭?渭?艒??墨渭?????
     ui->listWidget_2->setSpacing(5);
     ui->listWidget_2->setFocusPolicy(Qt::NoFocus);


    //?艒????卤?渭?2膩?艒?锟茅
    struct NODELINK *next=basicfunclink->next;




        //if(memcmp(next->name,"ANYTEST",strlen("ANYTEST"))==0)
        {
            QString str = "浠绘剰娴嬭瘯";
            QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Anytest.png"), QString(str));
            pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
            pItem->setTextAlignment(Qt::AlignCenter);
            //pItem->setToolTip("<font color=blue>"+QString("Any Test")+"</font>");
            ModelList.append("Anytest");
            ui->listWidget_2->addItem(pItem);
        }
       // else if(memcmp(next->name,"CURRENTPROTECT",strlen("CURRENTPROTECT"))==0)
        {
            QString str = "浜ゆ祦缁х數鍣;
            QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/BaseCurrent.png"), QString(str));
            pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
            pItem->setTextAlignment(Qt::AlignCenter);
         //   pItem->setToolTip("BaseCurrent Relay");
            //pItem->setToolTip("<font color=blue>"+QString("BaseCurrent Relay")+"</font>");
            ModelList.append("Sin");
            ui->listWidget_2->addItem(pItem);
        }

       //else if(memcmp(next->name,"VOLTAGEPROTECT",strlen("VOLTAGEPROTECT"))==0)
         {
            QString str = "鐩存祦缁х數鍣;
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Voltage.png"), QString(str));
             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
             pItem->setTextAlignment(Qt::AlignCenter);
          //   pItem->setToolTip("Voltage Relay");
            // pItem->setToolTip("<font color=blue>"+QString("Voltage Relay")+"</font>");
             ModelList.append("DC");
             ui->listWidget_2->addItem(pItem);
         }
//       else if(memcmp(next->name,"PHRASEPROTECT",strlen("PHRASEPROTECT"))==0)      //?1??贸???卤?
//         {
//            QString str = "渭??眉??艒?";
//             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Phase.png"), QString(str));
//             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
//             pItem->setTextAlignment(Qt::AlignCenter);

//             pItem->setToolTip("<font color=blue>"+QString("Phase Test")+"</font>");

//             ui->listWidget_2->addItem(pItem);
//         }
      // else if(memcmp(next->name,"FREQPROTECT",strlen("FREQPROTECT"))==0)          //?1??贸???卤?
         {
            QString str = "浣庡懆鍑忚浇";
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
            QString str = "浣庡帇鍑忚浇";
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/DC.png"), QString(str));
             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
             pItem->setTextAlignment(Qt::AlignCenter);
             //pItem->setToolTip("<font color=blue>"+QString("DC Relay")+"</font>");
             ModelList.append("dv/dt");
             ui->listWidget_2->addItem(pItem);
         }

      // else if(memcmp(next->name,"POWERDIRPROTECT",strlen("POWERDIRPROTECT"))==0)
         {
            QString str = "鍔熺巼鏂瑰悜";
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Power.png"), QString(str));
             pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
             pItem->setTextAlignment(Qt::AlignCenter);
           //  pItem->setToolTip("Power Direction");
            // pItem->setToolTip("<font color=blue>"+QString("Directional")+"</font>");
             ModelList.append("PowerDirection");
             ui->listWidget_2->addItem(pItem);
         }
      /*
       else if(memcmp(next->name,"LOWVOLTAGEPROTECT",strlen("LOWVOLTAGEPROTECT"))==0)    //?1??贸???卤?
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
            QString str = "鍙嶆椂闄;
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
            QString str = "璺濈瀹氬€兼楠;
              QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Zone.png"), QString(str));
              pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
              pItem->setTextAlignment(Qt::AlignCenter);
             // pItem->setToolTip("<font color=blue>"+QString("Distance")+"</font>");
              ui->listWidget_2->addItem(pItem);
                ModelList.append("Zone");
         }
      // else if(memcmp(next->name,"WHOLETEST",strlen("WHOLETEST"))==0)
         {
            QString str = "鏁寸粍璇曢獙";
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
            QString str = "宸姩姣旂巼鍒跺姩";
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
            QString str = "宸姩璋愭尝鍒跺姩";
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
            QString str = "闆跺簭瀹氬€兼楠;
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
            QString str = "涓棿缁х數鍣;
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
            QString str = "绯荤粺璁剧疆";
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
            QString str = "鏂囦欢浼犺緭";
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
    QString str=LastModified.toString("yyyy:MM:dd:mm:ss"); //2????鈥卤 ?鈥卤艒?膿?????linux??????8???鈥卤
    QByteArray  ba= str.toLatin1();
    char *TimeModified=ba.data();
    CRC=Check(TimeModified);
    return CRC;
}




int Widget::CheckAuthrizeFile(char *templatefile)//?锟茅????墨???
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
    QDateTime modifiedtime=templatefileinfo.lastModified().toUTC();//?锟÷墨???渭?脳墨?贸?????卤??


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
              fclose(fp);  ////????墨??????-1???
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
              fclose(fp);  ////????墨??????-1???
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

         QDateTime timetmp= QDateTime::fromString(temp,"yyyy-M-d h:m:s");//timetmp  ????墨?????渭?脳墨?贸?????卤??

         if(abs(timetmp.secsTo(modifiedtime))>5)//modifiedtime  ?锟÷墨???渭?脳墨?贸?????卤??
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

   //卤?????????

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


bool Widget::CheckSerailNumber(char *templatefile)  //??2茅?膿?????膿路艅????
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



void Widget::readfunctionfile() //??1|?眉????墨???
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

          //?膩墨???卤?渭?2膩?艒1|?眉?墨??
          if(memcmp(buf,"ANYTEST=",strlen("ANYTEST="))==0) //?墨?膩2膩?艒
          {
              char *p1=buf+strlen("ANYTEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"ANYTEST",ID);
              ID++;
              }
          }
          else if(memcmp(buf,"CURRENTPROTECT=",strlen("CURRENTPROTECT="))==0) //渭膿?梅
          {
              char *p1=buf+strlen("CURRENTPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"CURRENTPROTECT",ID);
              ID++;
              }
          }
          else if(memcmp(buf,"VOLTAGEPROTECT=",strlen("VOLTAGEPROTECT="))==0) //渭膿?1
          {
              char *p1=buf+strlen("VOLTAGEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"VOLTAGEPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"PHRASEPROTECT=",strlen("PHRASEPROTECT="))==0) //??墨?
          {
              char *p1=buf+strlen("PHRASEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"PHRASEPROTECT",ID);
              ID++;
              }


          }

          else if(memcmp(buf,"FREQPROTECT=",strlen("FREQPROTECT="))==0) //?渭膩?
          {
              char *p1=buf+strlen("FREQPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"FREQPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"TIMEPROTECT=",strlen("TIMEPROTECT="))==0) //?卤????渭膿?梅
          {
              char *p1=buf+strlen("TIMEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"TIMEPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"POWERDIRPROTECT=",strlen("POWERDIRPROTECT="))==0) //1|膩?路???
          {
              char *p1=buf+strlen("POWERDIRPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"POWERDIRPROTECT",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"LOWVOLTAGEPROTECT=",strlen("LOWVOLTAGEPROTECT="))==0)  //渭??1??艒?
          {
              char *p1=buf+strlen("LOWVOLTAGEPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"LOWVOLTAGEPROTECT",ID);
              ID++;
              }


          }

          else if(memcmp(buf,"FANSHIXIANPROTECT=",strlen("FANSHIXIANPROTECT="))==0) //路鈥卤??
          {
              char *p1=buf+strlen("FANSHIXIANPROTECT=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"FANSHIXIANPROTECT",ID);
              ID++;
              }


          }

          else if(memcmp(buf,"DISTANCETEST=",strlen("DISTANCETEST="))==0)   //????卤锟陇
          {
              char *p1=buf+strlen("DISTANCETEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"DISTANCETEST",ID);
              ID++;
              }
          }
          else if(memcmp(buf,"WHOLETEST=",strlen("WHOLETEST="))==0)   //?奴脳茅2膩?艒
          {
              char *p1=buf+strlen("WHOLETEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"WHOLETEST",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"DIFFERTEST=",strlen("DIFFERTEST="))==0)   //2墨??卤?膩?????
          {
              char *p1=buf+strlen("DIFFERTEST=");
              if(memcmp(p1,"ON",strlen("ON"))==0)
              {
              insertnodelink(basicfunclink,"DIFFERTEST",ID);
              ID++;
              }


          }
          else if(memcmp(buf,"HARMONICTEST=",strlen("HARMONICTEST="))==0)   //2墨???32?????
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
      ////??卤?2膩?艒1|?眉???鈥
      else if(memcmp(buf,"<TEMPLATECONFIGBEGIN>",strlen("<TEMPLATECONFIGBEGIN>"))==0&&sequnence==3)
      {
            sequnence=4;
            ID=0;

      }
      else if(sequnence==4&&memcmp(buf,"<TEMPLATECONFIGEND>",strlen("<TEMPLATECONFIGEND>"))!=0)
      {
          //2膩?艒?锟÷
          char *tmp=buf;
          char buffer[512];
          int len=0;
           while((*tmp!='\0')&&(*tmp!=0x0d)&&(*tmp!=0x0a))
          {
              buffer[len]=*tmp;
              tmp++;
              len++;
          }
          buffer[len]='\0';  // ?膩???锟÷?奴 2锟????贸脳?
          //if(CheckSerailNumber(buffer)==true)  //??2茅?膿?????膿路艅????,??2茅?膿?????膿路艅????,????艒??茅?艒艒??梅?膿???艒??
          if(CheckAuthrizeFile(buffer)==true)  //??2茅?膿?????膿路艅????,????艒??茅?艒艒??梅?膿???艒??,2016-12-21
         { insertnodelink(templatelink,buffer,ID);
           ID++;
         }

      }
      else if(memcmp(buf,"<TEMPLATECONFIGEND>",strlen("<TEMPLATECONFIGEND>"))==0&&sequnence==4)
      {
         //????
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





int Widget::CheckFile(char *templatefile)//?艅?鈥??????1?卤??
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
bool Widget::OpenTestWidget(int index)
{
    if(index==14)
      {
      ExportReport *exportreport=new  ExportReport;
      exportreport->setModal(true);
      exportreport->raise();
      exportreport->show();
      exportreport->move((QApplication::desktop()->width() - exportreport->width())/2/*-100*/,(QApplication::desktop()->height() - exportreport->height())/2/*-20*/);
      return true;
      }
    else
    {


    QString id;
    QMap<QString,int>::iterator iter = p_widget.begin();
    while(iter!=p_widget.end())
    {
        if(iter.value()==index)
        {
            id = iter.key();
            break;
        }
        iter++;
    }
    if(id.isEmpty())return false;

    if(p_currentModelWidget!=NULL)
    {
        if(p_currentModelWidget->m_ModelID==id){
            return true;}
        else
        {
            p_currentModelWidget->close();
            delete p_currentModelWidget;
            p_currentModelWidget = NULL;
        }
    }


 if(p_currentModelWidget==NULL)
  {

    switch(index)
    {

    case 0://anytest
    {
         p_currentModelWidget = new Manualtest(id);
        //           strcpy(tempfile,"./Manual");
        //           uartthread->setcmd(tempfile);
        //           uartthread->start();
        connect(uartthread,SIGNAL(SIG_binarystate(int)),p_currentModelWidget,SLOT(showdspstate(int)));
        connect(uartthread,SIGNAL(SIG_dspstate(int)),p_currentModelWidget,SLOT(showdspstate(int)));
        connect(uartthread,SIGNAL(SIG_testresult(bool)),p_currentModelWidget,SLOT(showtestresult(bool)));
        connect(uartthread,SIGNAL(SIG_setVanocheck()),p_currentModelWidget,SLOT(setVanocheck()));
        connect(p_currentModelWidget,SIGNAL(SIG_Manualoutput()),uartthread,SLOT(Manualoutput()));
        connect(uartthread,SIGNAL(SIG_SendClose()),p_currentModelWidget,SLOT(CloseWidget()));
        connect(uartthread,SIGNAL(SIG_Teststate(int)),p_currentModelWidget,SLOT(GetTeststate(int)));
        connect(uartthread,SIGNAL(SIG_SendWarning(int,int)),p_currentModelWidget,SLOT(ShowWarning(int,int)));
        connect(uartthread,SIGNAL(SIG_TestButtenState()),p_currentModelWidget,SLOT(GetTestButtenstate()));
        connect(uartthread,SIGNAL(SIG_DisplayAutoChangeValue(int)),p_currentModelWidget,SLOT(DisplayAutoChangeValue(int)));
        //涓嬮潰3琛屾槸閮芥湁鐨
        connect(p_currentModelWidget,SIGNAL(SIG_closeFrom()),this,SLOT(onCloseFrom()));
        connect(p_currentModelWidget,SIGNAL(SIG_SendTestResult(QString)),this,SLOT(sendTestResult(QString)));
        emit SIG_SendIndexToThread(index);
    }
    break;

    case 1://sin1
    {



    }
        break;

    case 2://time1
    {


    }
      break;


    case 3://dizhou1
    {



    }

//        break;
//   case 4:
//    {

//        strcpy(tempfile,"./Voltage");
//        uartthread->setcmd(tempfile);
//        uartthread->start();

//    }
//    break;
//   case 5:

//    {

//        strcpy(tempfile,"./Power");
//        uartthread->setcmd(tempfile);
//        uartthread->start();

//    }
//   break;



//   case 6:
//    {

//        strcpy(tempfile,"./fanshixian");
//        uartthread->setcmd(tempfile);
//        uartthread->start();
//    }

//        break;

//   case 7:
//    {

//        strcpy(tempfile,"./zone");
//        uartthread->setcmd(tempfile);
//        uartthread->start();

//    }
//        break;

//   case 8:
//    {

//        strcpy(tempfile,"./Systemtest");
//        uartthread->setcmd(tempfile);
//        uartthread->start();
//    }
//        break;

//    case 9:
//    {

//        strcpy(tempfile,"./Differ");
//        uartthread->setcmd(tempfile);
//        uartthread->start();

//    }
//        break;

//   case 10:
//    {

//        strcpy(tempfile,"./Harmonic");
//        uartthread->setcmd(tempfile);
//        uartthread->start();
//    }
//        break;

//    case 11:
//    {

//        strcpy(tempfile,"./Zero");
//        uartthread->setcmd(tempfile);
//        uartthread->start();
//    }
//        break;

//    case 12:
//    {

//        strcpy(tempfile,"./Middle");
//        uartthread->setcmd(tempfile);
//        uartthread->start();
//    }
//     break;

//    case 13:
//    {

////            if(Threadrunstate==true)
////               return;

//        strcpy(tempfile,"./Sysconfig");
//        uartthread->setcmd(tempfile);
//        uartthread->start();
//    }
//    break;



    }

    p_currentModelWidget->show();
    p_currentModelWidget->raise();
    p_currentModelWidget->activateWindow();


    }
 return true;
    }
}
void Widget::on_listWidget_2_itemClicked(QListWidgetItem *item)
{

//    if(Threadrunstate==true)
//       return;


       QListWidget *currentListwidget=item->listWidget();
        int index=currentListwidget->currentRow();
        //char tempfile[512];
        OpenTestWidget(index);
/*
        char buf[512];
        memset(buf,'\0',512);
        if(geditnamefromid(basicfunclink,index,buf)==false)
            return;

        char tempfile[512];
        if(memcmp(buf,"ANYTEST",strlen("ANYTEST"))==0) //?墨?膩2膩?艒
        {

           strcpy(tempfile,"./Manual");
           uartthread->setcmd(tempfile);
           uartthread->start();
        }
        else if(memcmp(buf,"CURRENTPROTECT",strlen("CURRENTPROTECT"))==0) //渭膿?梅
        {

             strcpy(tempfile,"./BaseCurrent");
             uartthread->setcmd(tempfile);
             uartthread->start();

        }
        else if(memcmp(buf,"VOLTAGEPROTECT",strlen("VOLTAGEPROTECT"))==0) //渭膿?1
        {

            strcpy(tempfile,"./Voltage");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
        else if(memcmp(buf,"PHRASEPROTECT",strlen("PHRASEPROTECT"))==0) //??墨?
        {

            strcpy(tempfile,"./Phase");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }

        else if(memcmp(buf,"FREQPROTECT",strlen("FREQPROTECT"))==0) //?渭膩?
        {

            strcpy(tempfile,"./dizhou");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"TIMEPROTECT",strlen("TIMEPROTECT"))==0) //?卤????渭膿?梅
        {

            strcpy(tempfile,"./Time");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"POWERDIRPROTECT",strlen("POWERDIRPROTECT"))==0) //1|膩?路???
        {

            strcpy(tempfile,"./Power");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }



        else if(memcmp(buf,"FANSHIXIANPROTECT",strlen("FANSHIXIANPROTECT"))==0) //路鈥卤??
        {

            strcpy(tempfile,"./fanshixian");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }

        else if(memcmp(buf,"DISTANCETEST",strlen("DISTANCETEST"))==0)   //????卤锟陇
        {

            strcpy(tempfile,"./zone");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"WHOLETEST",strlen("WHOLETEST"))==0)   //?奴脳茅2膩?艒
        {

            strcpy(tempfile,"./Systemtest");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
        else if(memcmp(buf,"DIFFERTEST",strlen("DIFFERTEST"))==0)   //2墨??卤?膩?????
        {

            strcpy(tempfile,"./Differ");
            uartthread->setcmd(tempfile);
            uartthread->start();

        }
        else if(memcmp(buf,"HARMONICTEST",strlen("HARMONICTEST"))==0)   //2墨???32?????
        {

            strcpy(tempfile,"./Harmonic");
            uartthread->setcmd(tempfile);
            uartthread->start();
        }
*/

}
void Widget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(Threadrunstate==true)
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
    if(Threadrunstate==true)
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
void Widget::onCloseFrom()
{
    if(p_currentModelWidget!=NULL)
    {
        p_currentModelWidget->close();
        delete p_currentModelWidget;
        p_currentModelWidget = NULL;
    }
}
