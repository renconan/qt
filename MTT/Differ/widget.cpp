#include "widget.h"
#include "ui_widget.h"
#include <QTableWidgetItem>
#include <math.h>
#include <unistd.h>
#include "qtranslator.h"
QTranslator *tor;
int current_language=2;  //1中文  2英文

TEMPRORARYDATA *temprorarydata;
float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

unsigned char result[70],receive_data[70];
double out_time,change_timedata;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
int   input_par=0,HelpFlag=0;
int   INPAR=0,seq_count,ResultPage,ResultFlag,ResultCount=1,DisResult,ResultEndPage,CurrentPage;
int   testcount=0,time_relay_testcount=0,auto_end=0;
int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest=0,low,row,page;
int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC,EndTest,ResultEndCount;
float LogicResolution,SystemFrec;
float ChangeValue,timeout,TripTime,TripTimeSum[63],IdInitValue;
int exit_par,save_f;
int sys[4];
float  V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;


float   faultduration=3;
float  SystemVolt=57.735;

int ConnectType=1;
int Testobject=1;
float prefaulttime=3;
float postfaulttime=3;
float Preparetime=3;

float PhaseCorrectMode=1; //△侧校正
float PhaseCorrectStep=1;

float ChangeAngel=0;  //变化的角度

float PhaseCorrectStepTime=0.2; //步长时间

float IdSearchStep=0.2;
float IdSearchStepTime=3;

float IrSeachStart=0.2;
float IrSeachEnd=2;
float IrSeachStep=0.2;
float CurrrentIrSearch=0.2;

float Kp1=1;
float Kp2=0.5;
float Kp3=0.6;
float IhStdValue=2;
float ImSdtValue=3;
float IlStdValue=4;


float IaValue,IaPhase,IaStep;
float IbValue,IbPhase,IbStep;
float IcValue,IcPhase,IcStep;



float Postfaulttime=3;
int   dcouttype=1;


VOLCHANGERINFO Volchanger;


float StepTime=0.2;
int CTHdir=1;
int CTMdir=1;
int CTLdir=1;

bool Threadrunstate=true;

bool SoftStartEndflag=false;

QVector<QPointF> TestResult;
 //int   ResultEndCount,ResultPage,ResultFlag,ResultCount,DisResult,ResultEndPage,CurrentPage;


/************************************

高中侧故障，Y-Y-Δ1 接线与 Y-Y-Δ11 接线的单相差动程序可以一样，
外部接线不同。
高-低侧故障，Y-Y-Δ1 接线与 Y-Y-Δ11 接线单相故障程序一样，外
部接线不同：

归算到Δ侧时，高中则故障 Y-Y-Δ5 的单相差动算法与 Y-Y-Δ11 和 Y-Y-Δ1
非常相似，只是补偿电流相差 180°。

归算到Δ侧时，高低则故障 Y-Y-Δ5 的单相差动算法 与 Y-Y-Δ1 接线与 Y-Y-Δ11相近
对 Ir=0 这点，可选最小参与电流, Iu=0, Iw = -(IA/√3)/Kbl, IA幅值按较小步长增加，直到差动动作。
    过程中保持 Iw = -(IA/√3)/Kbl, 以保证 C 相不动。
搜索曲线 Ir 步长取 0.1Ie
IA(测试仪 IA)=0.1*Inh*√3=0.152A∠0°, Iu(测试仪 IB)=0.1*Inl=0.165A∠0°,
Iw(测试仪 IC)=-(IA/√3)/Kbl = -1.089*IA, 增加 Iu(测试仪 IB)的相角直到差动动作。
此时差动动作电流为：
Id = (2*IA/√3)*sin(φ/2) = 0.175*sin(φ/2), 其中φ为 Iu的角度改变量。
可描出 Id=0.175A*sin(φ/2), Ir=0.1Ie=0.0875A 这点。



****************************************/

