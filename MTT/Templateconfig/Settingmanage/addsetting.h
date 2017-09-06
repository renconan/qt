#ifndef ADDSETTING_H
#define ADDSETTING_H

#include <QDialog>
namespace Ui {
    class Addsetting;
}

class Addsetting : public QDialog
{
    Q_OBJECT

public:
    explicit Addsetting(QWidget *parent = 0);
    ~Addsetting();
    bool checkid(QString str);
    bool checkvalue(QString str);
    void setsetting(QString name,QString value,QString ID);
   int is_digit( char ch);
   int is_space( char ch);


signals:
   void SIG_sendsetting(QString name,QString value,QString ID);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Addsetting *ui;

};

#endif // ADDSETTING_H
