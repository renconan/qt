/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGroupBox *groupBox_5;
    QTextBrowser *textBrowser;
    QLabel *label_17;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_5;
    QWidget *tab_2;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_8;
    QLabel *label_7;
    QWidget *tab_3;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLineEdit *lineEdit_12;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_12;
    QLabel *label_13;
    QWidget *tab_4;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_25;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_23;
    QLineEdit *lineEdit_24;
    QWidget *tab_5;
    QLineEdit *lineEdit_16;
    QLineEdit *lineEdit_17;
    QLineEdit *lineEdit_18;
    QLineEdit *lineEdit_19;
    QLineEdit *lineEdit_20;
    QComboBox *comboBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_24;
    QGroupBox *groupBox;
    QLabel *label_11;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label_26;
    QLineEdit *lineEdit_15;
    QLineEdit *lineEdit_22;
    QLabel *label;
    QLabel *label_27;
    QLabel *label_28;
    QLabel *label_2;
    QLabel *label_23;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *label_35;
    QPushButton *pushButton_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 480);
        groupBox_5 = new QGroupBox(Widget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(545, 310, 246, 111));
        textBrowser = new QTextBrowser(groupBox_5);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 20, 226, 81));
        label_17 = new QLabel(Widget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 421, 301, 51));
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 40, 491, 261));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayoutWidget_2 = new QWidget(tab);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 20, 151, 121));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        label_4 = new QLabel(verticalLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        label_5 = new QLabel(verticalLayoutWidget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_2->addWidget(label_5);

        label_6 = new QLabel(verticalLayoutWidget_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_2->addWidget(label_6);

        lineEdit_6 = new QLineEdit(tab);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(160, 110, 113, 31));
        lineEdit_4 = new QLineEdit(tab);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(160, 50, 113, 31));
        lineEdit_3 = new QLineEdit(tab);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(160, 20, 113, 31));
        lineEdit_5 = new QLineEdit(tab);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(160, 80, 113, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        lineEdit_7 = new QLineEdit(tab_2);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(190, 55, 113, 31));
        lineEdit_8 = new QLineEdit(tab_2);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(190, 85, 113, 31));
        verticalLayoutWidget_6 = new QWidget(tab_2);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(15, 55, 176, 61));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(verticalLayoutWidget_6);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        verticalLayout_6->addWidget(label_8);

        label_7 = new QLabel(verticalLayoutWidget_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_6->addWidget(label_7);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        lineEdit_9 = new QLineEdit(tab_3);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        lineEdit_9->setGeometry(QRect(190, 30, 113, 31));
        lineEdit_10 = new QLineEdit(tab_3);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));
        lineEdit_10->setGeometry(QRect(190, 60, 113, 31));
        lineEdit_11 = new QLineEdit(tab_3);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));
        lineEdit_11->setGeometry(QRect(190, 90, 113, 31));
        lineEdit_12 = new QLineEdit(tab_3);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));
        lineEdit_12->setGeometry(QRect(190, 120, 113, 31));
        verticalLayoutWidget_3 = new QWidget(tab_3);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(10, 30, 181, 121));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(verticalLayoutWidget_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_3->addWidget(label_9);

        label_10 = new QLabel(verticalLayoutWidget_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout_3->addWidget(label_10);

        label_12 = new QLabel(verticalLayoutWidget_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        verticalLayout_3->addWidget(label_12);

        label_13 = new QLabel(verticalLayoutWidget_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout_3->addWidget(label_13);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayoutWidget_4 = new QWidget(tab_4);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(5, 30, 251, 121));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_14 = new QLabel(verticalLayoutWidget_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        verticalLayout_4->addWidget(label_14);

        label_15 = new QLabel(verticalLayoutWidget_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        verticalLayout_4->addWidget(label_15);

        label_16 = new QLabel(verticalLayoutWidget_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        verticalLayout_4->addWidget(label_16);

        label_25 = new QLabel(verticalLayoutWidget_4);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        verticalLayout_4->addWidget(label_25);

        lineEdit_14 = new QLineEdit(tab_4);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));
        lineEdit_14->setGeometry(QRect(255, 60, 113, 31));
        lineEdit_13 = new QLineEdit(tab_4);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));
        lineEdit_13->setGeometry(QRect(255, 30, 113, 31));
        lineEdit_23 = new QLineEdit(tab_4);
        lineEdit_23->setObjectName(QString::fromUtf8("lineEdit_23"));
        lineEdit_23->setGeometry(QRect(255, 90, 113, 31));
        lineEdit_24 = new QLineEdit(tab_4);
        lineEdit_24->setObjectName(QString::fromUtf8("lineEdit_24"));
        lineEdit_24->setGeometry(QRect(255, 120, 113, 31));
        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        lineEdit_16 = new QLineEdit(tab_5);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));
        lineEdit_16->setGeometry(QRect(195, 20, 113, 31));
        lineEdit_17 = new QLineEdit(tab_5);
        lineEdit_17->setObjectName(QString::fromUtf8("lineEdit_17"));
        lineEdit_17->setGeometry(QRect(195, 50, 113, 31));
        lineEdit_18 = new QLineEdit(tab_5);
        lineEdit_18->setObjectName(QString::fromUtf8("lineEdit_18"));
        lineEdit_18->setGeometry(QRect(195, 80, 113, 31));
        lineEdit_19 = new QLineEdit(tab_5);
        lineEdit_19->setObjectName(QString::fromUtf8("lineEdit_19"));
        lineEdit_19->setGeometry(QRect(195, 110, 113, 31));
        lineEdit_20 = new QLineEdit(tab_5);
        lineEdit_20->setObjectName(QString::fromUtf8("lineEdit_20"));
        lineEdit_20->setGeometry(QRect(195, 140, 113, 31));
        comboBox = new QComboBox(tab_5);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(195, 170, 113, 31));
        layoutWidget = new QWidget(tab_5);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 20, 186, 181));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_18 = new QLabel(layoutWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        verticalLayout->addWidget(label_18);

        label_19 = new QLabel(layoutWidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        verticalLayout->addWidget(label_19);

        label_20 = new QLabel(layoutWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        verticalLayout->addWidget(label_20);

        label_21 = new QLabel(layoutWidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        verticalLayout->addWidget(label_21);

        label_22 = new QLabel(layoutWidget);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        verticalLayout->addWidget(label_22);

        label_24 = new QLabel(layoutWidget);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        verticalLayout->addWidget(label_24);

        tabWidget->addTab(tab_5, QString());
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 320, 491, 101));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(100, 10, 301, 71));
        lineEdit = new QLineEdit(Widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(710, 60, 81, 31));
        lineEdit->setMinimumSize(QSize(81, 31));
        lineEdit_2 = new QLineEdit(Widget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(710, 150, 81, 31));
        lineEdit_2->setMinimumSize(QSize(81, 31));
        label_26 = new QLabel(Widget);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(10, 10, 231, 31));
        lineEdit_15 = new QLineEdit(Widget);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));
        lineEdit_15->setGeometry(QRect(710, 90, 81, 31));
        lineEdit_15->setMinimumSize(QSize(81, 31));
        lineEdit_22 = new QLineEdit(Widget);
        lineEdit_22->setObjectName(QString::fromUtf8("lineEdit_22"));
        lineEdit_22->setGeometry(QRect(710, 120, 81, 31));
        lineEdit_22->setMinimumSize(QSize(81, 31));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(515, 70, 111, 16));
        label_27 = new QLabel(Widget);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(515, 100, 111, 16));
        label_28 = new QLabel(Widget);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(515, 130, 116, 16));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(515, 160, 186, 16));
        label_23 = new QLabel(Widget);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(260, 455, 16, 16));
        label_29 = new QLabel(Widget);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(300, 455, 16, 16));
        label_30 = new QLabel(Widget);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(340, 455, 16, 16));
        label_31 = new QLabel(Widget);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(380, 455, 16, 16));
        label_32 = new QLabel(Widget);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(265, 435, 16, 15));
        label_33 = new QLabel(Widget);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(305, 435, 16, 15));
        label_34 = new QLabel(Widget);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(345, 435, 16, 15));
        label_35 = new QLabel(Widget);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(385, 435, 16, 16));
        pushButton_4 = new QPushButton(Widget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(430, 435, 81, 35));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(81, 35));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(520, 435, 81, 35));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(81, 35));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(610, 435, 81, 35));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(81, 35));
        pushButton_3 = new QPushButton(Widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(700, 435, 81, 35));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setMinimumSize(QSize(81, 35));

        retranslateUi(Widget);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("Widget", "Test Result", 0, QApplication::UnicodeUTF8));
        label_17->setText(QString());
        label_3->setText(QApplication::translate("Widget", "Start(Hz):", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Widget", "End(Hz):", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Widget", "Step(Hz):", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Widget", "Step time(s):", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Widget", "Trip value", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Widget", "Test frequence(Hz):", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Widget", "Maximum time(s):", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Widget", "Trip time", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Widget", "End frequence(Hz):", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Widget", "Start df/dt(Hz/s):", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Widget", "End df/dt(Hz/s):", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Widget", "Df/dt step(Hz/s):", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Widget", "Df/dt trip", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Widget", "Test df/dt(Hz/s):", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Widget", "End frequence(Hz):", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Widget", "Duration of end frequency(s):", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("Widget", "Timing frequence(Hz):", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Widget", "Df/dt trip time", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Widget", "Df/dt(Hz/s):", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Widget", "End frequence(Hz):", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("Widget", "Start voltage(V):", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Widget", "End voltage(V):", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("Widget", "Voltage step(V):", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("Widget", "Vpn/Vpp", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("Widget", "Voltage blocking", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label_11->setText(QString());
        label_26->setText(QApplication::translate("Widget", "Frequence Test", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Widget", "Pre fault(s):", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("Widget", "Post fault(s):", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("Widget", "Current(A):", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Widget", "Hold time after trip(s):", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("Widget", "1", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("Widget", "2", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("Widget", "3", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("Widget", "4", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("Widget", "Test", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Widget", "Help", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Widget", "Save", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Widget", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
