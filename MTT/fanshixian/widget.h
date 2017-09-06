#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <thread1.h>
#include "keypads.h"
#include <QLineEdit>
#include "Help/helpdialog.h"
#include "Plotter/plotter.h"

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
    void updateUI();
  bool eventFilter(QObject *, QEvent *);

  int currenteditpos;
  void  init_par(void);
  void display_variry(int Outputmode);
  void display_testitem(int offset);

  void initlineedit();
  void vi_parameter_enter(int position);
  void Showfaulttype(int);

   void InitTestresult();
   void ShowExcitCurve();
   PlotSettings   CurveSetting;

public slots:

  void showtestresult(int num);
  void showdspstate(int state);
  void HelpDialogClose();
  void moveEvent(QMoveEvent *event);
  void CloseWidget();
   void    GetTestState();
   void    GetTeststate(int);



private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();



  void on_pushButton_3_clicked();

  void on_radioButton_clicked();

  void on_radioButton_2_clicked();

  void on_radioButton_3_clicked();




  void setlineedittext(QString str);
  void clearkeypadflag();


  void on_radioButton_4_clicked();

  void on_radioButton_5_clicked();

  void on_radioButton_6_clicked();

  void on_radioButton_7_clicked();

  void on_radioButton_9_clicked();

  void on_radioButton_8_clicked();

  void on_radioButton_10_clicked();

  void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    QLineEdit *currentedit;
    keypads *pad;
    Plotter *Curve;
    QString obj;

};

#endif // WIDGET_H
