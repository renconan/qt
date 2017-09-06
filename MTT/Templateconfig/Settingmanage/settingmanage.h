#ifndef SETTINGMANAGE_H
#define SETTINGMANAGE_H
#include <QWidget>
#include "format.h"
#include "addsetting.h"

namespace Ui {
    class Settingmanage;
}

class Settingmanage : public QWidget
{
    Q_OBJECT

public:
    explicit Settingmanage(QWidget *parent = 0);
    ~Settingmanage();
    void getsettinglist(struct SETTINGNODELIST *list);
    bool checkidchonghe(QString ID); //检查ID是否重合


private slots:
    void on_pushButton_clicked();
    void updatesetting(QString,QString,QString);
    void on_pushButton_2_clicked();
    void copytoshuzu(char shuzu[],QString str,int len);
    void updateshow();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

signals:
    void savesettingnode(void *);  //保存定值的链表到主程序


private:
    Ui::Settingmanage *ui;
    struct SETTINGNODELIST *settingheader;
    Addsetting *addsetting;
    bool isedit;
    QString editID; //正在编辑的ID号

};

#endif // SETTINGMANAGE_H
