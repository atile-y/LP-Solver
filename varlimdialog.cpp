#include "varlimdialog.h"
#include "ui_varlimdialog.h"

#include <QDebug>
#include <QLabel>

VarLimDialog::VarLimDialog(QString var, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VarLimDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Limites de las variables");

    int numVar = var.length();

    for(int i=0;i<numVar;i++){
        QLineEdit *lineEdit = new QLineEdit(ui->gridLayoutWidget);
        lineEdit->setPlaceholderText("-inf");
        lineEdit->setValidator(new QIntValidator(lineEdit));
        ui->gridLayout->addWidget(lineEdit, i, 0, 1, 1);
        m_vMin.append(lineEdit);

        QLabel *label = new QLabel(tr("<= %1 <=").arg(var.at(i)), ui->gridLayoutWidget);
        ui->gridLayout->addWidget(label, i, 1, 1, 1);

        lineEdit = new QLineEdit(ui->gridLayoutWidget);
        lineEdit->setPlaceholderText("+inf");
        lineEdit->setValidator(new QIntValidator(lineEdit));
        ui->gridLayout->addWidget(lineEdit, i, 2, 1, 1);
        m_vMax.append(lineEdit);
    }

    int offset = 30;
    this->setFixedHeight(50 + numVar * offset);
    ui->buttonBox->move(10, 10 + numVar * offset);
    ui->gridLayoutWidget->resize(ui->gridLayoutWidget->width(), 27 * numVar);
}

VarLimDialog::~VarLimDialog(){
    delete ui;
}

void VarLimDialog::accept(){
    for(int i=0;i<m_vMin.size();i++){
        long inf = (m_vMin.at(i)->text() == "" ? INT_MIN : m_vMin.at(i)->text().toInt());
        long sup = (m_vMax.at(i)->text() == "" ? INT_MAX : m_vMax.at(i)->text().toInt());
        m_vData.append(QPair<long,long>(inf, sup));
    }

    QDialog::accept();
}
