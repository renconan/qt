#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <thread1.h>

#include "keypads.h"
#include <QLineEdit>
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
    void updateUI();
    bool eventFilter(QObject *, QEvent *);
  int saveflag;
  void  init_par(void);
  void  vi_parameter_enter(int);
  int currenteditpos;




public slots:
  void DisplayChangeValue(bool flag,float value);
  void showtestresult(int num);
  void showdspstate(int state);
  void HelpDialogClose();
  void moveEvent(QMoveEvent *event);
  void CloseWidget();
  void    GetTestState();
  void    GetTeststate(int);
private slots:
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();



  void setlineedittext(QString str);
  void clearkeypadflag();



  void on_comboBox_currentIndexChanged(int index);


  void on_pushButton_clicked();

  void on_tabWidget_currentChanged(int index);

  void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    keypads *pad;
    QLineEdit *currentedit;


};

#endif // WIDGET_H
