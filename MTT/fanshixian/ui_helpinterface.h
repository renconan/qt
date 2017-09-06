/********************************************************************************
** Form generated from reading UI file 'helpinterface.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPINTERFACE_H
#define UI_HELPINTERFACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Helpinterface
{
public:
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *Helpinterface)
    {
        if (Helpinterface->objectName().isEmpty())
            Helpinterface->setObjectName(QString::fromUtf8("Helpinterface"));
        Helpinterface->resize(479, 342);
        label = new QLabel(Helpinterface);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 10, 81, 21));
        pushButton = new QPushButton(Helpinterface);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(190, 250, 75, 23));

        retranslateUi(Helpinterface);

        QMetaObject::connectSlotsByName(Helpinterface);
    } // setupUi

    void retranslateUi(QWidget *Helpinterface)
    {
        Helpinterface->setWindowTitle(QApplication::translate("Helpinterface", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Helpinterface", "\345\270\256\345\212\251\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Helpinterface", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Helpinterface: public Ui_Helpinterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPINTERFACE_H
