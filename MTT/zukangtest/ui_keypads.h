/********************************************************************************
** Form generated from reading UI file 'keypads.ui'
**
** Created: Thu Jun 16 15:38:16 2016
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYPADS_H
#define UI_KEYPADS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_keypads
{
public:
    QToolButton *toolButton_0;
    QToolButton *toolButton_1;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton_4;
    QToolButton *toolButton_5;
    QToolButton *toolButton_6;
    QToolButton *toolButton_7;
    QToolButton *toolButton_8;
    QToolButton *toolButton_9;
    QToolButton *toolButton_point;
    QToolButton *toolButton_sign;
    QToolButton *toolButton_clear;
    QToolButton *toolButton_enter;
    QToolButton *toolButton_back;
    QLineEdit *display;

    void setupUi(QDialog *keypads)
    {
        if (keypads->objectName().isEmpty())
            keypads->setObjectName(QString::fromUtf8("keypads"));
        keypads->resize(311, 221);
        toolButton_0 = new QToolButton(keypads);
        toolButton_0->setObjectName(QString::fromUtf8("toolButton_0"));
        toolButton_0->setGeometry(QRect(190, 40, 55, 55));
        QFont font;
        font.setPointSize(10);
        toolButton_0->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("picture/0.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_0->setIcon(icon);
        toolButton_1 = new QToolButton(keypads);
        toolButton_1->setObjectName(QString::fromUtf8("toolButton_1"));
        toolButton_1->setGeometry(QRect(10, 40, 55, 55));
        toolButton_1->setFont(font);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("picture/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_1->setIcon(icon1);
        toolButton_2 = new QToolButton(keypads);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        toolButton_2->setGeometry(QRect(70, 40, 55, 55));
        toolButton_2->setFont(font);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("picture/2.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon2);
        toolButton_3 = new QToolButton(keypads);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));
        toolButton_3->setGeometry(QRect(130, 40, 55, 55));
        toolButton_3->setFont(font);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("picture/3.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_3->setIcon(icon3);
        toolButton_4 = new QToolButton(keypads);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));
        toolButton_4->setGeometry(QRect(10, 100, 55, 55));
        toolButton_4->setFont(font);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("picture/4.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_4->setIcon(icon4);
        toolButton_5 = new QToolButton(keypads);
        toolButton_5->setObjectName(QString::fromUtf8("toolButton_5"));
        toolButton_5->setGeometry(QRect(70, 100, 55, 55));
        toolButton_5->setFont(font);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("picture/5.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_5->setIcon(icon5);
        toolButton_6 = new QToolButton(keypads);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));
        toolButton_6->setGeometry(QRect(130, 100, 55, 55));
        toolButton_6->setFont(font);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("picture/6.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_6->setIcon(icon6);
        toolButton_7 = new QToolButton(keypads);
        toolButton_7->setObjectName(QString::fromUtf8("toolButton_7"));
        toolButton_7->setGeometry(QRect(10, 160, 55, 55));
        toolButton_7->setFont(font);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("picture/7.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_7->setIcon(icon7);
        toolButton_8 = new QToolButton(keypads);
        toolButton_8->setObjectName(QString::fromUtf8("toolButton_8"));
        toolButton_8->setGeometry(QRect(70, 160, 55, 55));
        toolButton_8->setFont(font);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8("picture/8.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_8->setIcon(icon8);
        toolButton_9 = new QToolButton(keypads);
        toolButton_9->setObjectName(QString::fromUtf8("toolButton_9"));
        toolButton_9->setGeometry(QRect(130, 160, 55, 55));
        toolButton_9->setFont(font);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8("picture/9.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_9->setIcon(icon9);
        toolButton_point = new QToolButton(keypads);
        toolButton_point->setObjectName(QString::fromUtf8("toolButton_point"));
        toolButton_point->setGeometry(QRect(190, 160, 55, 55));
        toolButton_point->setFont(font);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8("picture/12.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_point->setIcon(icon10);
        toolButton_sign = new QToolButton(keypads);
        toolButton_sign->setObjectName(QString::fromUtf8("toolButton_sign"));
        toolButton_sign->setGeometry(QRect(190, 100, 55, 55));
        toolButton_sign->setFont(font);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8("picture/fu.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_sign->setIcon(icon11);
        toolButton_clear = new QToolButton(keypads);
        toolButton_clear->setObjectName(QString::fromUtf8("toolButton_clear"));
        toolButton_clear->setGeometry(QRect(250, 100, 55, 55));
        toolButton_clear->setFont(font);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8("picture/cha.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_clear->setIcon(icon12);
        toolButton_enter = new QToolButton(keypads);
        toolButton_enter->setObjectName(QString::fromUtf8("toolButton_enter"));
        toolButton_enter->setGeometry(QRect(250, 160, 55, 55));
        toolButton_enter->setFont(font);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8("picture/enter.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_enter->setIcon(icon13);
        toolButton_back = new QToolButton(keypads);
        toolButton_back->setObjectName(QString::fromUtf8("toolButton_back"));
        toolButton_back->setGeometry(QRect(250, 40, 55, 55));
        toolButton_back->setFont(font);
        QIcon icon14;
        icon14.addFile(QString::fromUtf8("picture/del.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_back->setIcon(icon14);
        display = new QLineEdit(keypads);
        display->setObjectName(QString::fromUtf8("display"));
        display->setGeometry(QRect(0, 0, 311, 31));
        QFont font1;
        font1.setPointSize(12);
        display->setFont(font1);
        display->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(keypads);

        QMetaObject::connectSlotsByName(keypads);
    } // setupUi

    void retranslateUi(QDialog *keypads)
    {
        keypads->setWindowTitle(QString());
        toolButton_0->setText(QApplication::translate("keypads", "0", 0, QApplication::UnicodeUTF8));
        toolButton_1->setText(QApplication::translate("keypads", "1", 0, QApplication::UnicodeUTF8));
        toolButton_2->setText(QApplication::translate("keypads", "2", 0, QApplication::UnicodeUTF8));
        toolButton_3->setText(QApplication::translate("keypads", "3", 0, QApplication::UnicodeUTF8));
        toolButton_4->setText(QApplication::translate("keypads", "4", 0, QApplication::UnicodeUTF8));
        toolButton_5->setText(QApplication::translate("keypads", "5", 0, QApplication::UnicodeUTF8));
        toolButton_6->setText(QApplication::translate("keypads", "6", 0, QApplication::UnicodeUTF8));
        toolButton_7->setText(QApplication::translate("keypads", "7", 0, QApplication::UnicodeUTF8));
        toolButton_8->setText(QApplication::translate("keypads", "8", 0, QApplication::UnicodeUTF8));
        toolButton_9->setText(QApplication::translate("keypads", "9", 0, QApplication::UnicodeUTF8));
        toolButton_point->setText(QApplication::translate("keypads", ".", 0, QApplication::UnicodeUTF8));
        toolButton_sign->setText(QApplication::translate("keypads", "-/+", 0, QApplication::UnicodeUTF8));
        toolButton_clear->setText(QApplication::translate("keypads", "C", 0, QApplication::UnicodeUTF8));
        toolButton_enter->setText(QApplication::translate("keypads", "Enter", 0, QApplication::UnicodeUTF8));
        toolButton_back->setText(QApplication::translate("keypads", "Back", 0, QApplication::UnicodeUTF8));
        display->setText(QApplication::translate("keypads", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class keypads: public Ui_keypads {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYPADS_H
