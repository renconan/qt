#ifndef KEYPADS_H
#define KEYPADS_H

#include <QDialog>

namespace Ui {
    class keypads;
}

class keypads : public QDialog
{
    Q_OBJECT

public:
    explicit keypads(QWidget *parent = 0);
    ~keypads();
    void setdata(QString str);

private:
    Ui::keypads *ui;
    void closeEvent(QCloseEvent *event);//关闭按钮重载
    void init();
    bool firstflag;//是否打开，保证只能打开一次
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

};

#endif // KEYPADS_H
