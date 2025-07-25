#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpixmap.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QScrollArea>
#include <QObject>
#include <Reader.h>
#include <Writer.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn, SIGNAL(clicked()), this, SLOT(GenerateImages()));
    connect(ui->PathButton1, SIGNAL(clicked()), this, SLOT(SelectInputPath()));
    connect(ui->PathButton2, SIGNAL(clicked()), this, SLOT(SelectOutDirPath()));
}

void MainWindow::SelectInputPath()
{
    QString FileName = QFileDialog::getOpenFileName(
                nullptr,
                "Выберите входной файл",
                "",
                "Бинарные файлы (*.bin*)"
                );
    if(!FileName.isEmpty())
    {
        ui->InputLine->setText(FileName);
    }
    else
    {
        ui->InputLine->setText("Не выбран файл");
    }
}

void MainWindow::SelectOutDirPath()
{
    QString OutDirName = QFileDialog::getExistingDirectory(
                nullptr,
                "Выберите выходную папку",
                "",
                QFileDialog::ShowDirsOnly
                );
    if(!OutDirName.isEmpty())
    {
        ui->OutputLine->setText(OutDirName);
    }
    else
    {
        ui->OutputLine->setText("Не выбрана выходная папка");
    }
}

void SelectOutDirPath();

void MainWindow::GenerateImages()
{
    std::vector<InfoString> InfoStrings = Reader::ReadBinaryFile(ui->InputLine->text().toStdString());
    Writer::MakeOutput(InfoStrings, ui->OutputLine->text().toStdString());
    ui->img1->setPixmap(QPixmap(ui->OutputLine->text() + "/Sar Beam Number.png"));
    ui->tabWidget->setStyleSheet("color: red");
    ui->img2->setPixmap(QPixmap(ui->OutputLine->text() + "/Delay Of Reflected Signal.png"));
    ui->img3->setPixmap(QPixmap(ui->OutputLine->text() + "/Period Of Repetition.png"));
    ui->img4->setPixmap(QPixmap(ui->OutputLine->text() + "/Start Delay.png"));
    ui->img5->setPixmap(QPixmap(ui->OutputLine->text() + "/End Delay.png"));
    ui->img6->setPixmap(QPixmap(ui->OutputLine->text() + "/Line Number.png"));
    ui->img7->setPixmap(QPixmap(ui->OutputLine->text() + "/Phase.png"));
    ui->img8->setPixmap(QPixmap(ui->OutputLine->text() + "/Sign.png"));
    ui->img9->setPixmap(QPixmap(ui->OutputLine->text() + "/Impulse Duration.png"));
    ui->img10->setPixmap(QPixmap(ui->OutputLine->text() + "/Frequency Deviation.png"));
    ui->img11->setPixmap(QPixmap(ui->OutputLine->text() + "/Sigma Noise.png"));
    ui->img12->setPixmap(QPixmap(ui->OutputLine->text() + "/RecATT"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
