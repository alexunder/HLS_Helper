#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExp>
#include <QValidator>
#include <QDoubleValidator>
#include <QRegExpValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_radioButton_apfixed_double_toggled(bool checked);

    void on_radioButton_double_apfixed_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    QValidator * mHexValidator;
    QDoubleValidator * mDoubleValidator;

    //false: double   -> ap_fixed
    // true: ap_fixed -> double
    bool mDouble_Apfixed_status;
    int mDecimalWidth;
};
#endif // MAINWINDOW_H
