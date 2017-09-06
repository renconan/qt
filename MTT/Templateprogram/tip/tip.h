#ifndef TIP_H
#define TIP_H

#include <QDialog>

namespace Ui {
    class Tip;
}

class Tip : public QDialog
{
    Q_OBJECT

public:
    explicit Tip(QWidget *parent = 0);
    ~Tip();
    void settip(int index);

private:
    Ui::Tip *ui;
};

#endif // TIP_H
