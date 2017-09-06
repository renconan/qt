#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QWidget>
#include "format.h"

namespace Ui {
    class Frequency;
}

class Frequency : public QWidget
{
    Q_OBJECT

public:
    explicit Frequency(QWidget *parent = 0);
    ~Frequency();
    bool settingover;
    void Setfrequencyset(FRECURRENCESET set);
    void copytoshuzu(char shuzu[],QString str,int len);
    struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
    FRECURRENCESET freset;
    void setsectmentset(struct SECTMENTSET &set);
    struct SETTINGNODELIST *settinglink; //所有的定值链表

signals:
    void sendsettingvalue(void *value,int type);
     void sendclose(int type); //发送信号关闭 这个界面 ，让编辑状态置为false

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();



private:
    Ui::Frequency *ui;
protected:
    void closeEvent(QCloseEvent *event);


};

#endif // FREQUENCY_H
