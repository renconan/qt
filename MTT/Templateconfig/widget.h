#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>
//测试模块的文件
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

     bool iseditting; //正在编辑
     int currentid;
     int  sumsectnum;

     struct  AUTHORIEDFILEINFO authorizedfileinfo; // 授权文件的 信息

     struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
     void copytoshuzu(char shuzu[],QString str,int len);
     void updatesetting(char *filename);

     int is_digit(char ch);
     bool checkvalue(char  *str);
     void CreatAuthorizeFile(char *Filename);

     void updatesectionlist(); //更新段的列表显示
     void updatefunclist();  //更新一段里面的测试功能显示


     void Writetestfunc(struct SECTMENTSETNODE *node,FILE *fp);

     void processrelerror(int type,int subtype,int relabs,float value,float value_2,struct SECTMENTSET &tmpsectset);
     void AnalysisAssesschar(char *assesschar,int type,int subtype,struct SECTMENTSET &sectsettmp);
     void ReadCurrentVariable(char *buf,struct SECTMENTSET &sectset,int type); //读电流的变量
     void ReadVoltageVariable(char *buf,struct SECTMENTSET &sectset,int type); //读电压的变量
     void ReadDirectionVariable(char *buf,struct SECTMENTSET &sectset); //读方向的变量
     void ReadFrequencyVariable(char *buf,struct SECTMENTSET &sectset,int type); //读频率的变量
     void ReadAnytestVariable(char *buf,struct SECTMENTSET &sectset); //读任意测试的变量
     void ReadPhraseVariable(char *buf,struct SECTMENTSET &sectset,int type); //读相位测试的变量
     void ReadHarmonicVariable(char *buf,struct SECTMENTSET &sectset);//读谐波的变量


     void Processsection(char *filename);
     void Analyzetemplatefile(char *filename);
     int CheckAuthrizeFile(char *templatefile); //分析授权文件


     float RelaceExp(char *exp); //替换 式中的ID
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
    void closefuncwin(int type); //关闭测试程序的 窗口
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
    Voltageset *volset;  //电压设置
    Currentset *curset;  //电流设置
    Direction *dirset;   //方向设置的界面
    Frequency *freset;   //频率设置界面
    Anytest   *anytestset; //任意测试的界面
    Phrase    *phraseset;
    Harmonic    *harmonicset;

    struct SECTMENTSETNODE *nodelink;
    struct SETTINGNODELIST *settinglink; //所有的定值链表
    Settingmanage *settingmanage;



    bool AuthorizedFileValid;

};

#endif // WIDGET_H
