#include "caculatekp.h"
#include "ui_caculatekp.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <QMouseEvent>
#include "qtranslator.h"
extern QTranslator *tor;
extern int current_language;  //1中文  2英文
CaculateKp::CaculateKp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaculateKp)
{
    ui->setupUi(this);
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

    this->setWindowTitle(tr(" "));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",13);
    setFont(font);
    showFullScreen();


}
void CaculateKp::updateUI()
{
     ui->label->setText(tr("额定容量(MVA):"));
    ui->label_3->setText(tr("高压侧"));
    ui->label_4->setText(tr("中压测"));
    ui->label_5->setText(tr("低压侧"));
    ui->label_6->setText(tr("额定电压(KV):"));
    ui->label_7->setText(tr("CT 变比:"));
    ui->label_8->setText(tr("额定电流(A):"));
    ui->label_9->setText(tr("平衡系数:"));

    ui->pushButton->setText(tr("确认"));
    ui->pushButton_2->setText(tr("保存/退出"));
    ui->pushButton_3->setText(tr("取消"));
}
bool CaculateKp::eventFilter(QObject *obj, QEvent *event)
{

    if(event->type()==QEvent::MouseButtonPress)
    {

        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
        if(mouseevent->button()==Qt::RightButton)
        {

            return true;
        }
        else
        return false;

    }
    else if(event->type()==QEvent::MouseButtonDblClick)
   {


        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
        if(mouseevent->button()==Qt::RightButton)
        {
            return true;
        }

       else if(obj==ui->lineEdit_2)
        {
             currentedit=ui->lineEdit_2;
             currenteditpos=1;

        }
        else if(obj==ui->lineEdit_3)
         {
              currentedit=ui->lineEdit_3;
              currenteditpos=2;

         }
        else if(obj==ui->lineEdit_4)
         {
              currentedit=ui->lineEdit_4;
              currenteditpos=3;

         }
        else if(obj==ui->lineEdit_7)
         {
              currentedit=ui->lineEdit_7;
              currenteditpos=4;

         }
        else if(obj==ui->lineEdit_8)
         {
              currentedit=ui->lineEdit_8;
              currenteditpos=5;

         }

        else if(obj==ui->lineEdit_9)
         {
              currentedit=ui->lineEdit_9;
              currenteditpos=6;
         }
        else if(obj==ui->lineEdit_12)
         {
              currentedit=ui->lineEdit_12;
              currenteditpos=7;
         }
        else if(obj==ui->lineEdit_13)
         {
              currentedit=ui->lineEdit_13;
              currenteditpos=8;
         }
        else if(obj==ui->lineEdit_14)
         {
              currentedit=ui->lineEdit_14;
              currenteditpos=9;
         }




        pad=new keypads;
        pad->setModal(true);
        connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
       // connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
        pad->show();
        pad->raise();
        pad->activateWindow();

        return true;
   }
    else
    return QWidget::eventFilter(obj,event);

}





CaculateKp::~CaculateKp()
{
    delete ui;
}

void CaculateKp::SetVolchangerinfo(VOLCHANGERINFO tmpinfo)
{
// memcmp(&VOLchanger,&tmpinfo,sizeof(VOLCHANGERINFO));
 VOLchanger=tmpinfo;



 ui->lineEdit_2->setText(QString::number(VOLchanger.Side1.RateDCapacity));
 ui->lineEdit_3->setText(QString::number(VOLchanger.Side1.RatedVol));
 ui->lineEdit_4->setText(QString::number(VOLchanger.Side1.CTRatio));
 ui->lineEdit_5->setText(QString::number(VOLchanger.Side1.RateCur));
 ui->lineEdit_6->setText(QString::number(VOLchanger.Side1.Kp));

 ui->lineEdit_7->setText(QString::number(VOLchanger.Side2.RateDCapacity));
 ui->lineEdit_8->setText(QString::number(VOLchanger.Side2.RatedVol));
 ui->lineEdit_9->setText(QString::number(VOLchanger.Side2.CTRatio));
 ui->lineEdit_10->setText(QString::number(VOLchanger.Side2.RateCur));
 ui->lineEdit_11->setText(QString::number(VOLchanger.Side2.Kp));

 ui->lineEdit_12->setText(QString::number(VOLchanger.Side3.RateDCapacity));
 ui->lineEdit_13->setText(QString::number(VOLchanger.Side3.RatedVol));
 ui->lineEdit_14->setText(QString::number(VOLchanger.Side3.CTRatio));
 ui->lineEdit_15->setText(QString::number(VOLchanger.Side3.RateCur));
 ui->lineEdit_16->setText(QString::number(VOLchanger.Side3.Kp));


 ui->lineEdit_2->installEventFilter(this);
 ui->lineEdit_3->installEventFilter(this);
 ui->lineEdit_4->installEventFilter(this);

 ui->lineEdit_7->installEventFilter(this);
 ui->lineEdit_8->installEventFilter(this);
 ui->lineEdit_9->installEventFilter(this);

 ui->lineEdit_12->installEventFilter(this);
 ui->lineEdit_13->installEventFilter(this);
 ui->lineEdit_14->installEventFilter(this);


 ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
 ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
 ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);

 ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
 ui->lineEdit_8->setContextMenuPolicy(Qt::NoContextMenu);
 ui->lineEdit_9->setContextMenuPolicy(Qt::NoContextMenu);

 ui->lineEdit_12->setContextMenuPolicy(Qt::NoContextMenu);
 ui->lineEdit_13->setContextMenuPolicy(Qt::NoContextMenu);
 ui->lineEdit_14->setContextMenuPolicy(Qt::NoContextMenu);

 ui->lineEdit_5->setEnabled(false);
 ui->lineEdit_6->setEnabled(false);
 ui->lineEdit_10->setEnabled(false);
 ui->lineEdit_11->setEnabled(false);
 ui->lineEdit_15->setEnabled(false);
 ui->lineEdit_16->setEnabled(false);

}