void Widget::init_par()
  {

    FILE *fp;
    if((fp=fopen("sysset.txt","a+"))==NULL)
    return;
    //fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&Postfaulttime,&dcouttype);
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
           &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language);
    fclose(fp);
    if(SystemFrec==0)
    {
    V1DC=1;
    V2DC=1;
    V3DC=1;

    I1DC=1;
    I2DC=1;
    I3DC=1;
    }
    else
    {
        V1DC=0;
        V2DC=0;
        V3DC=0;

        I1DC=0;
        I2DC=0;
        I3DC=0;
    }
    Volchanger.Side1.RateDCapacity=40;
    Volchanger.Side1.RatedVol=110;
    Volchanger.Side1.CTRatio=100;

    Volchanger.Side2.RateDCapacity=40;
    Volchanger.Side2.RatedVol=38;
    Volchanger.Side2.CTRatio=600;

    Volchanger.Side3.RateDCapacity=40;
    Volchanger.Side3.RatedVol=10.5;
    Volchanger.Side3.CTRatio=1000;

    Volchanger.Side1.Kp=1;
    Volchanger.Side2.Kp=Volchanger.Side2.RatedVol*Volchanger.Side2.CTRatio/\
                        (Volchanger.Side1.RatedVol*Volchanger.Side1.CTRatio);
    Volchanger.Side3.Kp=Volchanger.Side3.RatedVol*Volchanger.Side3.CTRatio/\
                        (Volchanger.Side1.RatedVol*Volchanger.Side1.CTRatio);

    Volchanger.Side1.RateCur=Volchanger.Side1.RateDCapacity*1000/\
                 (1.732*Volchanger.Side1.RatedVol*Volchanger.Side1.CTRatio);
    Volchanger.Side2.RateCur=Volchanger.Side2.RateDCapacity*1000/\
                 (1.732*Volchanger.Side2.RatedVol*Volchanger.Side2.CTRatio);
    Volchanger.Side3.RateCur=Volchanger.Side3.RateDCapacity*1000/\
                 (1.732*Volchanger.Side3.RatedVol*Volchanger.Side3.CTRatio);

    IhStdValue=Volchanger.Side1.RateCur;
    ImSdtValue=Volchanger.Side2.RateCur;
    IlStdValue=Volchanger.Side3.RateCur;
    Kp1=Volchanger.Side1.Kp;
    Kp2=Volchanger.Side2.Kp;
    Kp3=Volchanger.Side3.Kp;

    MAX_V_VALUEDATA_DC=176;
    MIN_V_VALUEDATA_DC=-176;
    MAX_V_VALUEDATA_AC=125;
    MIN_V_VALUEDATA_AC=0;
    MAX_I_VALUEDATA_DC=70;
    MIN_I_VALUEDATA_DC=-70;
    MAX_I_VALUEDATA_AC=50;
    MIN_I_VALUEDATA_AC=0;
  }


