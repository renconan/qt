#ifndef FREQPICKUP_H
#define FREQPICKUP_H

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
    class FreqPickup;
}

class FreqPickup : public QDialog
{
    Q_OBJECT

public:
    explicit FreqPickup(QWidget *parent = 0);
    ~FreqPickup();
    bool eventFilter(QObject *obj, QEvent *event);
    void  Setshowpara(struct SECTMENTSET value);
    void  SetSettingLink(struct SETTINGNODELIST *link);
    void  copytoshuzu(char shuzu[], QString str, int len);
    struct SECTMENTSET Sectset;


signals:
  void  sendflag();
  void  Sendpara(void *); //��һ�������õ�ѹ�����ã�int �ǵ�ѹ���� ��������


protected slots:
  void setlineedittext(char *str);
  void clearkeypadflag();




private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
    Ui::FreqPickup *ui;
    struct SETTINGNODELIST *settinglink;
    struct SECTMENTSET Sectvalue; //��ǰ�ε�ֵ
    QLineEdit *currentedit;
};

#endif // FREQPICKUP_H