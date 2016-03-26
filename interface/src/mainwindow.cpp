#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_videoFilename()
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered()
{
    m_videoFilename=QFileDialog::getOpenFileName(this);
    VLCPlayer::loadFile("test.mp4");
}

void MainWindow::on_actionCompute_similarity_spectrum_triggered()
{
}