void Widget::sysinit()
{
ui->comboBox->setCurrentIndex(ConnectType-1);
ui->comboBox_2->setCurrentIndex(PhaseCorrectMode-1);
ui->comboBox_3->setCurrentIndex(Testobject-1);

ui->lineEdit_2->setText(QString::number(Kp1));
ui->lineEdit_2->setEnabled(false);
ui->lineEdit_3->setText(QString::number(Kp2,'g',6));
ui->lineEdit_4->setText(QString::number(Kp3,'g',6));

ui->lineEdit_5->setText(QString::number(IrSeachStart));
ui->lineEdit_6->setText(QString::number(IrSeachEnd));
ui->lineEdit_7->setText(QString::number(IrSeachStep));


if(CTHdir==1)
    ui->radioButton->setChecked(true);
else
    ui->radioButton_2->setChecked(true);

if(CTMdir==1)
    ui->radioButton_3->setChecked(true);
else
    ui->radioButton_4->setChecked(true);

if(CTLdir==1)
    ui->radioButton_5->setChecked(true);
else
    ui->radioButton_6->setChecked(true);












}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init_par();
    /* 加载语言文件 默认中文 2016-9-9*/
        tor = new QTranslator();
        if (current_language == 1) {
            tor->load(":/trans/zh.qm");
        }
        else if (current_language == 2) {
            tor->load(":/trans/en.qm");
        }
        qApp->installTranslator(tor);
        updateUI();

    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    setFixedSize(800,480);

    temprorarydata=new TEMPRORARYDATA;
    memset(temprorarydata,0,sizeof(TEMPRORARYDATA));
    showFullScreen();

    ui->label_18->hide();
    ui->label_19->hide();
    ui->label_24->hide();
    ui->label_25->hide();
    ui->label_26->hide();
    ui->label_27->hide();
    ui->label_28->hide();
    ui->label_29->hide();

    QPixmap img(":/picture/hei.png");
    ui->label_18->setPixmap(img);
    ui->label_19->setPixmap(img);
    ui->label_24->setPixmap(img);
    ui->label_25->setPixmap(img);

    //init_par();
    sysinit();
    //ui->label->setText("<font color=blue>"+QString("差动比率制动(2.2.1)")+"</font>");

    currenteditpos=0;
    ui->tabWidget->setCurrentIndex(0);


    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_5->installEventFilter(this);
    ui->lineEdit_6->installEventFilter(this);
    ui->lineEdit_7->installEventFilter(this);







    ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);


    Curve=new Plotter(ui->tab_2);
    ui->horizontalLayout->addWidget(Curve);
    //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    //ui->pushButton->setFocusPolicy(Qt::NoFocus);

    //ui->pushButton_5->setFocusPolicy(Qt::NoFocus);









    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(10);

    int wide=ui->tableWidget->width()/2;
    for(int i=0;i<2;i++)
    {
     ui->tableWidget->setColumnWidth(i,wide-5);
    }
    QStringList strlist;
    strlist<<tr("Ir(A)")<<tr("Id(A)");
    ui->tableWidget->setHorizontalHeaderLabels(strlist);


    for(int i=0;i<10;i++)
    for(int j=0;j<2;j++)
    {
      ui->tableWidget->setItem(i,j,new QTableWidgetItem);
    }

    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_testresult(int)),this,SLOT(showtestresult(int)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),this,SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_dischange(float)),this,SLOT(ShowChangeValue(float)));
    connect(uartthread,SIGNAL(SIG_SendClose()),this,SLOT(CloseWidget()));
    connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));
    connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));

    uartthread->start();

}
void Widget::updateUI()
{
    ui->label->setText("<font color=blue>"+QString(tr("差动比率制动(2.2.1)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
    if(current_language==1)
        ui->pushButton->setText(tr("平衡系数计算"));
    else
        ui->pushButton->setText("Primary\nparameters");
        ui->pushButton_2->setText(tr("帮助"));
        ui->pushButton_3->setText(tr("保存"));
        ui->pushButton_4->setText(tr("退出"));
        ui->pushButton_5->setText(tr("测试"));


        ui->comboBox_3->addItems(QStringList()<<tr("高对中")<<tr("高对低"));

        ui->radioButton->setText(tr("两侧流入变压器"));
        ui->radioButton_2->setText(tr("一侧流入，一侧流出"));
        ui->radioButton_3->setText(tr("两侧流入变压器"));
        ui->radioButton_4->setText(tr("一侧流入，一侧流出"));
        ui->radioButton_5->setText(tr("两侧流入变压器"));
        ui->radioButton_6->setText(tr("一侧流入，一侧流出"));

        ui->groupBox->setTitle(tr("制动/动作方程"));
        ui->groupBox_2->setTitle(tr("平衡系数"));
        ui->groupBox_3->setTitle(tr("变压器接线"));
        ui->groupBox_4->setTitle(tr("Ir搜索范围"));
        ui->groupBox_5->setTitle(tr("CT 方向"));
        ui->groupBox_6->setTitle(tr("相位变化"));

        ui->label_3->setText(tr("接线方式"));
        ui->label_4->setText(tr("相位校正方式"));
        ui->label_14->setText(tr("测试对象"));
        ui->label_7->setText(tr("高压侧:Kb1="));
        ui->label_8->setText(tr("中压侧:Kb2="));
        ui->label_9->setText(tr("低压侧:Kb3="));
        ui->label_10->setText(tr("开始值"));
        ui->label_11->setText(tr("终止值"));
        ui->label_12->setText(tr("步长"));
        ui->label_20->setText(tr("高压侧"));
        ui->label_21->setText(tr("中压侧"));
        ui->label_22->setText(tr("低压侧"));

        ui->tabWidget->setTabText(0,tr("设置"));
        ui->tabWidget->setTabText(1,tr("计算结果"));
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if(starttest==1)
       return QWidget::eventFilter(obj,event);

    if(event->type()==QEvent::MouseButtonPress)
    {

        return QWidget::eventFilter(obj,event);

    }
    else if(event->type()==QEvent::MouseButtonDblClick)
   {


        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
        if(mouseevent->button()==Qt::RightButton)
        {
            return true;
        }

       else if(obj==ui->lineEdit_2)
        {    temprorarydata->keypadopenflag=true;
             currentedit=ui->lineEdit_2;
             currenteditpos=2;
        }
        else if(obj==ui->lineEdit_3)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_3;
              currenteditpos=3;
         }
        else if(obj==ui->lineEdit_4)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_4;
              currenteditpos=4;
         }

        else if(obj==ui->lineEdit_5)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_5;
              currenteditpos=5;
         }
        else if(obj==ui->lineEdit_6)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_6;
              currenteditpos=6;
         }
        else if(obj==ui->lineEdit_7)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_7;
              currenteditpos=7;
         }
        pad=new keypads;
        pad->setModal(true);
        connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
        connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
        pad->show();
        pad->raise();
        pad->activateWindow();
        return true;
   }
    else if(event->type()==QEvent::FocusOut)
    {
        if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               Kp1= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(Kp1,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                Kp2= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(Kp2,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                Kp3= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(Kp3,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                IrSeachStart= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(IrSeachStart,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                IrSeachEnd= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(IrSeachEnd,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                IrSeachStep= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(IrSeachStep,'g',6));
         }
        return QWidget::eventFilter(obj,event);
    }
    else
    return QWidget::eventFilter(obj,event);

}

void Widget::GetTeststate(int state)
{
    //2016-9-13  键盘输入，界面测试按钮，
    //设置软件“测试”按钮获取焦点，激活失去焦点时间，给最后的输入幅值
    if(state==1)
    ui->pushButton_5->setFocus();
}

void Widget::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;


        switch(currenteditpos)
        {
        case 2:
            Kp1=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(Kp1,'g',6));
            break;
        case 3:
            Kp2=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(Kp2,'g',6));
            break;
        case 4:
            Kp3=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(Kp3,'g',6));
            break;
        case 5:
            IrSeachStart=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(IrSeachStart,'g',6));
            break;
        case 6:
            IrSeachEnd=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(IrSeachEnd,'g',6));
            break;
        case 7:
            IrSeachStep=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(IrSeachStep,'g',6));
            break;


        }



}

