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
        ui->label_2->setPixmap(QPixmap::fromImage(*(m_ahd.getImg())));
        m_ahd.getImg()->save("output.png");
    }
}

void MainWindow::on_pushButton_clicked()
{
    on_actionLoad_triggered();
}

float MainWindow::hudMaskDistanceCalculation(QImage *img1, QImage *img2) const
{
    Q_ASSERT(img1!=NULL && img2!=NULL
            && img1->format()==QImage::Format_Grayscale8
            && img2->format()==QImage::Format_Grayscale8);

    quint8 *img1Ptr = (quint8 *) img1->bits();
    quint8 *img2Ptr = (quint8 *) img2->bits();

    quint64 pixelCount = img1->width() * img1->height();

    for (quint64 p = 0; p < pixelCount; p++)
    {

    }
}

void MainWindow::on_actionNew_triggered()
{
    QString dbName = QFileDialog::getSaveFileName(this, "Create database", ".");
    if(dbName.isEmpty())
        return;

    try {
        Database::create(dbName);
    }
    catch(std::runtime_error& e) {
        std::ostringstream oss;
        oss << "Could not create database : " << e.what();
        QMessageBox::critical(this, "Database error", QString::fromStdString(oss.str()));
    }
}

void MainWindow::on_actionOpen_database_triggered()
{
    QString dbName = QFileDialog::getOpenFileName(this, "Open database", ".");
    if(dbName.isEmpty())
        return;

    if(m_db) {
        delete m_db;
        m_db = nullptr;
    }

    try {
        m_db = new Database(dbName);
    }
    catch(std::runtime_error& e) {
        std::ostringstream oss;
        oss << "Could not open database : " << e.what();
        QMessageBox::critical(this, "Database error", QString::fromStdString(oss.str()));
    }
}
