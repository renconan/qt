#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "thread1.h"
#include "keypads.h"
#include <QLineEdit>

#include <QMouseEvent>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTableWidgetItem>
#include <QDebug>
#include <QDate>
#include <QTextCodec>
#include <QMessageBox>
#include "CaculateKp/caculatekp.h"
#include "plotter.h"
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
    void updateUI();
    bool eventFilter(QObject *, QEvent *);
    void init_par();
    void sysinit();
    void vi_parameter_enter(int position);
    void savereport();

    void ShowExcitCurve();
    float MaxFloatNumer(QVector<QPointF> &map,int type);
    float Keepfloatdata(float data,int n);
    //励磁曲线
    QVector<float> MapMaxXaxis; //每个曲线最大的X轴值
    QVector<float> MapMaxYaxis;
    PlotSettings   CurveSetting;



 public slots:
   void showtestresult(int num);
   void showdspstate(int state);
   void moveEvent(QMoveEvent *event);
   void setlineedittext(QString str);
   void clearkeypadflag();

   void UpdateCtinfo(void *);
   void SetAuxcalflag();
   void CloseWidget();
   void ShowChangeValue(float);
   void HelpDialogClose();
    void    GetTeststate(int);
    void    GetTestState();






private slots:
   void on_pushButton_2_clicked();

   void on_pushButton_3_clicked();

   void on_pushButton_4_clicked();



   void on_comboBox_currentIndexChanged(int index);
   void on_comboBox_2_currentIndexChanged(int index);
   void on_comboBox_3_currentIndexChanged(int index);

   void on_pushButton_clicked();






   void on_radioButton_clicked();

   void on_radioButton_2_clicked();

   void on_radioButton_3_clicked();

   void on_radioButton_4_clicked();

   void on_radioButton_5_clicked();

   void on_radioButton_6_clicked();

   void on_pushButton_5_clicked();

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    int currenteditpos;
    keypads *pad;
    QLineEdit *currentedit;
    Plotter *Curve;
};

#endif // WIDGET_H
