#ifndef VOLTAGEBLOCK_H
#define VOLTAGEBLOCK_H

#include <QDialog>
#include "format.h"
#include "Config.h"
#include "Defpam.h"
#include "CaculateVar.h"
#include "nodeprocess.h"
#include <QDialog>
#include <QLineEdit>
#include <Keypad/Keypaddialog.h>

namespace Ui {
    class VoltageBlock;
}

class VoltageBlock : public QDialog
{
    Q_OBJECT

public:
    explicit VoltageBlock(QWidget *parent = 0);
    ~VoltageBlock();
    bool eventFilter(QObject *obj, QEvent *event);
    void  Setshowpara(struct SECTMENTSET value);
    void  SetSettingLink(struct SETTINGNODELIST *link);
    void  copytoshuzu(char shuzu[], QString str, int len);
    struct SECTMENTSET Sectset;


protected slots:
  void setlineedittext(char *str);
  void clearkeypadflag();


signals:
void  sendflag();
void  Sendpara(void *); //第一个参数该电压的设置，int 是电压还是 电流还是


private slots:
void on_pushButton_clicked();
void on_pushButton_2_clicked();

private:
    Ui::VoltageBlock *ui;
    struct SETTINGNODELIST *settinglink;
    struct SECTMENTSET Sectvalue; //当前段的值
    QLineEdit *currentedit;
};

#endif // VOLTAGEBLOCK_H
