/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created: Wed Jan 18 14:41:30 2017
**      by: Qt User Interface Compiler version 4.7.4
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGroupBox *groupBox;
    QListWidget *listWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_4;
    QComboBox *comboBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton_6;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_2;
    QListWidget *listWidget_2;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_3;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_11;
    QSpacerItem *verticalSpacer_4;
    QPushButton *pushButton_5;
    QSpacerItem *verticalSpacer_5;
    QPushButton *pushButton_4;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_10;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_9;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_7;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_8;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(840, 540);
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 30, 401, 421));
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(190, 80, 171, 311));
        listWidget->setDragEnabled(true);
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 30, 351, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        comboBox = new QComboBox(horizontalLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);

        verticalLayoutWidget = new QWidget(groupBox);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 90, 161, 271));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(158, 36));

        verticalLayout->addWidget(pushButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton_6 = new QPushButton(verticalLayoutWidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setMinimumSize(QSize(158, 36));

        verticalLayout->addWidget(pushButton_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(420, 30, 411, 421));
        listWidget_2 = new QListWidget(groupBox_2);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));
        listWidget_2->setGeometry(QRect(180, 30, 171, 371));
        listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
        listWidget_2->setProperty("showDropIndicator", QVariant(true));
        listWidget_2->setDragEnabled(false);
        listWidget_2->setDragDropMode(QAbstractItemView::DragDrop);
        listWidget_2->setDefaultDropAction(Qt::MoveAction);
        verticalLayoutWidget_2 = new QWidget(groupBox_2);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 70, 160, 321));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_3 = new QPushButton(verticalLayoutWidget_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setMinimumSize(QSize(158, 36));

        verticalLayout_2->addWidget(pushButton_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        pushButton_11 = new QPushButton(verticalLayoutWidget_2);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
        sizePolicy.setHeightForWidth(pushButton_11->sizePolicy().hasHeightForWidth());
        pushButton_11->setSizePolicy(sizePolicy);
        pushButton_11->setMinimumSize(QSize(158, 36));

        verticalLayout_2->addWidget(pushButton_11);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        pushButton_5 = new QPushButton(verticalLayoutWidget_2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        sizePolicy.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy);
        pushButton_5->setMinimumSize(QSize(158, 36));

        verticalLayout_2->addWidget(pushButton_5);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);

        pushButton_4 = new QPushButton(verticalLayoutWidget_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(158, 36));

        verticalLayout_2->addWidget(pushButton_4);

        horizontalLayoutWidget_2 = new QWidget(Widget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(220, 470, 601, 43));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_10 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        sizePolicy.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
        pushButton_10->setSizePolicy(sizePolicy);
        pushButton_10->setMinimumSize(QSize(121, 41));

        horizontalLayout_2->addWidget(pushButton_10);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_9 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        sizePolicy.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy);
        pushButton_9->setMinimumSize(QSize(121, 41));

        horizontalLayout_2->addWidget(pushButton_9);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_7 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        sizePolicy.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy);
        pushButton_7->setMinimumSize(QSize(121, 41));

        horizontalLayout_2->addWidget(pushButton_7);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_8 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        sizePolicy.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy);
        pushButton_8->setMinimumSize(QSize(121, 41));

        horizontalLayout_2->addWidget(pushButton_8);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("Widget", "Add test function:", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Current", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Widget", "Voltage", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Widget", "Direction", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Widget", "Frequncy", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Widget", "Phase", 0, QApplication::UnicodeUTF8)
        );
        pushButton->setText(QApplication::translate("Widget", "Edit test function ", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("Widget", "Delete test function", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Widget", "Clear all\n"
" test function ", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        pushButton_3->setText(QApplication::translate("Widget", "Save section", 0, QApplication::UnicodeUTF8));
        pushButton_11->setText(QApplication::translate("Widget", "Edit section", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("Widget", "Delete section", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("Widget", "Clear setction", 0, QApplication::UnicodeUTF8));
        pushButton_10->setText(QApplication::translate("Widget", "Manage setting", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("Widget", "Open \n"
"template file ", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("Widget", "Save \n"
"template File", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("Widget", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
