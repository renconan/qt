#include "warning.h"
#include "ui_warning.h"
#include <QTextCodec>
#include "qtranslator.h"
extern QTranslator *tor;
extern int current_language;  //1中文  2英文
Warning::Warning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Warning)
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

    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    setWindowTitle(tr("         提示"));
    ui->pushButton->setFocusPolicy(Qt::NoFocus);


    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
}
void Warning::updateUI()
{
    ui->pushButton->setText(tr("确定"));
}
void Warning::Setwarning(int type,QString msg)
{

if(type==1)
 {  ui->label_2->setText(QString(" "));    
    ui->label->setText(msg);
    ui->label->setWordWrap(true);
    ui->label->setAlignment(Qt::AlignVCenter);
 }


}

Warning::~Warning()
{
    delete ui;
}

void Warning::on_pushButton_clicked()
{
this->close();
}

void Warning::closeEvent(QCloseEvent *event)
 {
     this->close();
 }
