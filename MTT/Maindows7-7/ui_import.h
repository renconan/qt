/********************************************************************************
** Form generated from reading UI file 'import.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORT_H
#define UI_IMPORT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_Import
{
public:
    QListWidget *listWidget;

    void setupUi(QDialog *Import)
    {
        if (Import->objectName().isEmpty())
            Import->setObjectName(QString::fromUtf8("Import"));
        Import->resize(800, 480);
        listWidget = new QListWidget(Import);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(0, 0, 800, 480));

        retranslateUi(Import);

        QMetaObject::connectSlotsByName(Import);
    } // setupUi

    void retranslateUi(QDialog *Import)
    {
        Import->setWindowTitle(QApplication::translate("Import", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Import: public Ui_Import {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORT_H
