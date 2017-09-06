#ifndef WARNING_H
#define WARNING_H

#include <QDialog>

namespace Ui
{
    class Warning;
}

class Warning : public QDialog
{
    Q_OBJECT

public:
    explicit Warning(QWidget *parent = 0);
    void Setwarning(int,QString);
    void updateUI();
    ~Warning();
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void WarningWindowClose();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Warning *ui;
};

#endif // WARNING_H
