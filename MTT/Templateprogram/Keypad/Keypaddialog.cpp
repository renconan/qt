#include "Keypaddialog.h"
#include "ui_Keydialog.h"
#include "Config.h"
#include "Defpam.h"
#include <QFile>
#include <QDebug>






KeyPadDialog::KeyPadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyPadDialog)
{
    ui->setupUi(this);
    memset(Textchar,'\0',sizeof(Textchar));
    setWindowTitle(QString(" "));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    initkeyshow(0,0);
    QFont font("wenquanyi",16);
    setFont(font);
     Caps_on=false;
     Shift_on=false;
     Textcharlen=0;
     ui->lineEdit->setFocusPolicy(Qt::NoFocus);
     ui->toolButton->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_2->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_3->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_4->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_5->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_6->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_7->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_8->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_9->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_10->setFocusPolicy(Qt::NoFocus);

     ui->toolButton_11->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_12->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_13->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_14->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_15->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_16->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_17->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_18->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_19->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_20->setFocusPolicy(Qt::NoFocus);

     ui->toolButton_21->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_22->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_23->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_24->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_25->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_26->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_27->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_28->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_29->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_30->setFocusPolicy(Qt::NoFocus);


     ui->toolButton_31->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_32->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_33->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_34->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_35->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_36->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_37->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_38->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_39->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_40->setFocusPolicy(Qt::NoFocus);

     ui->toolButton_41->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_42->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_43->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_44->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_45->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_46->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_47->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_48->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_49->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_50->setFocusPolicy(Qt::NoFocus);

     ui->toolButton_51->setFocusPolicy(Qt::NoFocus);
     ui->toolButton_52->setFocusPolicy(Qt::NoFocus);














}



void KeyPadDialog::updateshow(bool caps,bool shift) //更新显示
{
    initkeyshow(shift,caps);
}

void KeyPadDialog::keyboardclose()     //键盘关闭
{
 emit SIG_sendtext(Textchar);
 emit SIG_closeflag();
 close();
}

/*
 void KeyPadDialog::closeEvent(QCloseEvent *e)
 {
   emit SIG_sendtext(NULL);
   close();
 }

*/

void KeyPadDialog::dealdelete()       // 处理删除键
{

    if(Textcharlen>=1)
     {   Textchar[Textcharlen-1]='\0';
         Textcharlen--;
         ui->lineEdit->setText(QString(Textchar));

     }
}

void KeyPadDialog::getkey(char key)      //获得键值
{
    Textchar[Textcharlen]=key;
    Textcharlen++;
    ui->lineEdit->setText(QString(Textchar));
}

 void KeyPadDialog::closeEvent(QCloseEvent *e)
 {
     emit SIG_closeflag();
     close();
 }




