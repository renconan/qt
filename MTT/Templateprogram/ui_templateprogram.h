/********************************************************************************
** Form generated from reading UI file 'templateprogram.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEMPLATEPROGRAM_H
#define UI_TEMPLATEPROGRAM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Templateprogram
{
public:
    QLabel *label;
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLabel *label_2;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QLabel *label_3;
    QPushButton *pushButton_3;
    QLabel *label_4;

    void setupUi(QWidget *Templateprogram)
    {
        if (Templateprogram->objectName().isEmpty())
            Templateprogram->setObjectName(QString::fromUtf8("Templateprogram"));
        Templateprogram->resize(800, 480);
        label = new QLabel(Templateprogram);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 0, 391, 31));
        tableWidget = new QTableWidget(Templateprogram);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 40, 801, 281));
        pushButton = new QPushButton(Templateprogram);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(140, 330, 111, 41));
        pushButton_2 = new QPushButton(Templateprogram);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(140, 380, 111, 41));
        groupBox = new QGroupBox(Templateprogram);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(590, 330, 191, 101));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 20, 160, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        radioButton = new QRadioButton(horizontalLayoutWidget);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(horizontalLayoutWidget);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout->addWidget(radioButton_2);

        label_2 = new QLabel(Templateprogram);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(450, 440, 341, 31));
        pushButton_5 = new QPushButton(Templateprogram);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(270, 330, 111, 41));
        pushButton_6 = new QPushButton(Templateprogram);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(270, 380, 111, 41));
        pushButton_7 = new QPushButton(Templateprogram);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(10, 380, 111, 41));
        pushButton_8 = new QPushButton(Templateprogram);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(400, 330, 111, 41));
        label_3 = new QLabel(Templateprogram);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 440, 361, 31));
        pushButton_3 = new QPushButton(Templateprogram);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 330, 111, 41));
        label_4 = new QLabel(Templateprogram);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 0, 211, 41));

        retranslateUi(Templateprogram);

        QMetaObject::connectSlotsByName(Templateprogram);
    } // setupUi

    void retranslateUi(QWidget *Templateprogram)
    {
        Templateprogram->setWindowTitle(QApplication::translate("Templateprogram", "Widget", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        pushButton->setText(QApplication::translate("Templateprogram", "Page Up", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Templateprogram", "Page Down", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Templateprogram", "Run Mode", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("Templateprogram", "Auto", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("Templateprogram", "Manual", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        pushButton_5->setText(QApplication::translate("Templateprogram", "Setting", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("Templateprogram", "Help", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("Templateprogram", "Save Report", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("Templateprogram", "Exit", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        pushButton_3->setText(QApplication::translate("Templateprogram", "Test", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Templateprogram", "Template Test", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Templateprogram: public Ui_Templateprogram {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEMPLATEPROGRAM_H
