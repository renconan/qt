/********************************************************************************
** Form generated from reading UI file 'settingmanage.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
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

class Ui_SettingManage
{
public:
    QTableWidget *tableWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QWidget *SettingManage)
    {
        if (SettingManage->objectName().isEmpty())
            SettingManage->setObjectName(QString::fromUtf8("SettingManage"));
        SettingManage->resize(800, 480);
        tableWidget = new QTableWidget(SettingManage);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 801, 391));
        horizontalLayoutWidget = new QWidget(SettingManage);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 410, 761, 43));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(121, 41));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(horizontalLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(121, 41));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(horizontalLayoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setMinimumSize(QSize(121, 41));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(horizontalLayoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(121, 41));

        horizontalLayout->addWidget(pushButton_4);


        retranslateUi(SettingManage);

        QMetaObject::connectSlotsByName(SettingManage);
    } // setupUi

    void retranslateUi(QWidget *SettingManage)
    {
        SettingManage->setWindowTitle(QApplication::translate("SettingManage", "Widget", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("SettingManage", "Page Up", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("SettingManage", "Page Down", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("SettingManage", "Save", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("SettingManage", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingManage: public Ui_SettingManage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGMANAGE_H
