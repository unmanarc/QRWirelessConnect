#ifndef DIALOG_ABOUT_H
#define DIALOG_ABOUT_H

#include <QDialog>

namespace Ui {
class Dialog_About;
}

class Dialog_About : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_About(QWidget *parent = nullptr);
    ~Dialog_About();

private:
    Ui::Dialog_About *ui;
};

#endif // DIALOG_ABOUT_H