void CaculateKp::on_pushButton_clicked()
{
  // VOLCHANGERINFO tmp;
   if(ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()||ui->lineEdit_4->text().isEmpty()||\
      ui->lineEdit_7->text().isEmpty()||ui->lineEdit_8->text().isEmpty()||ui->lineEdit_9->text().isEmpty()||\
      ui->lineEdit_12->text().isEmpty()||ui->lineEdit_13->text().isEmpty()||ui->lineEdit_14->text().isEmpty())
   {
       if(current_language==1)
            QMessageBox::warning(this,tr("警告"),tr("输入为空!"),QMessageBox::Ok);
       else
            QMessageBox::warning(this,tr("Warning"),tr("Input is empty!"),QMessageBox::Ok);
       return;
   }
   bool ok;
   VOLchanger.Side1.RateDCapacity=ui->lineEdit_2->text().toFloat(&ok);
   VOLchanger.Side1.RatedVol=ui->lineEdit_3->text().toFloat(&ok);
   VOLchanger.Side1.CTRatio=ui->lineEdit_4->text().toFloat(&ok);

   VOLchanger.Side2.RateDCapacity=ui->lineEdit_7->text().toFloat(&ok);
   VOLchanger.Side2.RatedVol=ui->lineEdit_8->text().toFloat(&ok);
   VOLchanger.Side2.CTRatio=ui->lineEdit_9->text().toFloat(&ok);

   VOLchanger.Side3.RateDCapacity=ui->lineEdit_12->text().toFloat(&ok);
   VOLchanger.Side3.RatedVol=ui->lineEdit_13->text().toFloat(&ok);
   VOLchanger.Side3.CTRatio=ui->lineEdit_14->text().toFloat(&ok);

   ui->lineEdit_5->setEnabled(true);
   ui->lineEdit_6->setEnabled(true);
   ui->lineEdit_10->setEnabled(true);
   ui->lineEdit_11->setEnabled(true);
   ui->lineEdit_15->setEnabled(true);
   ui->lineEdit_16->setEnabled(true);

   VOLchanger.Side1.Kp=1;
   VOLchanger.Side2.Kp=VOLchanger.Side2.RatedVol*VOLchanger.Side2.CTRatio/\
                       (VOLchanger.Side1.RatedVol*VOLchanger.Side1.CTRatio);
   VOLchanger.Side3.Kp=VOLchanger.Side3.RatedVol*VOLchanger.Side3.CTRatio/\
                       (VOLchanger.Side1.RatedVol*VOLchanger.Side1.CTRatio);

   VOLchanger.Side1.RateCur=VOLchanger.Side1.RateDCapacity*1000/\
                (1.732*VOLchanger.Side1.RatedVol*VOLchanger.Side1.CTRatio);
   VOLchanger.Side2.RateCur=VOLchanger.Side2.RateDCapacity*1000/\
                (1.732*VOLchanger.Side2.RatedVol*VOLchanger.Side2.CTRatio);
   VOLchanger.Side3.RateCur=VOLchanger.Side3.RateDCapacity*1000/\
                (1.732*VOLchanger.Side3.RatedVol*VOLchanger.Side3.CTRatio);


   ui->lineEdit_2->setText(QString::number(VOLchanger.Side1.RateDCapacity));
   ui->lineEdit_3->setText(QString::number(VOLchanger.Side1.RatedVol));
   ui->lineEdit_4->setText(QString::number(VOLchanger.Side1.CTRatio));
   ui->lineEdit_5->setText(QString::number(VOLchanger.Side1.RateCur));
   ui->lineEdit_6->setText(QString::number(VOLchanger.Side1.Kp));

   ui->lineEdit_7->setText(QString::number(VOLchanger.Side2.RateDCapacity));
   ui->lineEdit_8->setText(QString::number(VOLchanger.Side2.RatedVol));
   ui->lineEdit_9->setText(QString::number(VOLchanger.Side2.CTRatio));
   ui->lineEdit_10->setText(QString::number(VOLchanger.Side2.RateCur));
   ui->lineEdit_11->setText(QString::number(VOLchanger.Side2.Kp));

   ui->lineEdit_12->setText(QString::number(VOLchanger.Side3.RateDCapacity));
   ui->lineEdit_13->setText(QString::number(VOLchanger.Side3.RatedVol));
   ui->lineEdit_14->setText(QString::number(VOLchanger.Side3.CTRatio));
   ui->lineEdit_15->setText(QString::number(VOLchanger.Side3.RateCur));
   ui->lineEdit_16->setText(QString::number(VOLchanger.Side3.Kp));




   ui->lineEdit_5->setEnabled(false);
   ui->lineEdit_6->setEnabled(false);
   ui->lineEdit_10->setEnabled(false);
   ui->lineEdit_11->setEnabled(false);
   ui->lineEdit_15->setEnabled(false);
   ui->lineEdit_16->setEnabled(false);

}

void CaculateKp::on_pushButton_2_clicked()
{

    emit SIG_SendVolchangerinfo((void *)&VOLchanger);
    emit SIG_SendClose();
    close();
}

void CaculateKp::on_pushButton_3_clicked()
{
    emit SIG_SendClose();
    close();
}

void CaculateKp::closeEvent(QCloseEvent *event)//关闭按钮重载
{
    emit SIG_SendClose();
    close();

}
void CaculateKp::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    currentedit->setText(QString::number(number,'g',6));

}

