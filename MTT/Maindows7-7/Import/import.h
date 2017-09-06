#ifndef IMPORT_H
#define IMPORT_H

#include <QDialog>
#include "Defpam.h"
#include <QListWidgetItem>
#include "Import/importthread.h"

namespace Ui {
    class Import;
}

class Import : public QDialog
{
    Q_OBJECT

public:
    explicit Import(QWidget *parent = 0);
    ~Import();
    void jiamiXor(char *str, char key2,char *result);
    void jiemiXor(char *str, char key2,char *result);
    unsigned long Check(char *str);
    void readtemplatefile(); //读本地的模板文件
    unsigned   int TemplateCrcNum(char *Filename);
    int  CheckAuthrizeFile(char *templatefile);
    bool CheckSerailNumber(char *templatefile);  //检查是序列是否匹配
    void SetSerialNum(char *);

    char SerialNumber[512];
    struct NODELINK *templatelink;  //模板测试功能的聊表
    long AuthorizeTime; //授权时间
    long ExpireTime;     //已经使用时间

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
    void moveEvent(QMoveEvent *event);



private:
    Ui::Import *ui;
  //  Importthread *importthread;
};

#endif // IMPORT_H
