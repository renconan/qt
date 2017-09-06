#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>
//����ģ����ļ�
#include "Currentset/currentset.h"
#include "Voltageset/voltageset.h"
#include "Direction/direction.h"
#include "Frequency/frequency.h"
#include "Anytest/anytest.h"
#include "Settingmanage/settingmanage.h"
#include "Phase/phrase.h"
#include "Harmonic/harmonic.h"
#include "caculate.h"
#include "JiaJieXor.h"
#include <QCloseEvent>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
     int currentsectfunc;

     bool iseditting; //���ڱ༭
     int currentid;
     int  sumsectnum;

     struct  AUTHORIEDFILEINFO authorizedfileinfo; // ��Ȩ�ļ��� ��Ϣ

     struct SECTMENTSET sectmentset;  //�εĶ��壬 �������Щ�Զ���Ĺ���
     void copytoshuzu(char shuzu[],QString str,int len);
     void updatesetting(char *filename);

     int is_digit(char ch);
     bool checkvalue(char  *str);
     void CreatAuthorizeFile(char *Filename);

     void updatesectionlist(); //���¶ε��б���ʾ
     void updatefunclist();  //����һ������Ĳ��Թ�����ʾ


     void Writetestfunc(struct SECTMENTSETNODE *node,FILE *fp);

     void processrelerror(int type,int subtype,int relabs,float value,float value_2,struct SECTMENTSET &tmpsectset);
     void AnalysisAssesschar(char *assesschar,int type,int subtype,struct SECTMENTSET &sectsettmp);
     void ReadCurrentVariable(char *buf,struct SECTMENTSET &sectset,int type); //�������ı���
     void ReadVoltageVariable(char *buf,struct SECTMENTSET &sectset,int type); //����ѹ�ı���
     void ReadDirectionVariable(char *buf,struct SECTMENTSET &sectset); //������ı���
     void ReadFrequencyVariable(char *buf,struct SECTMENTSET &sectset,int type); //��Ƶ�ʵı���
     void ReadAnytestVariable(char *buf,struct SECTMENTSET &sectset); //��������Եı���
     void ReadPhraseVariable(char *buf,struct SECTMENTSET &sectset,int type); //����λ���Եı���
     void ReadHarmonicVariable(char *buf,struct SECTMENTSET &sectset);//��г���ı���


     void Processsection(char *filename);
     void Analyzetemplatefile(char *filename);
     int CheckAuthrizeFile(char *templatefile); //������Ȩ�ļ�


     float RelaceExp(char *exp); //�滻 ʽ�е�ID
     void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl);
     void CaculateVariable(VARIABLE &exp);



private slots:
    void on_comboBox_currentIndexChanged(int index);
    void addtestfunc(void *,int);
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_10_clicked();
    void updatesettinglink(void *);
    void closefuncwin(int type); //�رղ��Գ���� ����
    void on_comboBox_activated(int index);
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_11_clicked();





    void on_listWidget_2_itemDoubleClicked(QListWidgetItem* item);

    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);

    void on_listWidget_2_customContextMenuRequested(const QPoint &pos);
    void UpSeedSlot();
    void DownSeedsSlot();

protected:
    void closeEvent(QCloseEvent *);


private:
    Ui::Widget *ui;
    Voltageset *volset;  //��ѹ����
    Currentset *curset;  //��������
    Direction *dirset;   //�������õĽ���
    Frequency *freset;   //Ƶ�����ý���
    Anytest   *anytestset; //������ԵĽ���
    Phrase    *phraseset;
    Harmonic    *harmonicset;

    struct SECTMENTSETNODE *nodelink;
    struct SETTINGNODELIST *settinglink; //���еĶ�ֵ����
    Settingmanage *settingmanage;



    bool AuthorizedFileValid;

};

#endif // WIDGET_H
