#ifndef VOLTAGESET_H
#define VOLTAGESET_H

#include <QDialog>
#include <format.h>

namespace Ui {
    class Voltageset;
}

class Voltageset : public QDialog
{
    Q_OBJECT

public:
    explicit Voltageset(QWidget *parent = 0);
    ~Voltageset();
     bool settingover;
     void Setvoltageset(VOLTAGESET *voltageset);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
signals:
    void sendsettingvalue(void *value,int type);

private:
    Ui::Voltageset *ui;
    VOLTAGESET *voltageset;
};

#endif // VOLTAGESET_H
