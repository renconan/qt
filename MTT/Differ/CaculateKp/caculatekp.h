#ifndef CACULATEKP_H
#define CACULATEKP_H

#include <QDialog>
#include "Config.h"
#include "Defpam.h"
#include "keypads.h"
#include <QLineEdit>

namespace Ui {
    class CaculateKp;
}

class CaculateKp : public QDialog
{
    Q_OBJECT

public:
    explicit CaculateKp(QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);
    void SetVolchangerinfo(VOLCHANGERINFO tmpinfo);
    VOLCHANGERINFO VOLchanger;
    void updateUI();
    ~CaculateKp();

signals:
    void SIG_SendVolchangerinfo(void *);
    void SIG_SendClose();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void closeEvent(QCloseEvent *event);//πÿ±’∞¥≈•÷ÿ‘ÿ
    void setlineedittext(QString str);
   // void clearkeypadflag();

private:
    Ui::CaculateKp *ui;
     int currenteditpos;
     keypads *pad;
     QLineEdit *currentedit;
};

#endif // CACULATEKP_H
