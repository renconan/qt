#ifndef HARMONIC_H
#define HARMONIC_H

#include <QWidget>
#include "format.h"

namespace Ui {
    class Harmonic;
}

class Harmonic : public QWidget
{
    Q_OBJECT

public:
    explicit Harmonic(QWidget *parent = 0);
    ~Harmonic();
    bool settingover;
    void Setharmonicset(HARMONICSET set);
    void copytoshuzu(char shuzu[],QString str,int len);
    void setsectmentset(struct SECTMENTSET &set);
    struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
    HARMONICSET harmonicset;
    struct SETTINGNODELIST *settinglink; //所有的定值链表
signals:
    void sendsettingvalue(void *value,int type);
    void sendclose(int type); //发送信号关闭 这个界面 ，让编辑状态置为false
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();


protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::Harmonic *ui;
};

#endif // HARMONIC_H
