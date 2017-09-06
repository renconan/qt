#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <thread1.h>
#include <QLineEdit>
#include "keypads.h"
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
    void init_par();
    bool eventFilter(QObject *, QEvent *);
    void vi_parameter_enter(int position);
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

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    QLineEdit *currentedit;
    keypads *pad;



private slots:
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
    void on_radioButton_6_clicked();






    void setlineedittext(QString str);
    void clearkeypadflag();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();



    void on_tabWidget_currentChanged(int index);
    void on_checkBox_clicked();
    void on_radioButton_7_clicked();
    void on_radioButton_8_clicked();
    void on_pushButton_4_clicked();
};

#endif // WIDGET_H
