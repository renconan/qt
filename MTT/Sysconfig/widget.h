#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "keypads.h"
namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

 void  init_par(void);
 void updateUI();
 void initalldata();
 void vi_parameter_enter(int position);
 int currenteditpos;
 bool eventFilter(QObject *, QEvent *);
protected slots:
  void moveEvent(QMoveEvent *event);


private slots:
  void on_pushButton_clicked();
  void on_pushButton_3_clicked();
  void on_checkBox_clicked();

  void on_checkBox_2_clicked();

  void on_checkBox_3_clicked();



  void setlineedittext(QString str);
  void clearkeypadflag();



  void on_pushButton_6_clicked();

  void on_pushButton_7_clicked();

  void on_radioButton_clicked();

  void on_radioButton_2_clicked();  

  void on_radioButton_3_clicked();

  void on_radioButton_4_clicked();

  void on_radioButton_5_clicked();

private:
    Ui::Widget *ui;
    QLineEdit *currentedit;
    keypads *pad;
};

#endif // WIDGET_H
