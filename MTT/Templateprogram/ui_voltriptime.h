/********************************************************************************
** Form generated from reading UI file 'voltriptime.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLTRIPTIME_H
#define UI_VOLTRIPTIME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VolTriptime
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_2;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_7;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_16;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_19;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QComboBox *comboBox_3;
    QGroupBox *groupBox_5;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_13;
    QLabel *label_14;
    QCheckBox *checkBox_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_7;
    QComboBox *comboBox;
    QLabel *label_12;
    QComboBox *comboBox_2;

    void setupUi(QDialog *VolTriptime)
    {
        if (VolTriptime->objectName().isEmpty())
            VolTriptime->setObjectName(QString::fromUtf8("VolTriptime"));
        VolTriptime->resize(800, 480);
        pushButton = new QPushButton(VolTriptime);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(540, 410, 81, 41));
        pushButton_2 = new QPushButton(VolTriptime);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(670, 410, 75, 41));
        groupBox_2 = new QGroupBox(VolTriptime);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(380, 60, 361, 161));
        lineEdit_5 = new QLineEdit(groupBox_2);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(170, 60, 91, 31));
        lineEdit_6 = new QLineEdit(groupBox_2);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(170, 90, 91, 31));
        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(170, 30, 91, 31));
        lineEdit_7 = new QLineEdit(groupBox_2);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(170, 120, 91, 31));
        verticalLayoutWidget_2 = new QWidget(groupBox_2);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(270, 60, 41, 91));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(verticalLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        label_16 = new QLabel(verticalLayoutWidget_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        verticalLayout_2->addWidget(label_16);

        verticalLayoutWidget_3 = new QWidget(groupBox_2);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(10, 30, 141, 121));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_19 = new QLabel(verticalLayoutWidget_3);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMinimumSize(QSize(120, 30));

        verticalLayout_3->addWidget(label_19);

        checkBox = new QCheckBox(verticalLayoutWidget_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        sizePolicy.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy);
        checkBox->setMinimumSize(QSize(120, 30));

        verticalLayout_3->addWidget(checkBox);

        checkBox_2 = new QCheckBox(verticalLayoutWidget_3);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        sizePolicy.setHeightForWidth(checkBox_2->sizePolicy().hasHeightForWidth());
        checkBox_2->setSizePolicy(sizePolicy);
        checkBox_2->setMinimumSize(QSize(120, 30));

        verticalLayout_3->addWidget(checkBox_2);

        comboBox_3 = new QComboBox(verticalLayoutWidget_3);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        sizePolicy.setHeightForWidth(comboBox_3->sizePolicy().hasHeightForWidth());
        comboBox_3->setSizePolicy(sizePolicy);
        comboBox_3->setMinimumSize(QSize(120, 30));

        verticalLayout_3->addWidget(comboBox_3);

        groupBox_5 = new QGroupBox(VolTriptime);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(20, 60, 351, 161));
        lineEdit_2 = new QLineEdit(groupBox_5);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(160, 60, 161, 31));
        lineEdit_3 = new QLineEdit(groupBox_5);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(160, 90, 161, 31));
        verticalLayoutWidget = new QWidget(groupBox_5);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 60, 151, 61));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(verticalLayoutWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout->addWidget(label_13);

        label_14 = new QLabel(verticalLayoutWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        verticalLayout->addWidget(label_14);

        checkBox_3 = new QCheckBox(groupBox_5);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(10, 16, 331, 31));
        horizontalLayoutWidget = new QWidget(VolTriptime);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 10, 692, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(90, 30));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(horizontalLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);
        lineEdit->setMinimumSize(QSize(120, 30));

        horizontalLayout->addWidget(lineEdit);

        label_7 = new QLabel(horizontalLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setMinimumSize(QSize(120, 30));

        horizontalLayout->addWidget(label_7);

        comboBox = new QComboBox(horizontalLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMinimumSize(QSize(120, 30));

        horizontalLayout->addWidget(comboBox);

        label_12 = new QLabel(horizontalLayoutWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_12);

        comboBox_2 = new QComboBox(horizontalLayoutWidget);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        sizePolicy.setHeightForWidth(comboBox_2->sizePolicy().hasHeightForWidth());
        comboBox_2->setSizePolicy(sizePolicy);
        comboBox_2->setMinimumSize(QSize(90, 30));

        horizontalLayout->addWidget(comboBox_2);


        retranslateUi(VolTriptime);

        QMetaObject::connectSlotsByName(VolTriptime);
    } // setupUi

    void retranslateUi(QDialog *VolTriptime)
    {
        VolTriptime->setWindowTitle(QApplication::translate("VolTriptime", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("VolTriptime", "Save", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("VolTriptime", "Cancel", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("VolTriptime", "Assessment", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("VolTriptime", "%", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("VolTriptime", "+", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("VolTriptime", "-", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("VolTriptime", "Compare with", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("VolTriptime", "Rel.error", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("VolTriptime", "abs.error", 0, QApplication::UnicodeUTF8));
        comboBox_3->clear();
        comboBox_3->insertItems(0, QStringList()
         << QApplication::translate("VolTriptime", "and", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "or", 0, QApplication::UnicodeUTF8)
        );
        groupBox_5->setTitle(QApplication::translate("VolTriptime", "Setting", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("VolTriptime", "Voltage End(V)", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("VolTriptime", "Maximum Time(s):", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("VolTriptime", "Use measured value", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("VolTriptime", "Name:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("VolTriptime", "Fault Type:", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("VolTriptime", "A-E", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "B-E", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "C-E", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "A-B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "B-C", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "C-A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "A-B-C", 0, QApplication::UnicodeUTF8)
        );
        label_12->setText(QApplication::translate("VolTriptime", "Voltage output type:", 0, QApplication::UnicodeUTF8));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("VolTriptime", "Vpn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VolTriptime", "Vpp", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class VolTriptime: public Ui_VolTriptime {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLTRIPTIME_H
