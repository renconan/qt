#ifndef EXPORTREPORT_H
#define EXPORTREPORT_H

#include <QDialog>
namespace Ui {
    class ExportReport;
}

class ExportReport : public QDialog
{
    Q_OBJECT

public:
    explicit ExportReport(QWidget *parent = 0);
    ~ExportReport();
    int TestSpace(char *dir);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

private:
    Ui::ExportReport *ui;
};

#endif // EXPORTREPORT_H
