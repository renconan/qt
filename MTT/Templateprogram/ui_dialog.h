/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QToolButton *toolButton_2;
    QToolButton *toolButton_point;
    QToolButton *toolButton_1;
    QToolButton *toolButton_0;
    QToolButton *toolButton_enter;
    QToolButton *toolButton_3;
    QToolButton *toolButton_back;
    QToolButton *toolButton_5;
    QToolButton *toolButton_4;
    QLineEdit *display;
    QToolButton *toolButton_clear;
    QToolButton *toolButton_7;
    QToolButton *toolButton_6;
    QToolButton *toolButton_sign;
    QToolButton *toolButton_8;
    QToolButton *toolButton_9;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(303, 220);
        toolButton_2 = new QToolButton(Dialog);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        toolButton_2->setGeometry(QRect(60, 40, 55, 55));
        QFont font;
        font.setPointSize(10);
        toolButton_2->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/picture/2.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon);
        toolButton_point = new QToolButton(Dialog);
        toolButton_point->setObjectName(QString::fromUtf8("toolButton_point"));
        toolButton_point->setGeometry(QRect(180, 160, 55, 55));
        toolButton_point->setFont(font);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/picture/dot.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_point->setIcon(icon1);
        toolButton_1 = new QToolButton(Dialog);
        toolButton_1->setObjectName(QString::fromUtf8("toolButton_1"));
        toolButton_1->setGeometry(QRect(0, 40, 55, 55));
        toolButton_1->setFont(font);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/picture/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_1->setIcon(icon2);
        toolButton_0 = new QToolButton(Dialog);
        toolButton_0->setObjectName(QString::fromUtf8("toolButton_0"));
        toolButton_0->setGeometry(QRect(180, 40, 55, 55));
        toolButton_0->setFont(font);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/picture/0.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_0->setIcon(icon3);
        toolButton_enter = new QToolButton(Dialog);
        toolButton_enter->setObjectName(QString::fromUtf8("toolButton_enter"));
        toolButton_enter->setGeometry(QRect(240, 160, 55, 55));
        toolButton_enter->setFont(font);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/picture/enter.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_enter->setIcon(icon4);
        toolButton_3 = new QToolButton(Dialog);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));
        toolButton_3->setGeometry(QRect(120, 40, 55, 55));
        toolButton_3->setFont(font);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/picture/3.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_3->setIcon(icon5);
        toolButton_back = new QToolButton(Dialog);
        toolButton_back->setObjectName(QString::fromUtf8("toolButton_back"));
        toolButton_back->setGeometry(QRect(240, 40, 55, 55));
        toolButton_back->setFont(font);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/picture/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_back->setIcon(icon6);
        toolButton_5 = new QToolButton(Dialog);
        toolButton_5->setObjectName(QString::fromUtf8("toolButton_5"));
        toolButton_5->setGeometry(QRect(60, 100, 55, 55));
        toolButton_5->setFont(font);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/picture/5.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_5->setIcon(icon7);
        toolButton_4 = new QToolButton(Dialog);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));
        toolButton_4->setGeometry(QRect(0, 100, 55, 55));
        toolButton_4->setFont(font);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/picture/4.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_4->setIcon(icon8);
        display = new QLineEdit(Dialog);
        display->setObjectName(QString::fromUtf8("display"));
        display->setGeometry(QRect(0, 0, 301, 31));
        QFont font1;
        font1.setPointSize(12);
        display->setFont(font1);
        display->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        toolButton_clear = new QToolButton(Dialog);
        toolButton_clear->setObjectName(QString::fromUtf8("toolButton_clear"));
        toolButton_clear->setGeometry(QRect(240, 100, 55, 55));
        toolButton_clear->setFont(font);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/picture/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_clear->setIcon(icon9);
        toolButton_7 = new QToolButton(Dialog);
        toolButton_7->setObjectName(QString::fromUtf8("toolButton_7"));
        toolButton_7->setGeometry(QRect(0, 160, 55, 55));
        toolButton_7->setFont(font);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/picture/7.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_7->setIcon(icon10);
        toolButton_6 = new QToolButton(Dialog);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));
        toolButton_6->setGeometry(QRect(120, 100, 55, 55));
        toolButton_6->setFont(font);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/picture/6.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_6->setIcon(icon11);
        toolButton_sign = new QToolButton(Dialog);
        toolButton_sign->setObjectName(QString::fromUtf8("toolButton_sign"));
        toolButton_sign->setGeometry(QRect(180, 100, 55, 55));
        toolButton_sign->setFont(font);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/picture/sign.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_sign->setIcon(icon12);
        toolButton_8 = new QToolButton(Dialog);
        toolButton_8->setObjectName(QString::fromUtf8("toolButton_8"));
        toolButton_8->setGeometry(QRect(60, 160, 55, 55));
        toolButton_8->setFont(font);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/picture/8.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_8->setIcon(icon13);
        toolButton_9 = new QToolButton(Dialog);
        toolButton_9->setObjectName(QString::fromUtf8("toolButton_9"));
        toolButton_9->setGeometry(QRect(120, 160, 55, 55));
        toolButton_9->setFont(font);
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/picture/9.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_9->setIcon(icon14);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        toolButton_2->setText(QApplication::translate("Dialog", "2", 0, QApplication::UnicodeUTF8));
        toolButton_point->setText(QApplication::translate("Dialog", ".", 0, QApplication::UnicodeUTF8));
        toolButton_1->setText(QApplication::translate("Dialog", "1", 0, QApplication::UnicodeUTF8));
        toolButton_0->setText(QApplication::translate("Dialog", "0", 0, QApplication::UnicodeUTF8));
        toolButton_enter->setText(QApplication::translate("Dialog", "Enter", 0, QApplication::UnicodeUTF8));
        toolButton_3->setText(QApplication::translate("Dialog", "3", 0, QApplication::UnicodeUTF8));
        toolButton_back->setText(QApplication::translate("Dialog", "Back", 0, QApplication::UnicodeUTF8));
        toolButton_5->setText(QApplication::translate("Dialog", "5", 0, QApplication::UnicodeUTF8));
        toolButton_4->setText(QApplication::translate("Dialog", "4", 0, QApplication::UnicodeUTF8));
        display->setText(QApplication::translate("Dialog", "0", 0, QApplication::UnicodeUTF8));
        toolButton_clear->setText(QApplication::translate("Dialog", "C", 0, QApplication::UnicodeUTF8));
        toolButton_7->setText(QApplication::translate("Dialog", "7", 0, QApplication::UnicodeUTF8));
        toolButton_6->setText(QApplication::translate("Dialog", "6", 0, QApplication::UnicodeUTF8));
        toolButton_sign->setText(QApplication::translate("Dialog", "-/+", 0, QApplication::UnicodeUTF8));
        toolButton_8->setText(QApplication::translate("Dialog", "8", 0, QApplication::UnicodeUTF8));
        toolButton_9->setText(QApplication::translate("Dialog", "9", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
