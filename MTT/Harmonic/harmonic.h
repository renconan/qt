#ifndef HARMONIC_H
#define HARMONIC_H

#include <QWidget>
#include <thread1.h>
#include <keypads.h>
#include <QLineEdit>
#include "Help/helpdialog.h"
#include <QMouseEvent>
#include "Config.h"
namespace Ui {
    class Harmonic;
}

class Harmonic : public QWidget
{
    Q_OBJECT

public:
    explicit Harmonic(QWidget *parent = 0);
    void init_par();
    bool eventFilter(QObject *, QEvent *);
    void sysinit();
    void vi_parameter_enter(int position);
     int currenteditpos;
     void updateUI();
    ~Harmonic();


public slots:
  void showtestresult(float num);
  void showdspstate(int state);
  void displaychange(int ,float);
  void HelpDialogClose();
  void moveEvent(QMoveEvent *event);
  void CloseWidget();
   void    GetTestState();
   void    GetTeststate(int);
private:
    Ui::Harmonic *ui;
    thread1 *uartthread;
    QLineEdit *currentedit;
    keypads *pad;


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();



    void setlineedittext(QString str);
    void clearkeypadflag();

    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_pushButton_4_clicked();
};

#endif // HARMONIC_H
