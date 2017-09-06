#ifndef SETTINGMANAGE_H
#define SETTINGMANAGE_H

#include <QWidget>
#include "format.h"
#include "Defpam.h"
#include "dialog.h"
#define num_per_row   10   //ÿҳsetting�ĸ���



namespace Ui {
    class SettingManage;
}

class SettingManage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingManage(QWidget *parent = 0);
    ~SettingManage();
    int settingnum; //��ֵ�ĸ���
    int settingnum_of_page; //��ǰҳ�Ķ�ֵ�����ĸ���
    int currentpage;       //��ǰ��ʾ�ڼ�ҳ
    int sumpage;       //�ܵ�ҳ��
    int page_remainder; //ʣ��ҳ
    void Showinit();//��ʾ��ʼ��
    void showprepage();
    void shownextpage();
    void Setsettinglink(struct SETTINGNODELIST *link);
    void SetNewsettinglink(struct SETTINGNODELIST *link1,struct SETTINGNODEIDLIST *link2);
    int  keyboardpos;
    bool keypadon;


protected slots:



    void saveclosewin();
    void cancellclosewin(); //ȡ���˳�
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
    struct SETTINGNODEIDLIST  *settinglink;  // ��ֵ���ı� ID NAME VALUE �����index
    struct SETTINGNODELIST    *settingnodelink;  // ���ڽ��˳�����������ģ�����Ķ�ֵ����

    Dialog *keyboad; //����

};

#endif // SETTINGMANAGE_H