void KeyPadDialog::initkeyshow(bool shift,bool tab)
{
    if(!shift)
   {
    ui->toolButton->setText(tr("1"));
    ui->toolButton_2->setText(tr("2"));
    ui->toolButton_3->setText(tr("3"));
    ui->toolButton_4->setText(tr("4"));
    ui->toolButton_5->setText(tr("5"));
    ui->toolButton_6->setText(tr("6"));
    ui->toolButton_7->text().clear();
    ui->toolButton_7->icon().detach();
    ui->toolButton_7->setText(tr("7"));
    ui->toolButton_8->setText(tr("8"));
    ui->toolButton_9->setText(tr("9"));
    ui->toolButton_10->setText(tr("0"));
    ui->toolButton_11->setText(tr("-"));
    ui->toolButton_12->setText(tr("="));

     ui->toolButton_23->setText(tr("\\"));
     ui->toolButton_24->setText("del");


     ui->toolButton_34->setText(tr(";"));
     ui->toolButton_35->setText(tr("/"));

     ui->toolButton_36->setText(tr("`"));
     ui->toolButton_44->setText(tr(","));
     ui->toolButton_45->setText(tr("."));
     ui->toolButton_46->setText(tr("'"));
     ui->toolButton_47->setText(tr("["));
     ui->toolButton_48->setText(tr("]"));

    }

    else {

        ui->toolButton->setText(tr("!"));
        ui->toolButton_2->setText(tr("@"));
        ui->toolButton_3->setText(tr("#"));
        ui->toolButton_4->setText(tr("$"));
        ui->toolButton_5->setText(tr("%"));
        ui->toolButton_6->setText(tr("^"));
        char *mm="&&";
        ui->toolButton_7->setText(tr(mm));
        ui->toolButton_8->setText(tr("*"));
        ui->toolButton_9->setText(tr("("));
        ui->toolButton_10->setText(tr(")"));
        ui->toolButton_11->setText(tr("_"));
        ui->toolButton_12->setText(tr("+"));


        ui->toolButton_23->setText(tr("|"));

        ui->toolButton_24->setText("del");

        ui->toolButton_34->setText(tr(":"));

        ui->toolButton_35->setText(tr("?"));
        ui->toolButton_36->setText(tr("~"));

        ui->toolButton_44->setText(tr("<"));
        ui->toolButton_45->setText(tr(">"));
        ui->toolButton_46->setText(tr("\""));
        ui->toolButton_47->setText(tr("{"));
        ui->toolButton_48->setText(tr("}"));

    }

    if(!tab)
{
    ui->toolButton_13->setText(tr("q"));
    ui->toolButton_14->setText(tr("w"));
    ui->toolButton_15->setText(tr("e"));
    ui->toolButton_16->setText(tr("r"));
    ui->toolButton_17->setText(tr("t"));
    ui->toolButton_18->setText(tr("y"));
    ui->toolButton_19->setText(tr("u"));
    ui->toolButton_20->setText(tr("i"));
    ui->toolButton_21->setText(tr("o"));
    ui->toolButton_22->setText(tr("p"));

   ui->toolButton_25->setText(tr("a"));
   ui->toolButton_26->setText(tr("s"));
   ui->toolButton_27->setText(tr("d"));
   ui->toolButton_28->setText(tr("f"));
   ui->toolButton_29->setText(tr("g"));
   ui->toolButton_30->setText(tr("h"));
   ui->toolButton_31->setText(tr("j"));
   ui->toolButton_32->setText(tr("k"));
   ui->toolButton_33->setText(tr("l"));

   ui->toolButton_37->setText(tr("z"));
   ui->toolButton_38->setText(tr("x"));
   ui->toolButton_39->setText(tr("c"));
   ui->toolButton_40->setText(tr("v"));
   ui->toolButton_41->setText(tr("b"));
   ui->toolButton_42->setText(tr("n"));
   ui->toolButton_43->setText(tr("m"));

 }


    else{


        ui->toolButton_13->setText(tr("Q"));
        ui->toolButton_14->setText(tr("W"));
        ui->toolButton_15->setText(tr("E"));
        ui->toolButton_16->setText(tr("R"));
        ui->toolButton_17->setText(tr("T"));
        ui->toolButton_18->setText(tr("Y"));
        ui->toolButton_19->setText(tr("U"));
        ui->toolButton_20->setText(tr("I"));
        ui->toolButton_21->setText(tr("O"));
        ui->toolButton_22->setText(tr("P"));

       ui->toolButton_25->setText(tr("A"));
       ui->toolButton_26->setText(tr("S"));
       ui->toolButton_27->setText(tr("D"));
       ui->toolButton_28->setText(tr("F"));
       ui->toolButton_29->setText(tr("G"));
       ui->toolButton_30->setText(tr("H"));
       ui->toolButton_31->setText(tr("J"));
       ui->toolButton_32->setText(tr("K"));
       ui->toolButton_33->setText(tr("L"));

       ui->toolButton_37->setText(tr("Z"));
       ui->toolButton_38->setText(tr("X"));
       ui->toolButton_39->setText(tr("C"));
       ui->toolButton_40->setText(tr("V"));
       ui->toolButton_41->setText(tr("B"));
       ui->toolButton_42->setText(tr("N"));
       ui->toolButton_43->setText(tr("M"));
    }


}


 void KeyPadDialog::SetText(QString text)
 {
    if(text.isEmpty())
        return;

    QByteArray ba=text.toLatin1();
    int i=0;
    for( i=0;i<ba.size();i++)
    {
     Textchar[i]=ba.at(i);
    }
    Textchar[ba.size()]='\0';
    Textcharlen=strlen(Textchar);
    ui->lineEdit->setText(QString(Textchar));
 }



KeyPadDialog::~KeyPadDialog()
{
    delete ui;
}

void KeyPadDialog::on_toolButton_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='1';
    else key='!';
    getkey(key);
}

void KeyPadDialog::on_toolButton_2_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='2';
    else key='@';
    getkey(key);
}

void KeyPadDialog::on_toolButton_3_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='3';
    else key='#';
    getkey(key);
}

void KeyPadDialog::on_toolButton_4_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='4';
    else key='$';
    getkey(key);
}

void KeyPadDialog::on_toolButton_5_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='5';
    else key='%';
    getkey(key);
}

void KeyPadDialog::on_toolButton_6_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='6';
    else key='^';
    getkey(key);
}

void KeyPadDialog::on_toolButton_7_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='7';
    else key='&';
    getkey(key);
}

void KeyPadDialog::on_toolButton_8_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='8';
    else key='*';
    getkey(key);
}

void KeyPadDialog::on_toolButton_9_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='9';
    else key='(';
    getkey(key);
}

void KeyPadDialog::on_toolButton_10_clicked() //0
{
    char key;
    if(!Shift_on) //未开启大小写
        key='0';
    else key=')';
    getkey(key);
}

