/********************************************************************************
** Form generated from reading UI file 'warning.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WARNING_H
#define UI_WARNING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Warning
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;

    void setupUi(QDialog *Warning)
    {
        if (Warning->objectName().isEmpty())
            Warning->setObjectName(QString::fromUtf8("Warning"));
        Warning->resize(397, 267);
        label = new QLabel(Warning);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 90, 356, 51));
        label_2 = new QLabel(Warning);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 175, 501, 51));
        pushButton = new QPushButton(Warning);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(155, 195, 75, 31));

        retranslateUi(Warning);

        QMetaObject::connectSlotsByName(Warning);
    } // setupUi

    void retranslateUi(QDialog *Warning)
    {
        Warning->setWindowTitle(QApplication::translate("Warning", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QString());
        pushButton->setText(QApplication::translate("Warning", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Warning: public Ui_Warning {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WARNING_H
