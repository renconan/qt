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
        Warning->resize(502, 288);
        label = new QLabel(Warning);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 50, 501, 51));
        label_2 = new QLabel(Warning);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 110, 501, 51));
        label_2->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(Warning);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(200, 250, 75, 31));

        retranslateUi(Warning);

        QMetaObject::connectSlotsByName(Warning);
    } // setupUi

    void retranslateUi(QDialog *Warning)
    {
        Warning->setWindowTitle(QApplication::translate("Warning", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QApplication::translate("Warning", "\350\257\267\351\207\215\346\226\260\350\256\276\347\275\256\346\255\245\351\225\277\345\222\214\346\255\245\346\225\260", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Warning", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Warning: public Ui_Warning {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WARNING_H
