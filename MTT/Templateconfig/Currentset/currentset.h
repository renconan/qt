#ifndef CURRENTSET_H
#define CURRENTSET_H

#include <QDialog>
#include <format.h>

namespace Ui {
    class Currentset;
}

class Currentset : public QDialog
{
    Q_OBJECT

public:
    explicit Currentset(QWidget *parent = 0);
    ~Currentset();
    bool settingover;
    void Setcurrentset(CURRENTSET set);
    void copytoshuzu(char shuzu[],QString str,int len);
    void setsectmentset(struct SECTMENTSET &set);
    struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
    CURRENTSET currentset;
    struct SETTINGNODELIST *settinglink; //所有的定值链表
signals:
    void sendsettingvalue(void *value,int type);
    void sendclose(int type); //发送信号关闭 这个界面 ，让编辑状态置为false
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::Currentset *ui;
protected:
    void closeEvent(QCloseEvent *event);

};

#endif // CURRENTSET_H