void KeyPadDialog::on_toolButton_11_clicked() //-
{
    char key;
    if(!Shift_on) //未开启大小写
        key='-';
    else key='_';
    getkey(key);
}

void KeyPadDialog::on_toolButton_12_clicked() //=
{
    char key;
    if(!Shift_on) //未开启大小写
        key='=';
    else key='+';
    getkey(key);
}

void KeyPadDialog::on_toolButton_13_clicked() //q
{
    char key;
    if(!Caps_on) //未开启大小写
        key='q';
    else key='Q';
    getkey(key);
}

void KeyPadDialog::on_toolButton_14_clicked() //w
{
    char key;
    if(!Caps_on) //未开启大小写
        key='w';
    else key='W';
    getkey(key);
}

void KeyPadDialog::on_toolButton_15_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='e';
    else key='E';
    getkey(key);
}

void KeyPadDialog::on_toolButton_16_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='r';
    else key='R';
    getkey(key);
}

void KeyPadDialog::on_toolButton_17_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='t';
    else key='T';
    getkey(key);
}

void KeyPadDialog::on_toolButton_18_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='y';
    else key='Y';
    getkey(key);
}

void KeyPadDialog::on_toolButton_19_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='u';
    else key='U';
    getkey(key);
}

void KeyPadDialog::on_toolButton_20_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='i';
    else key='I';
    getkey(key);
}

void KeyPadDialog::on_toolButton_21_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='o';
    else key='O';
    getkey(key);
}

void KeyPadDialog::on_toolButton_22_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='p';
    else key='P';
    getkey(key);
}

void KeyPadDialog::on_toolButton_23_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key=92;
    else key='|';
    getkey(key);
}

void KeyPadDialog::on_toolButton_24_clicked()
{
 dealdelete();


}

void KeyPadDialog::on_toolButton_25_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='a';
    else key='A';
    getkey(key);
}

void KeyPadDialog::on_toolButton_26_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='s';
    else key='S';
    getkey(key);
}

void KeyPadDialog::on_toolButton_27_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='d';
    else key='D';
    getkey(key);
}

void KeyPadDialog::on_toolButton_28_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='f';
    else key='F';
    getkey(key);
}

void KeyPadDialog::on_toolButton_29_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='g';
    else key='G';
    getkey(key);
}

void KeyPadDialog::on_toolButton_30_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='h';
    else key='H';
    getkey(key);
}

void KeyPadDialog::on_toolButton_31_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='j';
    else key='J';
    getkey(key);
}

void KeyPadDialog::on_toolButton_32_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='k';
    else key='K';
    getkey(key);
}

void KeyPadDialog::on_toolButton_33_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='l';
    else key='L';
    getkey(key);
}

void KeyPadDialog::on_toolButton_34_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key=';';
    else key=':';
    getkey(key);
}

void KeyPadDialog::on_toolButton_35_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='/';
    else key='?';
    getkey(key);
}

void KeyPadDialog::on_toolButton_36_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='/';
    else key='?';
    getkey(key);
}

void KeyPadDialog::on_toolButton_37_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='z';
    else key='Z';
    getkey(key);
}

void KeyPadDialog::on_toolButton_38_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='x';
    else key='X';
    getkey(key);
}

void KeyPadDialog::on_toolButton_39_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='c';
    else key='C';
    getkey(key);
}

void KeyPadDialog::on_toolButton_40_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='v';
    else key='V';
    getkey(key);
}

void KeyPadDialog::on_toolButton_41_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='b';
    else key='B';
    getkey(key);
}

void KeyPadDialog::on_toolButton_42_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='n';
    else key='N';
    getkey(key);
}

void KeyPadDialog::on_toolButton_43_clicked()
{
    char key;
    if(!Caps_on) //未开启大小写
        key='m';
    else key='M';
    getkey(key);
}

void KeyPadDialog::on_toolButton_44_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key=',';
    else key='<';
    getkey(key);
}

void KeyPadDialog::on_toolButton_45_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='.';
    else key='>';
    getkey(key);
}

void KeyPadDialog::on_toolButton_46_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key=39;
    else key=34;
    getkey(key);
}

void KeyPadDialog::on_toolButton_47_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key='[';
    else key='{';
    getkey(key);
}

void KeyPadDialog::on_toolButton_48_clicked()
{
    char key;
    if(!Shift_on) //未开启大小写
        key=']';
    else key='}';
    getkey(key);
}

void KeyPadDialog::on_toolButton_49_clicked()
{
 Caps_on=!Caps_on;
 updateshow(Caps_on,Shift_on);
}

void KeyPadDialog::on_toolButton_50_clicked()
{
    Shift_on=!Shift_on;
    updateshow(Caps_on,Shift_on);
}

void KeyPadDialog::on_toolButton_51_clicked()
{
   char key;
   key=' ';
   getkey(key);
}

void KeyPadDialog::on_toolButton_52_clicked()
{
keyboardclose();
}