void Widget::clearkeypadflag()
{
temprorarydata->keypadopenflag=false;
}


void Widget::vi_parameter_enter(int position)
{

    switch(position)
    {
    case 2:

        break;

    case 3:
        if(Kp2>999.9)
          Kp2=999.9;
        else if(Kp2<0)
         Kp2=0.0001;
        break;

    case 4:
        if(Kp3>999.9)
          Kp3=999.9;
        else if(Kp3<0)
         Kp3=0.0001;
        break;

    case 5:
        if(IrSeachStart>999.9)
          IrSeachStart=999.9;
        else if(IrSeachStart<=0)
         IrSeachStart=IrSeachStep;
        break;

    case 6:
        if(IrSeachEnd>999.9)
          IrSeachEnd=999.9;
        else if(IrSeachEnd<=0)
         IrSeachEnd=IrSeachStep;
        break;

    case 7:
        if(IrSeachStep>999.9)
          IrSeachStep=999.9;
        else if(IrSeachStep<=0)
         IrSeachStep=0.001;
        break;



    }


}


void Widget::showtestresult(int num)
{

    if(TestResult.size()==0)//ceshijieguomeiyou
    {
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setRowCount(10);

        int wide=ui->tableWidget->width()/2;
        for(int i=0;i<2;i++)
        {
         ui->tableWidget->setColumnWidth(i,wide-5);
        }
        QStringList strlist;
        strlist<<tr("Ir(A)")<<tr("Id(A)");
        ui->tableWidget->setHorizontalHeaderLabels(strlist);


        for(int i=0;i<10;i++)
        for(int j=0;j<2;j++)
        {
          ui->tableWidget->setItem(i,j,new QTableWidgetItem);
        }
    }
    else
    {
        ui->tableWidget->clear();
        int rowcount=TestResult.size();
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setRowCount(rowcount);
        int wide=ui->tableWidget->width()/2;
        for(int i=0;i<2;i++)
        {
         ui->tableWidget->setColumnWidth(i,wide-5);
        }
        QStringList strlist;
        strlist<<tr("Ir(A)")<<tr("Id(A)");
        ui->tableWidget->setHorizontalHeaderLabels(strlist);


        for(int i=0;i<rowcount;i++)
        for(int j=0;j<2;j++)
        {
         QTableWidgetItem  *item=new QTableWidgetItem;
         ui->tableWidget->setItem(i,j,item);
         if(j==0)
            item->setText(QString::number(TestResult.at(i).x(),'g',5));
         else
            item->setText(QString::number(TestResult.at(i).y(),'g',5));
        }
        ui->tableWidget->scrollToBottom();
    }

  ShowExcitCurve();






}

