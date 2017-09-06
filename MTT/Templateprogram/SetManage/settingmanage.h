#ifndef SETTINGMANAGE_H
#define SETTINGMANAGE_H

#include <QWidget>
#include "format.h"
#include "Defpam.h"
#include "dialog.h"
#define num_per_row   10   //每页setting的个数



namespace Ui {
    class SettingManage;
}

class SettingManage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingManage(QWidget *parent = 0);
    ~SettingManage();
    int settingnum; //定值的个数
    int settingnum_of_page; //当前页的定值个数的个数
    int currentpage;       //当前显示第几页
    int sumpage;       //总的页数
    int page_remainder; //剩余页
    void Showinit();//显示初始化
    void showprepage();
    void shownextpage();
    void Setsettinglink(struct SETTINGNODELIST *link);
    void SetNewsettinglink(struct SETTINGNODELIST *link1,struct SETTINGNODEIDLIST *link2);
    int  keyboardpos;
    bool keypadon;


protected slots:



    void saveclosewin();
    void cancellclosewin(); //取消退出
    void dissettingvalue(QString value);
    void clearkeypadflag();
signals:
    void SIG_returnsetlink(void *);
    void SIG_Close();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();



    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::SettingManage *ui;
    struct SETTINGNODEIDLIST  *settinglink;  // 定值的聊表 ID NAME VALUE 添加了index
    struct SETTINGNODELIST    *settingnodelink;  // 用于将退出后重新设置模板界面的定值链表

    Dialog *keyboad; //键盘

};

#endif // SETTINGMANAGE_H
