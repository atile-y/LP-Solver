#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "poblacion.h"

#include <QMainWindow>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addRestPushButton_clicked();
    void on_runPushButton_clicked();
    void on_generacionLineEdit_editingFinished();
    void on_poblacionLineEdit_editingFinished();
    void on_precisionLineEdit_editingFinished();

    void removeRest();

private:
    Ui::MainWindow *ui;

    Poblacion *poblacion;
    uint m_nPrecision;
    QVector<QLabel *> m_vRLabel;
    QVector<QLineEdit *> m_vRLineEdit;
    QVector<QPushButton *> m_vRPushButton;
    QIcon m_removeIcon;
};

#endif // MAINWINDOW_H
