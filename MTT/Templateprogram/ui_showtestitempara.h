/********************************************************************************
** Form generated from reading UI file 'showtestitempara.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWTESTITEMPARA_H
#define UI_SHOWTESTITEMPARA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_ShowTestitemPara
{
public:
    QPushButton *pushButton;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *ShowTestitemPara)
    {
        if (ShowTestitemPara->objectName().isEmpty())
            ShowTestitemPara->setObjectName(QString::fromUtf8("ShowTestitemPara"));
        ShowTestitemPara->resize(465, 344);
        pushButton = new QPushButton(ShowTestitemPara);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(170, 300, 121, 41));
        textBrowser = new QTextBrowser(ShowTestitemPara);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(0, 0, 471, 291));

        retranslateUi(ShowTestitemPara);

        QMetaObject::connectSlotsByName(ShowTestitemPara);
    } // setupUi

    void retranslateUi(QDialog *ShowTestitemPara)
    {
        ShowTestitemPara->setWindowTitle(QApplication::translate("ShowTestitemPara", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ShowTestitemPara", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ShowTestitemPara: public Ui_ShowTestitemPara {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWTESTITEMPARA_H
