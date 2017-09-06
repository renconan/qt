#ifndef PHRASE_H
#define PHRASE_H

#include <QWidget>
#include "format.h"

namespace Ui {
    class Phrase;
}

class Phrase : public QWidget
{
    Q_OBJECT

public:
    explicit Phrase(QWidget *parent = 0);
    ~Phrase();
    bool settingover;
    void Setphraseset(PHRASESET set);
    void copytoshuzu(char shuzu[],QString str,int len);
    void setsectmentset(struct SECTMENTSET &set);
    struct SECTMENTSET sectmentset;  //段的定义， 可以添加些自定义的功能
    PHRASESET phraseset;
    struct SETTINGNODELIST *settinglink; //所有的定值链表
signals:
    void sendsettingvalue(void *value,int type);
    void sendclose(int type); //发送信号关闭 这个界面 ，让编辑状态置为false
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_checkBox_clicked();

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::Phrase *ui;
};

#endif // PHRASE_H
