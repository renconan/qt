#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <thread1.h>
#include "keypads.h"
#include <QLineEdit>
#include <QFileSystemWatcher>
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
    void  init_par();
    void  sysinit();
    void  vi_parameter_enter(int position);
    bool eventFilter(QObject *, QEvent *);
    int currenteditpos;
    void ShowRadio(int );

public slots:

  void showtestresult(int num);
  void showdspstate(int state);
  void DisplayChangeValue(bool, float);
  void HelpDialogClose();
  void moveEvent(QMoveEvent *event);
  void CloseWidget();
  void GetTestState();
  void GetTeststate(int);
private slots:
  void on_pushButton_3_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_clicked();









  void setlineedittext(QString str);
  void clearkeypadflag();
  //void slotFileChanged(const QString& path);/*槽函数，处理下面fileWatcher的信号*/





  void on_tabWidget_currentChanged(int index);





  void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    thread1 *uartthread;
    QLineEdit *currentedit;
    keypads *pad;
    QFileSystemWatcher *fileWatcher;
    bool bMouse;

};

#endif // WIDGET_H
