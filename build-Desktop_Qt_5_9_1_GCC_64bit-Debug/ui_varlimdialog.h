/********************************************************************************
** Form generated from reading UI file 'varlimdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VARLIMDIALOG_H
#define UI_VARLIMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VarLimDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;

    void setupUi(QDialog *VarLimDialog)
    {
        if (VarLimDialog->objectName().isEmpty())
            VarLimDialog->setObjectName(QStringLiteral("VarLimDialog"));
        VarLimDialog->resize(400, 110);
        QFont font;
        font.setFamily(QStringLiteral("Monospace"));
        font.setPointSize(10);
        VarLimDialog->setFont(font);
        buttonBox = new QDialogButtonBox(VarLimDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 70, 381, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(VarLimDialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 381, 54));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(VarLimDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), VarLimDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VarLimDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(VarLimDialog);
    } // setupUi

    void retranslateUi(QDialog *VarLimDialog)
    {
        VarLimDialog->setWindowTitle(QApplication::translate("VarLimDialog", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class VarLimDialog: public Ui_VarLimDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VARLIMDIALOG_H
