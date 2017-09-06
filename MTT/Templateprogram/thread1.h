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
    void key_response(KEY key);  //��Ҫ����
    void key_deal_enter_current(int &position);

    void settemprorarydata(TEMPRORARYDATA *value);
    void setsumfucos(int num);
    void key_deal_test();
    void clearallresult();
    void key_deal_stop();

    void writeanytestsettingfile(ANYTESTSET set); //д������ԵĶ�ֵ�ļ�
    void setsectmentnodelink(struct SECTMENTSETNODE *set); //���öεĽڵ�����
    void setsettingnodelink(struct SETTINGNODELIST *set);  //���ö�ֵ������

    void Writetestfunc(struct SECTMENTSETNODE *node,FILE *fp);
    void creatReportLink();
    void savereport(); //���汨��

    void stoprun();




protected slots:
    void getprocessstr2();
    void getprocessstr();


signals:
    void SIG_updatepos(int pos); //����λ��
    void SIG_enterpos(int pos); //����ȷ������λ��
    void SIG_showtip(int flag); //  0�ر���ʾ   1����ʾ1 2:��ʾ2 3��
    void SIG_showfirstfuncofsect();//��ʾ�εĵ�һ�����Թ���
    void SIG_showcurrenttestpos(int); //����ǰ�Ĳ�����ı���ɫ
    void SIG_cleartestitemcolor();  // ������в�����ı���ɫ
    void SIG_SendProcessStr(char *str);
    void SIG_AuthorizedFileExpire();
     void   SIG_SendClose();

      void   SIG_TestState();



private:
    int currentpos;
    TEMPRORARYDATA *temprorarydata;
    int sumfucos;  //�ܵ��ܻ�ý������
    struct SECTMENTSETNODE *sectmentnodelink; //�ε��ı�
    struct SETTINGNODELIST *settingnodelink;  // ��ֵ���ı� ID NAME VALUE
    struct REPORTLINK  *ReportLink;
    QProcess *proc;




};

#endif // THREAD1_H
