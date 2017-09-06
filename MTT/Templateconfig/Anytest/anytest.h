#ifndef ANYTEST_H
#define ANYTEST_H

#include <QWidget>
#include "format.h"

namespace Ui {
    class Anytest;
}

class Anytest : public QWidget
{
    Q_OBJECT

public:
    explicit Anytest(QWidget *parent = 0);
    ~Anytest();
    bool settingover;
    void Setanytestset(ANYTESTSET set);
    void copytoshuzu(char shuzu[],QString str,int len);
    void setsectmentset(struct SECTMENTSET &set);
    struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
    ANYTESTSET anytestset;

signals:
     void sendsettingvalue(void *value,int type);
     void sendclose(int type); //发送信号关闭 这个界面 ，让编辑状态置为false

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Anytest *ui;
protected:
    void closeEvent(QCloseEvent *event);



};

#endif // ANYTEST_H
