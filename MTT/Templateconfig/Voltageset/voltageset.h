#ifndef VOLTAGESET_H
#define VOLTAGESET_H

#include <QDialog>
#include <format.h>

namespace Ui {
    class Voltageset;
}

class Voltageset : public QDialog
{
    Q_OBJECT

public:
    explicit Voltageset(QWidget *parent = 0);
    ~Voltageset();
     bool settingover;
     void Setvoltageset(VOLTAGESET voltageset);
     void copytoshuzu(char shuzu[],QString str,int len);
     void setsectmentset(struct SECTMENTSET &set);
     struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
     VOLTAGESET  voltageset;
     struct SETTINGNODELIST *settinglink; //所有的定值链表
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_checkBox_5_clicked();
signals:
     void sendsettingvalue(void *value,int type);
     void sendclose(int type); //发送信号关闭 这个界面 ，让编辑状态置为false

private:
    Ui::Voltageset *ui;
protected:
    void closeEvent(QCloseEvent *event);

};
#endif // VOLTAGESET_H
