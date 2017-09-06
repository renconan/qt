#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <keypads.h>
#include "thread1.h"
#include "Help/helpdialog.h"
#include <QMouseEvent>




namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void  init_par();
    void  sysinit();
    void  disfaulttype(int type);
    void  vi_parameter_enter(int position);
    int   currenteditpos;


    void LLLNormalFaultPar();
    void LLNormalFaultPar();
    void LNNormalFaultPar();
    int  OutputSignalCompute(int Fault);

    void CaculatePerfalultPhase();
    float CaculatePhase(float r,float y);




public slots:
  void showtestresult(int num);
  void showdspstate(int state);
  void showwarnint(int num);
  void changetab(int);
  void HelpDialogClose();  
   void CloseWidget();
    void    GetTestState();




private slots:
  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_radioButton_clicked();

  void on_radioButton_2_clicked();

  void on_radioButton_3_clicked();

  void on_radioButton_4_clicked();

  void on_radioButton_5_clicked();

  void on_radioButton_6_clicked();

  void on_radioButton_7_clicked();

  void on_radioButton_8_clicked();

  void on_radioButton_9_clicked();




  void setlineedittext(QString str);
  void clearkeypadflag();

  void on_pushButton_clicked();

  void on_checkBox_clicked();
  void on_pushButton_2_clicked();

  void on_textBrowser_textChanged();

  void on_tabWidget_currentChanged(int index);

  void on_pushButton_5_clicked();

  void on_lineEdit_6_textChanged(const QString &arg1);

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    keypads *pad;
    QLineEdit *currentedit;

};

#endif // WIDGET_H
