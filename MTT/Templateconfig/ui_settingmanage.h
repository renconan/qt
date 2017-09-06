/********************************************************************************
** Form generated from reading UI file 'settingmanage.ui'
**
** Created: Wed Dec 14 10:18:59 2016
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settingmanage
{
public:
    QTableWidget *tableWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;

    void setupUi(QWidget *Settingmanage)
    {
        if (Settingmanage->objectName().isEmpty())
            Settingmanage->setObjectName(QString::fromUtf8("Settingmanage"));
        Settingmanage->resize(510, 561);
        tableWidget = new QTableWidget(Settingmanage);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 510, 491));
        horizontalLayoutWidget = new QWidget(Settingmanage);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 500, 491, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(60, 30));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(horizontalLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(60, 30));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(horizontalLayoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setMinimumSize(QSize(60, 30));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(horizontalLayoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(60, 30));

        horizontalLayout->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(horizontalLayoutWidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        sizePolicy.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy);
        pushButton_5->setMinimumSize(QSize(60, 30));

        horizontalLayout->addWidget(pushButton_5);


        retranslateUi(Settingmanage);

        QMetaObject::connectSlotsByName(Settingmanage);
    } // setupUi

    void retranslateUi(QWidget *Settingmanage)
    {
        Settingmanage->setWindowTitle(QApplication::translate("Settingmanage", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Settingmanage", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Settingmanage", "Edit", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Settingmanage", "Delete", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("Settingmanage", "Save", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("Settingmanage", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Settingmanage: public Ui_Settingmanage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGMANAGE_H
