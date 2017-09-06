/********************************************************************************
** Form generated from reading UI file 'exportreport.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTREPORT_H
#define UI_EXPORTREPORT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ExportReport
{
public:
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QRadioButton *radioButton_3;
    QLabel *label_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;

    void setupUi(QDialog *ExportReport)
    {
        if (ExportReport->objectName().isEmpty())
            ExportReport->setObjectName(QString::fromUtf8("ExportReport"));
        ExportReport->resize(435, 300);
        label = new QLabel(ExportReport);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 140, 351, 41));
        pushButton = new QPushButton(ExportReport);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(50, 210, 91, 41));
        pushButton_2 = new QPushButton(ExportReport);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(255, 210, 91, 41));
        radioButton_3 = new QRadioButton(ExportReport);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(148, 103, 101, 21));
        label_2 = new QLabel(ExportReport);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(100, 15, 231, 26));
        radioButton = new QRadioButton(ExportReport);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(3, 103, 126, 21));
        radioButton_2 = new QRadioButton(ExportReport);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(275, 103, 146, 21));

        retranslateUi(ExportReport);

        QMetaObject::connectSlotsByName(ExportReport);
    } // setupUi

    void retranslateUi(QDialog *ExportReport)
    {
        ExportReport->setWindowTitle(QApplication::translate("ExportReport", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        pushButton->setText(QApplication::translate("ExportReport", "\346\211\247\350\241\214", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("ExportReport", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        radioButton_3->setText(QApplication::translate("ExportReport", "\345\210\240\351\231\244\346\212\245\345\221\212", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ExportReport", "TextLabel", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("ExportReport", "\345\257\274\345\207\272\346\212\245\345\221\212", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("ExportReport", "\346\233\264\346\226\260\347\250\213\345\272\217", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExportReport: public Ui_ExportReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTREPORT_H
