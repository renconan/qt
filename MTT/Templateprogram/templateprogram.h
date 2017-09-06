#ifndef TEMPLATEPROGRAM_H
#define TEMPLATEPROGRAM_H

#include <QWidget>
#include "format.h"
#include "Defpam.h"
#include "thread1.h"
#include "settingmanage.h"
#include "tip.h"
#include "Keypad/Keypaddialog.h"
#include "caculate.h"
#include "CaculateVar.h"
#include "Help/helpdialog.h"
#include <sys/time.h>
//参数的界面
#include "VolPara/volpickup.h"
#include "VolPara/voltriptime.h"
#include "CurPara/curpickup.h"
#include "CurPara/curtriptime.h"
#include "PowerPara/powerpara.h"
#include "PhasePara/phasepickup.h"
#include "PhasePara/phasetriptime.h"
#include "HarmonicPara/harmonicpara.h" //2016-12-13
#include "FreqPara/freqpickup.h"
#include "FreqPara/freqtriptime.h"
#include "FreqPara/dfdtblocking.h"
#include "FreqPara/dfdtblockingtime.h"
#include "FreqPara/voltageblock.h"







namespace Ui {
    class Templateprogram;
}




class Templateprogram : public QWidget
{
    Q_OBJECT

public:
    explicit Templateprogram(QWidget *parent = 0);
    ~Templateprogram();

    int is_digit(char ch);
    void processrelerror(int type,int subtype,int relabs,float value,float value_2,struct SECTMENTSET &tmpsectset);
    void AnalysisAssesschar(char *assesschar,int type,int subtype,struct SECTMENTSET &sectsettmp);
    void ReadCurrentVariable(char *buf,struct SECTMENTSET &sectset,int type);
    void ReadVoltageVariable(char *buf,struct SECTMENTSET &sectset,int type);
    void ReadHarmonicVariable(char *buf,struct SECTMENTSET &sectset);//2016-12-13
    void ReadDirectionVariable(char *buf,struct SECTMENTSET &sectset);
    void ReadFrequencyVariable(char *buf,struct SECTMENTSET &sectset,int type);
    void ReadAnytestVariable(char *buf,struct SECTMENTSET &sectset);
    void ReadPhraseVariable(char *buf,struct SECTMENTSET &sectset,int type); //读相位测试的变量

    void settemplatefilename(char *filename); //设置模板的文件名
    void copytoshuzu(char shuzu[], QString str, int len);
    void updatesetting(char *filename); //读模板定值文件的所有ID name value
    void Processsection(char *filename);
    void Analyzetemplatefile(char *filename); //解析模板文件

    void showfirstpage(int sectid);
    void processpresect(); //按下上一段按钮的设置
    void processnextsect(); //按下下一段按钮的设置
    void processprepage();
    void processnextpage();
    void showprotection(int type,int sectmentid);   //显示某一保护类型的数据
    void temprorarydatainit();
    void UartsInit();
    void UartInit(int uartno);
    char templatefilename[512];
    int WriteAuthrizedFile();
    int TimeCounter;




protected slots:
    void ShowFileExpired();
    void returnsetlink(void *);
    void Closesectmange();
    void Closeshowpara();
    void GetNewSectset(void *value); //获取新的段设置
    void showtip(int num);
    void showfuncofsect();
    void getreportname(char *name);
    void closereportkeypad();
    void CloseWidget();
    void showcurrenttestpos(int);
    void cleartestitemcolor();
    void getthreadoutputstr(char *);
    void GetStarttest(int type);
    void HelpDialogClose();
    void moveEvent(QMoveEvent *event);
    void GetTestState();



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_3_clicked();

private:
    Ui::Templateprogram *ui;
    struct SECTMENTSETNODE *sectmentlink_Auto; //段的聊表
    struct SETTINGNODELIST *settinglink;  // 定值的聊表 ID NAME VALUE
    TEMPRORARYDATA *temprorarydata;
    thread1 *uartthread; //串口线程
    SettingManage  *settingmanage; //定值管理界面
    Tip *tip;
    KeyPadDialog *keypad;


};

#endif // TEMPLATEPROGRAM_H
