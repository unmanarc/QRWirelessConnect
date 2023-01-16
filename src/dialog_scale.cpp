#include "dialog_scale.h"
#include "ui_dialog_scale.h"

Dialog_Scale::Dialog_Scale(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Scale)
{
    ui->setupUi(this);
}

Dialog_Scale::~Dialog_Scale()
{
    delete ui;
}

float Dialog_Scale::divFactor()
{
    return 100.0/((float)ui->horizontalSlider->value());
}

void Dialog_Scale::on_horizontalSlider_valueChanged(int value)
{
    ui->label_curval->setText( QString().setNum(value) );
}

