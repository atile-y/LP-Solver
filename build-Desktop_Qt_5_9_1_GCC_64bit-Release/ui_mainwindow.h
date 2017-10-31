/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *PLGroupBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *zLabel;
    QPushButton *addRestPushButton;
    QLineEdit *zLineEdit;
    QLabel *saLabel;
    QGroupBox *geneticGroupBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *generacionLabel;
    QLineEdit *generacionLineEdit;
    QLabel *poblacionLabel;
    QLineEdit *poblacionLineEdit;
    QLabel *presicionLabel;
    QLineEdit *precisionLineEdit;
    QRadioButton *maxRadioButton;
    QRadioButton *minRadioButton;
    QLabel *aptitudLabel;
    QPushButton *runPushButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(451, 451);
        QFont font;
        font.setFamily(QStringLiteral("Monospace"));
        font.setPointSize(10);
        MainWindow->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PLGroupBox = new QGroupBox(centralWidget);
        PLGroupBox->setObjectName(QStringLiteral("PLGroupBox"));
        PLGroupBox->setGeometry(QRect(10, 10, 431, 161));
        gridLayoutWidget = new QWidget(PLGroupBox);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 30, 411, 122));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        zLabel = new QLabel(gridLayoutWidget);
        zLabel->setObjectName(QStringLiteral("zLabel"));

        gridLayout->addWidget(zLabel, 0, 0, 1, 1);

        addRestPushButton = new QPushButton(gridLayoutWidget);
        addRestPushButton->setObjectName(QStringLiteral("addRestPushButton"));
        addRestPushButton->setMaximumSize(QSize(16777215, 24));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addRestPushButton->setIcon(icon);
        addRestPushButton->setIconSize(QSize(24, 24));
        addRestPushButton->setFlat(true);

        gridLayout->addWidget(addRestPushButton, 2, 1, 1, 1);

        zLineEdit = new QLineEdit(gridLayoutWidget);
        zLineEdit->setObjectName(QStringLiteral("zLineEdit"));

        gridLayout->addWidget(zLineEdit, 0, 1, 1, 2);

        saLabel = new QLabel(gridLayoutWidget);
        saLabel->setObjectName(QStringLiteral("saLabel"));
        saLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(saLabel, 1, 0, 1, 3);

        geneticGroupBox = new QGroupBox(centralWidget);
        geneticGroupBox->setObjectName(QStringLiteral("geneticGroupBox"));
        geneticGroupBox->setGeometry(QRect(10, 180, 431, 181));
        formLayoutWidget = new QWidget(geneticGroupBox);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 30, 411, 137));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        generacionLabel = new QLabel(formLayoutWidget);
        generacionLabel->setObjectName(QStringLiteral("generacionLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, generacionLabel);

        generacionLineEdit = new QLineEdit(formLayoutWidget);
        generacionLineEdit->setObjectName(QStringLiteral("generacionLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, generacionLineEdit);

        poblacionLabel = new QLabel(formLayoutWidget);
        poblacionLabel->setObjectName(QStringLiteral("poblacionLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, poblacionLabel);

        poblacionLineEdit = new QLineEdit(formLayoutWidget);
        poblacionLineEdit->setObjectName(QStringLiteral("poblacionLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, poblacionLineEdit);

        presicionLabel = new QLabel(formLayoutWidget);
        presicionLabel->setObjectName(QStringLiteral("presicionLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, presicionLabel);

        precisionLineEdit = new QLineEdit(formLayoutWidget);
        precisionLineEdit->setObjectName(QStringLiteral("precisionLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, precisionLineEdit);

        maxRadioButton = new QRadioButton(formLayoutWidget);
        maxRadioButton->setObjectName(QStringLiteral("maxRadioButton"));
        maxRadioButton->setChecked(true);

        formLayout->setWidget(3, QFormLayout::FieldRole, maxRadioButton);

        minRadioButton = new QRadioButton(formLayoutWidget);
        minRadioButton->setObjectName(QStringLiteral("minRadioButton"));

        formLayout->setWidget(4, QFormLayout::FieldRole, minRadioButton);

        aptitudLabel = new QLabel(formLayoutWidget);
        aptitudLabel->setObjectName(QStringLiteral("aptitudLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, aptitudLabel);

        runPushButton = new QPushButton(centralWidget);
        runPushButton->setObjectName(QStringLiteral("runPushButton"));
        runPushButton->setGeometry(QRect(210, 380, 32, 32));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        runPushButton->setIcon(icon1);
        runPushButton->setIconSize(QSize(32, 32));
        runPushButton->setFlat(true);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Gen\303\251tico aplicado a problemas PL", Q_NULLPTR));
        PLGroupBox->setTitle(QApplication::translate("MainWindow", "Problema PL", Q_NULLPTR));
        zLabel->setText(QApplication::translate("MainWindow", "Z =", Q_NULLPTR));
        addRestPushButton->setText(QString());
        zLineEdit->setText(QString());
        zLineEdit->setPlaceholderText(QApplication::translate("MainWindow", "a + b", Q_NULLPTR));
        saLabel->setText(QApplication::translate("MainWindow", "Sujeto a", Q_NULLPTR));
        geneticGroupBox->setTitle(QApplication::translate("MainWindow", "Algoritmo Gen\303\251tico", Q_NULLPTR));
        generacionLabel->setText(QApplication::translate("MainWindow", "Generaciones", Q_NULLPTR));
        generacionLineEdit->setText(QApplication::translate("MainWindow", "50", Q_NULLPTR));
        generacionLineEdit->setPlaceholderText(QApplication::translate("MainWindow", "N\303\272mero de iteraciones", Q_NULLPTR));
        poblacionLabel->setText(QApplication::translate("MainWindow", "Poblaci\303\263n", Q_NULLPTR));
        poblacionLineEdit->setText(QApplication::translate("MainWindow", "100", Q_NULLPTR));
        poblacionLineEdit->setPlaceholderText(QApplication::translate("MainWindow", "Tama\303\261o de la poblaci\303\263n", Q_NULLPTR));
        presicionLabel->setText(QApplication::translate("MainWindow", "Precisi\303\263n", Q_NULLPTR));
        precisionLineEdit->setText(QApplication::translate("MainWindow", "5", Q_NULLPTR));
        precisionLineEdit->setPlaceholderText(QApplication::translate("MainWindow", "Bits de precisi\303\263n", Q_NULLPTR));
        maxRadioButton->setText(QApplication::translate("MainWindow", "Maximizar", Q_NULLPTR));
        minRadioButton->setText(QApplication::translate("MainWindow", "Minimizar", Q_NULLPTR));
        aptitudLabel->setText(QApplication::translate("MainWindow", "Aptitud (Z)", Q_NULLPTR));
        runPushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
