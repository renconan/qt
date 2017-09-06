#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <thread1.h>
#include "keypads.h"
#include <QLineEdit>
#include "Help/helpdialog.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    bool eventFilter(QObject *, QEvent *);

  int saveflag;
  void  init_par(void);
  void updateUI();
  void display_variry(int Outputmode);
  void  vi_parameter_enter(int position);

 

public slots:
  void DisplayChangeValue(bool flag,float value);
  void showtestresult(int num);
  void showdspstate(int state);
  void HelpDialogClose();
  void moveEvent(QMoveEvent *);
  void CloseWidget();
  void    GetTestState();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();



    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_7_clicked();


/*
    void on_lineEdit_selectionChanged();
    void on_lineEdit_2_selectionChanged();
    void on_lineEdit_3_selectionChanged();
    void on_lineEdit_4_selectionChanged();
    void on_lineEdit_5_selectionChanged();
    void on_lineEdit_6_selectionChanged();
    void on_lineEdit_7_selectionChanged();
    void on_lineEdit_8_selectionChanged();

    */

    void setlineedittext(QString str);
    void clearkeypadflag();




    void on_tabWidget_currentChanged(int index);

    void on_checkBox_clicked();



    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    QLineEdit *currentedit;
    keypads *pad;
    int currenteditpos;
};

#endif // WIDGET_H
