#ifndef ADDHARMONIC_H
#define ADDHARMONIC_H

#include <QWidget>
#include "Config.h"
namespace Ui {
    class AddHarmonic;
}

class AddHarmonic : public QWidget
{
    Q_OBJECT

public:
    explicit AddHarmonic(QWidget *parent = 0);
    ~AddHarmonic();

private:
    Ui::AddHarmonic *ui;
};

#endif // ADDHARMONIC_H