void Widget::showdspstate(int state)
{
    switch(state)
        {
        case 0: ui->label_13->setText(tr("运行正常")); break;
        case 1: ui->label_13->setText(tr("IA 过载")); break;
        case 2: ui->label_13->setText(tr("IABC 温度保护")); break;
        case 3: ui->label_13->setText(tr("IC 间歇")); break;
        case 4: ui->label_13->setText(tr("IB 过载")); break;
        case 5: ui->label_13->setText(tr("IC 闭锁")); break;
        case 6: ui->label_13->setText(tr("IA 间歇")); break;
        case 7: ui->label_13->setText(tr("IC 过载")); break;
        case 8: ui->label_13->setText(tr("IABC 电源保护")); break;
        case 9: ui->label_13->setText(tr("IB 间歇")); break;
        case 10: ui->label_13->setText(tr("VA 过载")); break;

        case 12: ui->label_13->setText(tr("VC 间歇")); break;
        case 13: ui->label_13->setText(tr("VB 过载")); break;
        case 14: ui->label_13->setText(tr("VABC 温度保护")); break;
        case 15: ui->label_13->setText(tr("VA 间歇")); break;
        case 16: ui->label_13->setText(tr("VC 过载")); break;
        case 17: ui->label_13->setText(tr("VABC 电源保护")); break;
        case 18: ui->label_13->setText(tr("Vz 过载")); break;
        case 19: ui->label_13->setText(tr("直流过载")); break;
        case 20: ui->label_13->setText(tr("IABC 电源保护")); break;
        case 21: ui->label_13->setText(tr("VC 间歇")); break;


    }

    QPixmap img(":/picture/hei.png");
    QPixmap pic(":/picture/hong.png");
    switch(state)
    {
    case 100:
        ui->label_18->setPixmap(img);
        ui->label_19->setPixmap(img);
        ui->label_24->setPixmap(img);
        ui->label_25->setPixmap(img);
        break;
    case 101: ui->label_18->setPixmap(pic); break;
    case 102: ui->label_19->setPixmap(pic); break;
    case 103: ui->label_24->setPixmap(pic); break;
    case 104: ui->label_25->setPixmap(pic); break;
    case 111: ui->label_18->setPixmap(img); break;
    case 112: ui->label_19->setPixmap(img); break;
    case 113: ui->label_24->setPixmap(img); break;
    case 114: ui->label_25->setPixmap(img); break;
    }
}

void Widget::moveEvent(QMoveEvent *event)
{
    QPoint oldpos=event->oldPos();
    this->move(oldpos);

}




