#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qrencode.h>

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
    void on_lineEdit_Pass_1_textChanged(const QString &arg1);
    void on_lineEdit_Pass_2_textChanged(const QString &arg1);

    void on_actionCopy_QR_to_Clipboard_triggered();

    void on_actionPrint_triggered();

    void on_actionAbout_triggered();


    void on_lineEdit_SSID_textChanged(const QString &arg1);

    void on_comboBox_auth_currentIndexChanged(int index);

    void on_actionSave_triggered();

private:

    void encode();
    void setQRCode(const QString &text);
    bool isPassPhraseOK();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
