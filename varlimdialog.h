#ifndef VARLIMDIALOG_H
#define VARLIMDIALOG_H

#include <QDialog>

#include <QLineEdit>

namespace Ui {
class VarLimDialog;
}

class VarLimDialog : public QDialog{
    Q_OBJECT

public:
    VarLimDialog(QString, QWidget * = 0);
    ~VarLimDialog();

    QVector< QPair<long,long> > getData(){ return m_vData; }

public slots:
    void accept();

private:
    Ui::VarLimDialog *ui;

    QVector<QLineEdit *> m_vMin;
    QVector<QLineEdit *> m_vMax;
    QVector< QPair<long,long> > m_vData;
};

#endif // VARLIMDIALOG_H
