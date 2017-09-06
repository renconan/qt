/********************************************************************************
** Form generated from reading UI file 'settingmanage.ui'
**
** Created: Thu Oct 30 14:26:10 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGMANAGE_H
#define UI_SETTINGMANAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settingmanage
{
public:
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;

    void setupUi(QWidget *Settingmanage)
    {
        if (Settingmanage->objectName().isEmpty())
            Settingmanage->setObjectName(QString::fromUtf8("Settingmanage"));
        Settingmanage->resize(535, 561);
        tableWidget = new QTableWidget(Settingmanage);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 541, 491));
        pushButton = new QPushButton(Settingmanage);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 510, 75, 31));
        pushButton_2 = new QPushButton(Settingmanage);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 510, 75, 31));
        pushButton_3 = new QPushButton(Settingmanage);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(210, 510, 75, 31));
        pushButton_4 = new QPushButton(Settingmanage);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(310, 510, 71, 31));
        pushButton_5 = new QPushButton(Settingmanage);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(410, 510, 81, 31));

        retranslateUi(Settingmanage);

        QMetaObject::connectSlotsByName(Settingmanage);
    } // setupUi

    void retranslateUi(QWidget *Settingmanage)
    {
        Settingmanage->setWindowTitle(QApplication::translate("Settingmanage", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Settingmanage", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Settingmanage", "Edit", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Settingmanage", "Delete", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("Settingmanage", "OK", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("Settingmanage", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Settingmanage: public Ui_Settingmanage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGMANAGE_H
