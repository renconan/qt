#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QTimer>
#include "thread1.h"
#include <QLineEdit>
#include <keypads.h>
#include "Help/helpdialog.h"
#include <QMouseEvent>
#include "thread1.h"
#include <Warning/warning.h>


#ifdef ARMLINUX
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <math.h>
#include <dirent.h>
#include <sys/vfs.h>
#endif

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);
    ~Widget();
    void updateUI();
   void  init_par();
   void  sysinit();
    int currenteditpos;
   void vi_parameter_enter(int position);
   void LLLNormalFaultPar();
   void LLNormalFaultPar();
   void LNNormalFaultPar();
   int  OutputSignalCompute(int flag,int k);
   void ShowCaculateValue(int);

   float CaculatePhase(float r,float y);
   void CaculatePerfalultPhase();
   int  ErrorWarning(int segment,int fault);

protected slots:
  void showtestresult(int num);
  void showdspstate(int state);
  void showwarnint(int sectnum,int num);
  void changetab(int);
  void ShowPage(int page);
  void initshowresult(void);//≥ı ºªØœ‘ æ
  void HelpDialogClose();
  void moveEvent(QMoveEvent *event);
  void ShowHeader(QString str);
  void Showzone(int i);
  void CloseWidget();
     void    GetTestState();
     void    GetTeststate(int);





private slots:
  void on_pushButton_9_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

  void on_checkBox_8_clicked();

  void on_checkBox_9_clicked();

  void on_checkBox_11_clicked();

  void on_checkBox_clicked();

  void on_checkBox_2_clicked();

  void on_checkBox_3_clicked();

  void on_checkBox_4_clicked();

  void on_checkBox_5_clicked();

  void on_checkBox_6_clicked();

  void on_checkBox_7_clicked();

  void on_checkBox_10_clicked();

/*
  void on_lineEdit_7_selectionChanged();

  void on_lineEdit_8_selectionChanged();

  void on_lineEdit_9_selectionChanged();

  void on_lineEdit_10_selectionChanged();

  void on_lineEdit_11_selectionChanged();

  void on_lineEdit_12_selectionChanged();

  void on_lineEdit_13_selectionChanged();

  void on_lineEdit_14_selectionChanged();

  void on_lineEdit_15_selectionChanged();

  void on_lineEdit_16_selectionChanged();

  void on_lineEdit_17_selectionChanged();

  void on_lineEdit_18_selectionChanged();

  void on_lineEdit_19_selectionChanged();

  void on_lineEdit_20_selectionChanged();

  void on_lineEdit_21_selectionChanged();

  void on_lineEdit_22_selectionChanged();

  void on_lineEdit_selectionChanged();

  void on_lineEdit_2_selectionChanged();

  void on_lineEdit_3_selectionChanged();

  void on_lineEdit_4_selectionChanged();

  void on_lineEdit_5_selectionChanged();

  void on_lineEdit_6_selectionChanged();
*/
  void setlineedittext(QString str);
  void clearkeypadflag();

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_checkBox_12_clicked();

  void on_textBrowser_textChanged();

  void on_tabWidget_currentChanged(int index);

  void on_pushButton_6_clicked();

  void on_pushButton_7_clicked();

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    QLineEdit *currentedit;
    keypads *pad;

};

#endif // WIDGET_H
