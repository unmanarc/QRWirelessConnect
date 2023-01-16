#ifndef DIALOG_SCALE_H
#define DIALOG_SCALE_H

#include <QDialog>

namespace Ui {
class Dialog_Scale;
}

class Dialog_Scale : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Scale(QWidget *parent = nullptr);
    ~Dialog_Scale();

    float divFactor();

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Dialog_Scale *ui;
};

#endif // DIALOG_SCALE_H
