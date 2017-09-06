#ifndef THREAD1_H
#define THREAD1_H
#include <QThread>
#include <QProcess>


#include <QTimer>
#include "Defpam.h"
#include "Config.h"
#include "KEY.h"
#include "format.h"
#include "caculate.h"
#include "CaculateVar.h"



class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void run();
    void key_response(KEY key);  //需要更改
    void key_deal_enter_current(int &position);

    void settemprorarydata(TEMPRORARYDATA *value);
    void setsumfucos(int num);
    void key_deal_test();
    void clearallresult();
    void key_deal_stop();

    void writeanytestsettingfile(ANYTESTSET set); //写任意测试的定值文件
    void setsectmentnodelink(struct SECTMENTSETNODE *set); //设置段的节点链表
    void setsettingnodelink(struct SETTINGNODELIST *set);  //设置定值的链表

    void Writetestfunc(struct SECTMENTSETNODE *node,FILE *fp);
    void creatReportLink();
    void savereport(); //保存报告

    void stoprun();




protected slots:
    void getprocessstr2();
    void getprocessstr();


signals:
    void SIG_updatepos(int pos); //更新位置
    void SIG_enterpos(int pos); //按下确定键的位置
    void SIG_showtip(int flag); //  0关闭提示   1：提示1 2:提示2 3：
    void SIG_showfirstfuncofsect();//显示段的第一个测试功能
    void SIG_showcurrenttestpos(int); //将当前的测试项的背景色
    void SIG_cleartestitemcolor();  // 清楚所有测试项的背景色
    void SIG_SendProcessStr(char *str);
    void SIG_AuthorizedFileExpire();
     void   SIG_SendClose();

      void   SIG_TestState();



private:
    int currentpos;
    TEMPRORARYDATA *temprorarydata;
    int sumfucos;  //总的能获得焦点个数
    struct SECTMENTSETNODE *sectmentnodelink; //段的聊表
    struct SETTINGNODELIST *settingnodelink;  // 定值的聊表 ID NAME VALUE
    struct REPORTLINK  *ReportLink;
    QProcess *proc;




};

#endif // THREAD1_H
