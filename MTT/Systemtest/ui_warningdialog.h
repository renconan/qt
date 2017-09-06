/********************************************************************************
** Form generated from reading UI file 'warningdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WARNINGDIALOG_H
#define UI_WARNINGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Warningdialog
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *Warningdialog)
    {
        if (Warningdialog->objectName().isEmpty())
            Warningdialog->setObjectName(QString::fromUtf8("Warningdialog"));
        Warningdialog->resize(607, 376);
        label = new QLabel(Warningdialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 70, 591, 41));
        label_2 = new QLabel(Warningdialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 140, 601, 41));

        retranslateUi(Warningdialog);

        QMetaObject::connectSlotsByName(Warningdialog);
    } // setupUi

    void retranslateUi(QDialog *Warningdialog)
    {
        Warningdialog->setWindowTitle(QApplication::translate("Warningdialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Warningdialog: public Ui_Warningdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WARNINGDIALOG_H
