#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

namespace Ui {
    class Helpdialog;
}

class Helpdialog : public QDialog
{
    Q_OBJECT

public:
    explicit Helpdialog(QWidget *parent = 0);
    ~Helpdialog();
    void SetHelpfilename(char *filename);

signals:
    void HelpWindowClose(); //���ʹ��ڹر��ź�

private slots:
    void on_pushButton_clicked();

private:
    Ui::Helpdialog *ui;
};

#endif // HELPDIALOG_H
