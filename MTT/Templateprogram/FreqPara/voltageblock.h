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
void  Sendpara(void *); //��һ�������õ�ѹ�����ã�int �ǵ�ѹ���� ��������


private slots:
void on_pushButton_clicked();
void on_pushButton_2_clicked();

private:
    Ui::VoltageBlock *ui;
    struct SETTINGNODELIST *settinglink;
    struct SECTMENTSET Sectvalue; //��ǰ�ε�ֵ
    QLineEdit *currentedit;
};

#endif // VOLTAGEBLOCK_H
