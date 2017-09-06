/********************************************************************************
** Form generated from reading UI file 'dfdtblockingtime.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DFDTBLOCKINGTIME_H
#define UI_DFDTBLOCKINGTIME_H

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

class Ui_DfdtBlockingTime
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QGroupBox *groupBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_2;
    QGroupBox *groupBox_2;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_9;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_6;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *DfdtBlockingTime)
    {
        if (DfdtBlockingTime->objectName().isEmpty())
            DfdtBlockingTime->setObjectName(QString::fromUtf8("DfdtBlockingTime"));
        DfdtBlockingTime->resize(800, 480);
        label = new QLabel(DfdtBlockingTime);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 81, 31));
        lineEdit = new QLineEdit(DfdtBlockingTime);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(120, 20, 161, 31));
        groupBox = new QGroupBox(DfdtBlockingTime);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 60, 421, 161));
        verticalLayoutWidget = new QWidget(groupBox);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 201, 121));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        label_5 = new QLabel(verticalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        lineEdit_5 = new QLineEdit(groupBox);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(210, 110, 201, 31));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(210, 50, 201, 31));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(210, 80, 201, 31));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(210, 20, 201, 31));
        groupBox_2 = new QGroupBox(DfdtBlockingTime);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 230, 421, 181));
        lineEdit_6 = new QLineEdit(groupBox_2);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(140, 40, 113, 31));
        lineEdit_7 = new QLineEdit(groupBox_2);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(140, 70, 113, 31));
        lineEdit_8 = new QLineEdit(groupBox_2);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(140, 100, 113, 31));
        lineEdit_9 = new QLineEdit(groupBox_2);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        lineEdit_9->setGeometry(QRect(140, 130, 113, 31));
        verticalLayoutWidget_2 = new QWidget(groupBox_2);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 40, 121, 121));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(verticalLayoutWidget_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMinimumSize(QSize(120, 25));

        verticalLayout_2->addWidget(label_6);

        checkBox = new QCheckBox(verticalLayoutWidget_2);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        sizePolicy.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy);
        checkBox->setMinimumSize(QSize(120, 25));

        verticalLayout_2->addWidget(checkBox);

        checkBox_2 = new QCheckBox(verticalLayoutWidget_2);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        sizePolicy.setHeightForWidth(checkBox_2->sizePolicy().hasHeightForWidth());
        checkBox_2->setSizePolicy(sizePolicy);
        checkBox_2->setMinimumSize(QSize(120, 25));

        verticalLayout_2->addWidget(checkBox_2);

        comboBox = new QComboBox(verticalLayoutWidget_2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMinimumSize(QSize(120, 25));

        verticalLayout_2->addWidget(comboBox);

        pushButton = new QPushButton(DfdtBlockingTime);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(530, 420, 91, 41));
        pushButton_2 = new QPushButton(DfdtBlockingTime);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(670, 420, 91, 41));

        retranslateUi(DfdtBlockingTime);

        QMetaObject::connectSlotsByName(DfdtBlockingTime);
    } // setupUi

    void retranslateUi(QDialog *DfdtBlockingTime)
    {
        DfdtBlockingTime->setWindowTitle(QApplication::translate("DfdtBlockingTime", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DfdtBlockingTime", "Name:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("DfdtBlockingTime", "Setting", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DfdtBlockingTime", "df/dt(Hz/s)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DfdtBlockingTime", "stop freq(Hz)", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DfdtBlockingTime", "Maximum Time(s):", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DfdtBlockingTime", "hold Time(s):", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("DfdtBlockingTime", "Assessment", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DfdtBlockingTime", "Compare with:", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("DfdtBlockingTime", "Rel.error", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("DfdtBlockingTime", "Abs.error", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("DfdtBlockingTime", "and", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DfdtBlockingTime", "or", 0, QApplication::UnicodeUTF8)
        );
        pushButton->setText(QApplication::translate("DfdtBlockingTime", "Save", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("DfdtBlockingTime", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DfdtBlockingTime: public Ui_DfdtBlockingTime {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DFDTBLOCKINGTIME_H
