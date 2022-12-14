#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hls_helper.h"

#include <math.h>

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->spinBox_i->setMinimum(2);
    ui->spinBox_w->setMinimum(3);
    ui->spinBox_i->setValue(8);
    ui->spinBox_w->setValue(32);

    QRegExp regx("[A-Fa-f0-9]{8}");
    mHexValidator = new QRegExpValidator(regx, ui->lineEdit);
    ui->lineEdit->setValidator(mHexValidator);

    mDoubleValidator = new QDoubleValidator(ui->lineEdit);
    mDouble_Apfixed_status = true;

    mDecimalWidth = 8;
    ui->radioButton_apfixed_double->setChecked(true);

    ui->label_result->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

MainWindow::~MainWindow()
{
    delete mHexValidator;
    delete mDoubleValidator;
    delete ui;
}


void MainWindow::on_pushButton_clicked() {
    int W = ui->spinBox_w->value();
    int I = ui->spinBox_i->value();
    qDebug() << "W=" << W << ", I=" <<I;

    if (W <= I) {
        QMessageBox::information(this, "Error", "W can't be smaller than I!");
        return;
    }

    QString content =  ui->lineEdit->text();

    if (mDouble_Apfixed_status == true) {
        QString y = "0x";
        content.prepend(y);
        qDebug() << "content=" << content;
        bool ok;

        uint ap_value = content.toUInt(&ok, 16);
        if (ok) {
            qDebug() << "ap_value=" << ap_value;
            double value = hls_helper::hls_apfixed_to_double(ap_value, W, I);
            QString result = QString::number(value);
            ui->label_result->setText(result);
        } else {
             QMessageBox::information(this, "Error", "ap_value transfered failed!");
        }
    } else {
        double d_value = content.toDouble();
        qDebug() << "d_value=" << d_value;
        unsigned int ap_value = hls_helper::double_to_hls_apfixed(d_value, W, I);
        QString result = QString::number(ap_value, 16);
        result = result.toUpper();
        qDebug() << "result=" << result;
        int length = result.length();
        qDebug() << "length=" << length;

        int hex_width;
        if (W % 4 == 0) {
            hex_width = W / 4;
        } else {
            hex_width = W / 4 + 1;
        }

        if (length < hex_width) {
            int padding_length = hex_width - length;
            int i = 0;
            while (i < padding_length) {
                result.push_front('0');
                i++;
            }
        }
        qDebug() << "result=" << result;
        ui->label_result->setText(result);
    }
}


void MainWindow::on_radioButton_apfixed_double_toggled(bool checked)
{
    qDebug() << "checked=" << checked;
    if (checked == true) {
        ui->lineEdit->setValidator(mHexValidator);
        mDouble_Apfixed_status = true;
        ui->lineEdit->clear();
    }
}


void MainWindow::on_radioButton_double_apfixed_toggled(bool checked)
{
    qDebug() << "double to ap_fixed checked=" << checked;
    if (checked == true) {
        int W = ui->spinBox_w->value();
        int I = ui->spinBox_i->value();
        double topNumber = pow(2, I - 1) - 1;
        double bottomNumber = -pow(2, I - 1);
        mDoubleValidator->setTop(topNumber);
        mDoubleValidator->setBottom(bottomNumber);

        int decimal_bin_width = W - I;
        int decimal_width = 0;
        double atomic_number = pow(2, - decimal_bin_width);

        while (atomic_number < 1.0) {
            atomic_number *= 10;
            decimal_width++;
        }

        qDebug() << "atomic_number=" << atomic_number;
        qDebug() << "decimal_width=" << decimal_width;
        mDecimalWidth = decimal_width;

        mDoubleValidator->setDecimals(mDecimalWidth);
        ui->lineEdit->setValidator(mDoubleValidator);

        mDouble_Apfixed_status = false;
        ui->lineEdit->clear();
    }
}