void Widget::savereport()
{

    if(save_f==0)
            return;
    save_f=0;
        if(current_language==1)
        {
                FILE *fp;
                char strbuf[80];
                int day,mon,year;
                int hour,minute,second;
                fp=fopen("report.txt","a+");
                if(fp==NULL)
                return;
                fprintf(fp,"\r\n");
                day=QDate::currentDate().day();
                mon=QDate::currentDate().month();
                year=QDate::currentDate().year();
                hour=QTime::currentTime().hour();
                minute=QTime::currentTime().minute();
                second=QTime::currentTime().second();
                fprintf(fp,"%d年%d月%d日 %d时%d分%d秒\r\n",year,mon,day,hour,minute,second);
                strcpy(strbuf,"Ir/Id 差动比率制动:");
                fprintf(fp,"%s\r\n",strbuf);

                if(Testobject==1)
                      strcpy(strbuf,"高对中");
                else if(Testobject==2)
                      strcpy(strbuf,"高对低");
                fprintf(fp,"测试对象：%s\r\n",strbuf);
                switch(ConnectType)
                {
                case 1:
                    strcpy(strbuf,"Y/Y/△-11");
                    break;
                case 2:
                    strcpy(strbuf,"Y/Y/△-1");
                    break;
                case 3:
                    strcpy(strbuf,"Y/Y/△-5");
                    break;
    }

    fprintf(fp,"接线方式：%s\r\n",strbuf);

    for(int i=0;i<TestResult.size();i++)
    {
           sprintf(strbuf,"Ir=%.5f,Id=%.5f",TestResult.at(i).x(),TestResult.at(i).y());
           fprintf(fp,"%s\r\n",strbuf);
     }
    fclose(fp);
    system("sync");
        }
        else
        {
            FILE *fp;
            char strbuf[80];
            int day,mon,year;
            int hour,minute,second;
            fp=fopen("report.txt","a+");
            if(fp==NULL)
            return;
            fprintf(fp,"\r\n");
            day=QDate::currentDate().day();
            mon=QDate::currentDate().month();
            year=QDate::currentDate().year();
            fprintf(fp,"Date:%d.%d.%d\n",mon,day,year);

            hour=QTime::currentTime().hour();
            minute=QTime::currentTime().minute();
            second=QTime::currentTime().second();
            fprintf(fp,"Time:%d.%d.%d\r\n",hour,minute,second);
            strcpy(strbuf,"Ir/Id Differential test result");
            fprintf(fp,"%s\r\n",strbuf);

            if(Testobject==1)
                  strcpy(strbuf,"Side1/Side2");
            else if(Testobject==2)
                  strcpy(strbuf,"Side1/Side3");
            fprintf(fp,"Connection:%s\r\n",strbuf);
            switch(ConnectType)
            {
            case 1:
                strcpy(strbuf,"Y/Y/△-11");
                break;
            case 2:
                strcpy(strbuf,"Y/Y/△-1");
                break;
            case 3:
                strcpy(strbuf,"Y/Y/△-5");
                break;
}

fprintf(fp,"Test object:%s\r\n",strbuf);

for(int i=0;i<TestResult.size();i++)
{
       sprintf(strbuf,"Ir=%.5f,Id=%.5f",TestResult.at(i).x(),TestResult.at(i).y());
       fprintf(fp,"%s\r\n",strbuf);
 }
fclose(fp);
system("sync");
        }

}



float Widget::MaxFloatNumer(QVector<QPointF> &map,int type)
{                                                    //1 id   2 ir
    float max=0;
    if(type==1)//id
    max=map.at(0).x();
    else
    max=map.at(0).y();
    for(int i=1;i<map.size();i++)
    {
        if(type==1)
        {
          if(map.at(i).x()>max)
              max=map.at(i).x();
        }
        else
          {  if(map.at(i).y()>max)
              max=map.at(i).y();
          }

    }
    return max;

}

float Widget::Keepfloatdata(float data,int n)  //保留几个小数
{

    float  b=pow(10,n);
    return (int)( b*data + 0.5)/b;

}


void Widget::ShowExcitCurve()
{


    if(TestResult.size()==0)
      {
          Curve->CleatAll();
          return;
      }


   float MaxI=MaxFloatNumer(TestResult,2); //ir
   float MaxV=MaxFloatNumer(TestResult,1); //id
   float IAxis,VAxis;





   VAxis=2*(IrSeachEnd+IrSeachStep)*IhStdValue;
   IAxis=(IrSeachEnd+IrSeachStep)*IhStdValue;


   CurveSetting.minX=0;
   CurveSetting.maxX=IAxis;
   CurveSetting.minY=0;
   CurveSetting.maxY=VAxis;
   Curve->setCurveData(0,TestResult);
   Curve->setPlotSettings(CurveSetting);
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked()
{
//帮助
    if(starttest==1)
        return;
    Helpdialog *helpdialog=new Helpdialog;
    temprorarydata->helpopenflag=true;
    connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
    if(current_language==2)
    helpdialog->SetHelpfilename("Differ.txt");
    else if(current_language==1)
        helpdialog->SetHelpfilename("Differ1.txt");
    //helpdialog->SetHelpfilename("Differ.txt");
    helpdialog->show();
}


void Widget::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

}


