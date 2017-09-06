/********************************************************************************
** Form generated from reading UI file 'helpdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPDIALOG_H
#define UI_HELPDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Helpdialog
{
public:
    QTextEdit *textEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *Helpdialog)
    {
        if (Helpdialog->objectName().isEmpty())
            Helpdialog->setObjectName(QString::fromUtf8("Helpdialog"));
        Helpdialog->resize(800, 480);
        textEdit = new QTextEdit(Helpdialog);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(0, 0, 801, 431));
        pushButton = new QPushButton(Helpdialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(350, 430, 91, 41));

        retranslateUi(Helpdialog);

        QMetaObject::connectSlotsByName(Helpdialog);
    } // setupUi

    void retranslateUi(QDialog *Helpdialog)
    {
        Helpdialog->setWindowTitle(QApplication::translate("Helpdialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Helpdialog", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Helpdialog: public Ui_Helpdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPDIALOG_H
