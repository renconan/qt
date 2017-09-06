/********************************************************************************
** Form generated from reading UI file 'freqtriptime.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FREQTRIPTIME_H
#define UI_FREQTRIPTIME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FreqTripTime
{
public:
    QLabel *label;
    QGroupBox *groupBox_3;
    QLineEdit *lineEdit_6;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_7;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_7;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_6;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QComboBox *comboBox;
    QLineEdit *lineEdit;
    QGroupBox *groupBox_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *FreqTripTime)
    {
        if (FreqTripTime->objectName().isEmpty())
            FreqTripTime->setObjectName(QString::fromUtf8("FreqTripTime"));
        FreqTripTime->resize(800, 480);
        label = new QLabel(FreqTripTime);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 71, 31));
        groupBox_3 = new QGroupBox(FreqTripTime);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 180, 351, 191));
        lineEdit_6 = new QLineEdit(groupBox_3);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(150, 90, 131, 31));
        verticalLayoutWidget_2 = new QWidget(groupBox_3);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(290, 60, 31, 91));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(verticalLayoutWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_2->addWidget(label_7);

        label_14 = new QLabel(verticalLayoutWidget_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        verticalLayout_2->addWidget(label_14);

        label_15 = new QLabel(verticalLayoutWidget_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        verticalLayout_2->addWidget(label_15);

        lineEdit_4 = new QLineEdit(groupBox_3);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(150, 30, 131, 31));
        lineEdit_5 = new QLineEdit(groupBox_3);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(150, 60, 131, 31));
        lineEdit_7 = new QLineEdit(groupBox_3);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(150, 120, 131, 31));
        verticalLayoutWidget_3 = new QWidget(groupBox_3);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(10, 30, 141, 121));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(verticalLayoutWidget_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMinimumSize(QSize(120, 25));

        verticalLayout_3->addWidget(label_6);

        checkBox = new QCheckBox(verticalLayoutWidget_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        sizePolicy.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy);
        checkBox->setMinimumSize(QSize(120, 25));

        verticalLayout_3->addWidget(checkBox);

        checkBox_2 = new QCheckBox(verticalLayoutWidget_3);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        sizePolicy.setHeightForWidth(checkBox_2->sizePolicy().hasHeightForWidth());
        checkBox_2->setSizePolicy(sizePolicy);
        checkBox_2->setMinimumSize(QSize(120, 25));

        verticalLayout_3->addWidget(checkBox_2);

        comboBox = new QComboBox(verticalLayoutWidget_3);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMinimumSize(QSize(120, 25));

        verticalLayout_3->addWidget(comboBox);

        lineEdit = new QLineEdit(FreqTripTime);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(80, 20, 121, 31));
        groupBox_2 = new QGroupBox(FreqTripTime);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 70, 351, 111));
        verticalLayoutWidget = new QWidget(groupBox_2);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 141, 61));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(170, 50, 171, 31));
        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(170, 20, 171, 31));
        pushButton_2 = new QPushButton(FreqTripTime);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(670, 410, 81, 41));
        pushButton = new QPushButton(FreqTripTime);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(550, 410, 81, 41));

        retranslateUi(FreqTripTime);

        QMetaObject::connectSlotsByName(FreqTripTime);
    } // setupUi

    void retranslateUi(QDialog *FreqTripTime)
    {
        FreqTripTime->setWindowTitle(QApplication::translate("FreqTripTime", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FreqTripTime", "Name:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("FreqTripTime", "Assessment", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("FreqTripTime", "%", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("FreqTripTime", "+", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("FreqTripTime", "-", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("FreqTripTime", "Compare with", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("FreqTripTime", "Rel.error", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("FreqTripTime", "ABS.error", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("FreqTripTime", "and", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FreqTripTime", "or", 0, QApplication::UnicodeUTF8)
        );
        groupBox_2->setTitle(QApplication::translate("FreqTripTime", "Setting", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FreqTripTime", "Freq stop(Hz):", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("FreqTripTime", "Maximum Time(s):", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("FreqTripTime", "Cancel", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FreqTripTime", "Save", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FreqTripTime: public Ui_FreqTripTime {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREQTRIPTIME_H
