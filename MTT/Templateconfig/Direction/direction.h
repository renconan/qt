#ifndef DIRECTION_H
#define DIRECTION_H

#include <QWidget>
#include "format.h"

namespace Ui {
    class Direction;
}

class Direction : public QWidget
{
    Q_OBJECT

public:
    explicit Direction(QWidget *parent = 0);
    ~Direction();
    bool settingover;
    void SetDirectionset(DIRECTIONSET set);
    void copytoshuzu(char shuzu[],QString str,int len);
    void setsectmentset(struct SECTMENTSET &set);
     struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
     DIRECTIONSET directionset;
     struct SETTINGNODELIST *settinglink; //所有的定值链表
signals:
    void sendsettingvalue(void *value,int type);
    void sendclose(int type); //发送信号关闭 这个界面 ，让编辑状态置为false
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::Direction *ui;
protected:
    void closeEvent(QCloseEvent *event);


};

#endif // DIRECTION_H
