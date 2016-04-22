#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_videoFilename(),
    m_analyser(),
    m_acd(),
    m_ahd()
{
    ui->setupUi(this);

    VLCPlayer::init(&m_analyser);
    m_analyser.addAnalyser(&m_acd);
    m_analyser.addAnalyser(&m_ahd);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered()
{
    m_videoFilename=QFileDialog::getOpenFileName(this);
    VLCPlayer::loadFile(m_videoFilename);
    VLCPlayer::release();

    m_analyser.produceOutputs();
}
