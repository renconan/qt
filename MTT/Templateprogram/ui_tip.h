/********************************************************************************
** Form generated from reading UI file 'tip.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIP_H
#define UI_TIP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Tip
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *Tip)
    {
        if (Tip->objectName().isEmpty())
            Tip->setObjectName(QString::fromUtf8("Tip"));
        Tip->resize(617, 273);
        label = new QLabel(Tip);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 50, 591, 31));
        label_2 = new QLabel(Tip);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 140, 571, 51));

        retranslateUi(Tip);

        QMetaObject::connectSlotsByName(Tip);
    } // setupUi

    void retranslateUi(QDialog *Tip)
    {
        Tip->setWindowTitle(QApplication::translate("Tip", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Tip", "1", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Tip", "2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Tip: public Ui_Tip {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIP_H
