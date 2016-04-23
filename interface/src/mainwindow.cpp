#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qmessagebox.h>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_videoFilename(),
    m_analyser(),
    m_acd(),
    m_ahd(),
    m_db(nullptr)
{
    ui->setupUi(this);

    m_ahd.setLabel(ui->label_2);

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
    m_videoFilename=QFileDialog::getOpenFileName(this, tr("Open Video"),
                                                 "/home",
                                                 tr("Videos (*.mp4 *.avi *.ogv)"));
    ui->lineEdit->setText(m_videoFilename);
    if(!m_videoFilename.isEmpty())
    {
        VLCPlayer::loadFile(m_videoFilename);
        VLCPlayer::release();

        m_analyser.produceOutputs();
    }
}

void MainWindow::on_actionNew_triggered()
{
    QString dbName = QFileDialog::getOpenFileName(this, tr("Create database"),
                                            "",
                                            "");
    try {
        Database::create(dbName);
    }
    catch(std::runtime_error& e) {
        std::ostringstream oss;
        oss << "Could not create database : " << e.what();
        QMessageBox::critical(this, "Database error", QString::fromStdString(oss.str()));
    }
}

void MainWindow::on_pushButton_clicked()
{
    on_actionLoad_triggered();
}
