#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <thread1.h>
#include <QLineEdit>
#include <keypads.h>
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
    bool eventFilter(QObject *, QEvent *);
    ~Widget();
    void updateUI();
  void init_par(void);
  void vi_parameter_enter(int position);
   int currenteditpos;
public slots:
  void DisplayChangeValue(bool flag,float value);
  void showtestresult(int num);
  void showdspstate(int state);
  void setlineedittext(QString str);
  void clearkeypadflag();
  void HelpDialogClose();
  void moveEvent(QMoveEvent *event);
   void CloseWidget();
   void    GetTestState();
void    GetTeststate(int);


private:
    Ui::Widget *ui;
    thread1 *uartthread;
    keypads *pad;
    QLineEdit *currentedit;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();





    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_pushButton_4_clicked();
};

#endif // WIDGET_H
