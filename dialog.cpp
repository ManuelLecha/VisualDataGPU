#include "dialog.h"
#include "ui_dialog.h"
#include "iostream"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->doubleSpinBox->setMinimum(-10);
    ui->doubleSpinBox_2->setMinimum(-10);
    ui->doubleSpinBox_3->setMinimum(-10);
    ui->doubleSpinBox_4->setMinimum(-10);

    ui->doubleSpinBox->setMaximum(10);
    ui->doubleSpinBox_2->setMaximum(10);
    ui->doubleSpinBox_3->setMaximum(10);
    ui->doubleSpinBox_4->setMaximum(10);

    ui->doubleSpinBox->setValue(-1);
    ui->doubleSpinBox_3->setValue(-1);

    ui->doubleSpinBox_2->setValue(1);
    ui->doubleSpinBox_4->setValue(1);

    ui->lineEdit->setText("://resources/data/prova.txt");
    ui->lineEdit_2->setText("://resources/models/sphere0.obj");
    ui->lineEdit_3->setText("://resources/textures/mosaic.png");
    ui->lineEdit_4->setText("inferno");
}

Dialog::~Dialog()
{
    delete ui;
}
//Okei button
void Dialog::on_pushButton_clicked()
{
    emit acceptatOK(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->lineEdit_4->text(), ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value(), ui->doubleSpinBox_3->value(), ui->doubleSpinBox_4->value());
    emit cancelOK();
}
//Cancel button
void Dialog::on_pushButton_2_clicked()
{
    std::cout<<"Cancel"<<std::endl;
    emit cancelOK();
}
