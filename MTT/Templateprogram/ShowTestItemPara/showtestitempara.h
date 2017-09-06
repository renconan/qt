#ifndef SHOWTESTITEMPARA_H
#define SHOWTESTITEMPARA_H
#include "format.h"
#include "Config.h"
#include "Defpam.h"
#include "CaculateVar.h"
#include "nodeprocess.h"


#include <QDialog>

namespace Ui {
    class ShowTestitemPara;
}

class ShowTestitemPara : public QDialog
{
    Q_OBJECT

public:
    explicit ShowTestitemPara(QWidget *parent = 0);
    void  Setshowpara(struct SECTMENTSET value,int type,int item);
    void  SetSettingLink(struct SETTINGNODELIST *link);

    void ShowVoltagePara(int type);

    void ShowCurrentPara(int type);

    void ShowPowerPara();

    void ShowFreqPara(int type);

    void ShowPhasePara(int type);

    void ShowHarmonicPara();



    ~ShowTestitemPara();

private slots:
    void on_pushButton_clicked();


signals:

    void sendflag();


protected slots:
    void closeEvent(QCloseEvent *);

private:
    Ui::ShowTestitemPara *ui;
    struct SECTMENTSET Sectvalue; //当前段的值
    int Protecttype; //保护类型
    int CurrentItem; //当前测试项目

    struct SETTINGNODELIST *settinglink;
};

#endif // SHOWTESTITEMPARA_H
