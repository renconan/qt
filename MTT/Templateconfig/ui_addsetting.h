/********************************************************************************
** Form generated from reading UI file 'addsetting.ui'
**
** Created: Wed Dec 14 10:18:58 2016
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDSETTING_H
#define UI_ADDSETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Addsetting
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;

    void setupUi(QDialog *Addsetting)
    {
        if (Addsetting->objectName().isEmpty())
            Addsetting->setObjectName(QString::fromUtf8("Addsetting"));
        Addsetting->resize(397, 181);
        pushButton = new QPushButton(Addsetting);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(110, 130, 81, 41));
        pushButton_2 = new QPushButton(Addsetting);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(240, 130, 81, 41));
        verticalLayoutWidget = new QWidget(Addsetting);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 20, 81, 91));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        lineEdit = new QLineEdit(Addsetting);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(110, 20, 271, 31));
        lineEdit_2 = new QLineEdit(Addsetting);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(110, 50, 271, 31));
        lineEdit_3 = new QLineEdit(Addsetting);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(110, 80, 271, 31));

        retranslateUi(Addsetting);

        QMetaObject::connectSlotsByName(Addsetting);
    } // setupUi

    void retranslateUi(QDialog *Addsetting)
    {
        Addsetting->setWindowTitle(QApplication::translate("Addsetting", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Addsetting", "OK", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Addsetting", "Cancel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Addsetting", "name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Addsetting", "value:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Addsetting", "ID:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Addsetting: public Ui_Addsetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDSETTING_H
