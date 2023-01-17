#include "mainwindow.h"
#include "qnamespace.h"
#include "qpixmap.h"
#include "ui_mainwindow.h"
#include "dialog_about.h"
#include <QMessageBox>
#include <QClipboard>
#include <QRegularExpression>

#include <QPainter>
#include <QFileDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>

#include <dialog_scale.h>
#include <qrencode.h>

// function to check password strength
int checkPasswordStrength(const QString &password)
{
    int score = 0;
    // check for length
    if (password.length() >= 14)
    {
        score++;
    }
    // check for uppercase letters
    if (password.contains(QRegularExpression("[A-Z]")))
    {
        score++;
    }
    // check for lowercase letters
    if (password.contains(QRegularExpression("[a-z]")))
    {
        score++;
    }
    // check for numbers
    if (password.contains(QRegularExpression("[0-9]")))
    {
        score++;
    }
    // check for special characters
    if (password.contains(QRegularExpression("[^A-Za-z0-9]")))
    {
        score++;
    }
    return score;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_lineEdit_Pass_1_textChanged("");
    ui->label_strength->setStyleSheet("background-color: white;");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setQRCode(const QString &text)
{
    QRcode *qr = QRcode_encodeString(text.toUtf8().constData(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qr)
    {
        QMessageBox::critical(this,"QR Generation", "Error ocurred during QR Generation (too large?).");
        return;
    }

    int qrWidth = qMin(ui->label_qr->width(), ui->label_qr->height());


    QImage qrImage = QImage(qr->width + 8, qr->width + 8, QImage::Format_RGB32);
    qrImage.fill(0xffffff);
    unsigned char *p = qr->data;
    for (int y = 0; y < qr->width; y++) {
        for (int x = 0; x < qr->width; x++) {
            qrImage.setPixel(x + 4, y + 4, ((*p & 1) ? 0x0 : 0xffffff));
            p++;
        }
    }
    QRcode_free(qr);

    auto px = QPixmap::fromImage(qrImage.scaled(qrWidth,qrWidth));

    QPainter painter(&px);


    // Draw Description Text:
    QFont font = QFont( "Monospace", 18, QFont::DemiBold );
    font.setPointSize(font.pointSize());
    QFontMetrics metrics(font);
    painter.setPen(Qt::black);
    painter.setFont(font);
    painter.drawText(  0,  0, px.size().width(), metrics.height(), Qt::AlignCenter , ui->lineEdit_SSID->text());

    ui->label_qr->setPixmap(px);
}

void MainWindow::encode()
{
/*   QString wifiString = "WIFI:S:%1;T:%2;Enc:%3;P:%4;;";
    wifiString = wifiString.arg(    ui->lineEdit_SSID->text().replace(";", "\\;").replace(":","\\:").replace("\\","\\\\"), // ESSID
                                    ui->comboBox_auth->currentText(), // AUTH..
                                    ui->comboBox_encryption->currentText(),  // ENCRYPTION...
                                    ui->lineEdit_Pass_1->text().replace(";", "\\;").replace(":","\\:").replace("\\","\\\\")
                                    ); // PASSWORD*/
    QString wifiString = "WIFI:S:%1;T:%2;P:%3;;";
    wifiString = wifiString.arg(    ui->lineEdit_SSID->text().replace(";", "\\;").replace(":","\\:").replace("\\","\\\\"), // ESSID
                                    ui->comboBox_auth->currentText(), // AUTH..
                                    ui->lineEdit_Pass_1->text().replace(";", "\\;").replace(":","\\:").replace("\\","\\\\")
                                    ); // PASSWORD

    setQRCode(wifiString);
}

void MainWindow::on_lineEdit_Pass_1_textChanged(const QString &arg1)
{
    if (isPassPhraseOK())
    {
        ui->lineEdit_Pass_1->setStyleSheet("background-color: rgb(128, 255, 128);");
        ui->lineEdit_Pass_2->setStyleSheet("background-color: rgb(128, 255, 128);");

        encode();

        int score = checkPasswordStrength(ui->lineEdit_Pass_1->text());
        // display widget with color depending on password strength
        if (score <= 2)
        {
            ui->label_strength->setStyleSheet("background-color: red;");
        }
        else if (score == 3)
        {
            ui->label_strength->setStyleSheet("background-color: orange;");
        }
        else
        {
            ui->label_strength->setStyleSheet("background-color: green;");
        }
    }
    else
    {
        ui->lineEdit_Pass_1->setStyleSheet("background-color: rgb(255, 64, 64);");
        ui->lineEdit_Pass_2->setStyleSheet("background-color: rgb(255, 64, 64);");
        ui->label_strength->setStyleSheet("background-color: white;");

        QPixmap x;
        ui->label_qr->setPixmap(x);
    }
}


void MainWindow::on_lineEdit_Pass_2_textChanged(const QString &arg1)
{
    on_lineEdit_Pass_1_textChanged("");
}

bool MainWindow::isPassPhraseOK()
{
    return ui->lineEdit_Pass_1->text() == ui->lineEdit_Pass_2->text() && !ui->lineEdit_Pass_1->text().isEmpty();
}

void MainWindow::on_actionCopy_QR_to_Clipboard_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    QImage image = ui->label_qr->pixmap().toImage();
    clipboard->setImage(image);
    QMessageBox::information(this,"Clipboard", "The current QR code was copied to the clipboard.");
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;

    Dialog_Scale scale;
    if (scale.exec() != QDialog::Accepted)
    {
        return;
    }
    auto factor = scale.divFactor();

    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, [=](QPrinter *printer){
        QPainter painter;
        if (!painter.begin(printer))
        {
            // error al iniciar la vista previa
        }
        else
        {
            QImage image = ui->label_qr->pixmap().toImage();
            // Viewport rectangle...
            QRect rect = painter.viewport();
            // Image Size.
            QSize imageSize = image.size();

            // Scale the image size to the viewport size.
            //imageSize.scale(rect.size(), Qt::KeepAspectRatio);
            imageSize.scale( (image.size().width()*2)/factor,(image.size().height()*2)/factor, Qt::KeepAspectRatio );
            // Set the viewport using the initial painter viewport... and the current image size.
            painter.setViewport(rect.x(), rect.y(), imageSize.width(), imageSize.height());
            // Set the window as the rect image...
            painter.setWindow(image.rect());
            // Draw the image...
            painter.drawImage(0, 0, image);
            painter.end();
        }
    });
    if(preview.exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("Printing"), tr("Printed."));
    }
}

void MainWindow::on_actionAbout_triggered()
{
    Dialog_About about;
    about.exec();
}


void MainWindow::on_lineEdit_SSID_textChanged(const QString &arg1)
{
    on_lineEdit_Pass_1_textChanged("");
}

void MainWindow::on_comboBox_auth_currentIndexChanged(int index)
{
    on_lineEdit_Pass_1_textChanged("");
}

void savePixmap(QLabel *label)
{
    QPixmap pixmap = label->pixmap();
    if(pixmap.isNull())
    {
        QMessageBox::warning(nullptr, "Warning", "No image to save.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Image", "", "PNG Images (*.png)");

    if (!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix().isEmpty())
        {
            fileName += ".png";
        }
        pixmap.save(fileName, "PNG");
    }
}

void MainWindow::on_actionSave_triggered()
{
    savePixmap(ui->label_qr);
}

