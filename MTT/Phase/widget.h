#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "thread1.h"
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
    void init_par(void);
    void updateUI();
    void display_testitem(int offset);
    void vi_parameter_enter(int position);
    int currenteditpos;
    bool eventFilter(QObject *, QEvent *);






protected slots:
    void DisplayChangeValue(bool flag,float value);
    void showtestresult(int num);
    void HelpDialogClose();

    void showdspstate(int state);

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_5_clicked();

    void on_checkBox_6_clicked();




    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();



    void CloseWidget();
    void setlineedittext(QString str);
    void clearkeypadflag();
    void moveEvent(QMoveEvent *event);

    void    GetTestState();


private slots:
    void on_pushButton_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
      Ui::Widget *ui;
      thread1 *uartthread;
      QLineEdit *currentedit;
      keypads *pad;
};

#endif // WIDGET_H
