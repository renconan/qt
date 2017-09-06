#ifndef MANUALTEST_H
#define MANUALTEST_H



#include <QWidget>
#include <thread1.h>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include "keypads.h"
#include "Help/helpdialog.h"
#include <QMouseEvent>
#include <Warning/warning.h>


namespace Ui {
    class Manualtest;
}

class Manualtest : public QWidget
{
    Q_OBJECT

public:
    explicit Manualtest(QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    ~Manualtest();
    void updateUI();
    void init_par();
    void display_mode(int mode);
    void display_variry(int variry);
    void initalldata();
    void clearallselect();  //清楚所有表格中的选中
    void vi_parameter_enter(int position);
    void displaydataandstate(int position);
    int vi_change_step(int add_or_dec,float devision);
    void updateVz(); //
    int currenteditpos;

    int VolPhaseChange;  //电压相位最后一次改变的位置
    int CurPhaseChange;  //电流相位最后一次改变的位置


    QList<QLineEdit *> lineeditlist; //linedit的容量器

public slots:
  void  showdspstate(int state);
  void  showtestresult(bool k);  //显示测试结果
  float showPhase(float value);
  void  setVanocheck();
   void HelpDialogClose();
   void moveEvent(QMoveEvent *);
    void CloseWidget();
  void  ShowWarning(int,int);
  void  WarningWinclose();
  void    GetTestState();
  void DisplayAutoChangeValue(int flag);

private:
    Ui::Manualtest *ui;
    thread1 *uartthread;
    keypads *pad;
    QLineEdit *currentedit;

signals:
   void SIG_Manualoutput();


private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_4_clicked();
    void on_checkBox_5_clicked();
    void on_checkBox_6_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
    void on_radioButton_6_clicked();


    void setlineedittext(QString str);
    void clearkeypadflag();

    void GetTeststate(int);


    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();
    void on_checkBox_7_clicked();
    void on_checkBox_8_clicked();
    void on_checkBox_9_clicked();

    void on_pushButton_6_clicked();
};

#endif // MANUALTEST_H
