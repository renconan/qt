#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "Defpam.h"

class threadkeyboard;
namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setorignaltext(QString text);


private slots:
    void on_toolButton_enter_clicked();
    void on_toolButton_clear_clicked();
    void on_toolButton_back_clicked();
    void on_toolButton_point_clicked();
    void on_toolButton_sign_clicked();
    void on_toolButton_0_clicked();
    void on_toolButton_1_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_6_clicked();
    void on_toolButton_7_clicked();
    void on_toolButton_8_clicked();
    void on_toolButton_9_clicked();
signals:
        void sendvalue(const QString &);
        void sendflag();
private:
    Ui::Dialog *ui;
    void closeEvent(QCloseEvent *event);//关闭按钮重载
    void init();
    bool firstflag;//是否打开，保证只能打开一次


};

#endif // DIALOG_H
