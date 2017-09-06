#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QDialog>

namespace Ui {
    class Warningdialog;
}

class Warningdialog : public QDialog
{
    Q_OBJECT

public:
    explicit Warningdialog(QWidget *parent = 0);
    ~Warningdialog();
    void showwarning(int num,float SystemVolt);

private:
    Ui::Warningdialog *ui;
};

#endif // WARNINGDIALOG_H
