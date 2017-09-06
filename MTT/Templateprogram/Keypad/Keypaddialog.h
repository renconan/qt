#ifndef KEYPADDIALOG_H
#define KEYPADDIALOG_H

#include <QDialog>

namespace Ui {
    class KeyPadDialog;
}

class KeyPadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyPadDialog(QWidget *parent = 0);
    ~KeyPadDialog();
  void initkeyshow(bool shift,bool tab);
  void SetText(QString text);

  bool Caps_on;
  bool Shift_on;
  char Textchar[1024];



  int Textcharlen;



private:
    Ui::KeyPadDialog *ui;

protected slots:

    void updateshow(bool,bool); //更新显示
    void keyboardclose();       //键盘关闭
    void dealdelete();          // 处理删除键
    void getkey(char key);      //获得键值
    void closeEvent(QCloseEvent *e);

signals:
    void SIG_sendtext(char *text);
    void SIG_closeflag();
private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_5_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_7_clicked();

    void on_toolButton_8_clicked();

    void on_toolButton_9_clicked();

    void on_toolButton_10_clicked();

    void on_toolButton_11_clicked();

    void on_toolButton_12_clicked();

    void on_toolButton_13_clicked();

    void on_toolButton_14_clicked();

    void on_toolButton_15_clicked();

    void on_toolButton_16_clicked();

    void on_toolButton_17_clicked();

    void on_toolButton_18_clicked();

    void on_toolButton_19_clicked();

    void on_toolButton_20_clicked();

    void on_toolButton_21_clicked();

    void on_toolButton_22_clicked();

    void on_toolButton_23_clicked();

    void on_toolButton_24_clicked();

    void on_toolButton_25_clicked();

    void on_toolButton_26_clicked();

    void on_toolButton_27_clicked();

    void on_toolButton_28_clicked();

    void on_toolButton_29_clicked();

    void on_toolButton_30_clicked();

    void on_toolButton_31_clicked();

    void on_toolButton_32_clicked();

    void on_toolButton_33_clicked();

    void on_toolButton_34_clicked();

    void on_toolButton_35_clicked();

    void on_toolButton_36_clicked();

    void on_toolButton_37_clicked();

    void on_toolButton_38_clicked();

    void on_toolButton_39_clicked();

    void on_toolButton_40_clicked();

    void on_toolButton_41_clicked();

    void on_toolButton_42_clicked();

    void on_toolButton_43_clicked();

    void on_toolButton_44_clicked();

    void on_toolButton_45_clicked();

    void on_toolButton_46_clicked();

    void on_toolButton_47_clicked();

    void on_toolButton_48_clicked();

    void on_toolButton_49_clicked();

    void on_toolButton_50_clicked();

    void on_toolButton_51_clicked();

    void on_toolButton_52_clicked();




};

#endif // DIALOG_H
