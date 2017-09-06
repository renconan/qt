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
    void readtemplatefile(); //�����ص�ģ���ļ�
    unsigned   int TemplateCrcNum(char *Filename);
    int  CheckAuthrizeFile(char *templatefile);
    bool CheckSerailNumber(char *templatefile);  //����������Ƿ�ƥ��
    void SetSerialNum(char *);

    char SerialNumber[512];
    struct NODELINK *templatelink;  //ģ����Թ��ܵ��ı�
    long AuthorizeTime; //��Ȩʱ��
    long ExpireTime;     //�Ѿ�ʹ��ʱ��

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
    void moveEvent(QMoveEvent *event);



private:
    Ui::Import *ui;
  //  Importthread *importthread;
};

#endif // IMPORT_H