void Widget::on_pushButton_3_clicked()
{//save
    if(starttest==1)
        return;
    savereport();
}

void Widget::on_pushButton_4_clicked()
{//exit
    if(starttest==1)
        return;
  uartthread->stoprun();
  while(uartthread->isRunning())
  {
      uartthread->stoprun();
      sleep(1);
      break;
  }
    free(temprorarydata);
    temprorarydata=NULL;
    close();




}
void Widget::CloseWidget()
{

}



void Widget::on_comboBox_currentIndexChanged(int index)
{
    ConnectType=index+1;
}

void Widget::on_comboBox_2_currentIndexChanged(int index)
{
   PhaseCorrectMode=index+1;

   /*
   if(PhaseCorrectMode==1)
   {
    ui->comboBox_4->clear();
    ui->comboBox_5->clear();

    ui->comboBox_5->addItem(tr("Ir=(I1+I2+I3)"));
    ui->comboBox_4->addItem(tr("Id=max(I1+I2+I3)"));

   }
   else if(PhaseCorrectMode==2)
   {
       ui->comboBox_4->clear();
       ui->comboBox_5->clear();
       ui->comboBox_5->addItem(tr("Ir=(|I1+I2|)/2"));
       ui->comboBox_4->addItem(tr("Id=(|I1+I2|)"));

   }
  */

}

void Widget::on_comboBox_3_currentIndexChanged(int index)
{
    Testobject=index+1;
}







void Widget::on_pushButton_clicked()
{//caculate
    if(starttest==1)
        return;

 CaculateKp *caculatekp=new CaculateKp;
 connect(caculatekp,SIGNAL(SIG_SendVolchangerinfo(void *)),this,SLOT(UpdateCtinfo(void *)));
 connect(caculatekp,SIGNAL(SIG_SendClose()),this,SLOT(SetAuxcalflag()));
 caculatekp->SetVolchangerinfo(Volchanger);

 temprorarydata->AuxCaculateflag=true;
 caculatekp->activateWindow();
 caculatekp->raise();
 caculatekp->setModal(true);
 caculatekp->show();

}

void Widget::UpdateCtinfo(void *value)
{
Volchanger=*((VOLCHANGERINFO *)value);
Kp1=Volchanger.Side1.Kp;
Kp2=Volchanger.Side2.Kp;
Kp3=Volchanger.Side3.Kp;
IhStdValue=Volchanger.Side1.RateCur;
ImSdtValue=Volchanger.Side2.RateCur;
IlStdValue=Volchanger.Side3.RateCur;

ui->lineEdit_3->setText(QString::number(Kp2,'g',6));
ui->lineEdit_4->setText(QString::number(Kp3,'g',6));

}

void Widget::SetAuxcalflag()
{
temprorarydata->AuxCaculateflag=false;

}


void Widget::ShowChangeValue(float value)
{

    if(value==-500)
     {   ui->label_23->setText("");
         return;
     }

    ui->label_23->setText(tr("相位(Deg):")+QString::number(value,'g',4));



}


void Widget::on_radioButton_clicked()
{
  CTHdir=1;
}

void Widget::on_radioButton_2_clicked()
{
  CTHdir=2;
}



void Widget::on_radioButton_3_clicked()
{
  CTMdir=1;
}

void Widget::on_radioButton_4_clicked()
{
  CTMdir=2;
}

void Widget::on_radioButton_5_clicked()
{
  CTLdir=1;
}

void Widget::on_radioButton_6_clicked()
{
   CTLdir=2;
}

void Widget::on_pushButton_5_clicked()
{
      if(SoftStartEndflag==true)
            return;
        if(starttest==1)
        {
            ui->pushButton_5->setText(tr("测试"));
            ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_4->setEnabled(true);
        }
        else
        {
            ui->pushButton_5->setText(tr("停止"));
            ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
        }

        SoftStartEndflag=true;

}

void Widget::GetTestState(){
    if(starttest==1)
    {
        ui->pushButton_5->setText(tr("停止"));
        ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }
    else
    {
        ui->pushButton_5->setText(tr("测试"));
        ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    }

}
