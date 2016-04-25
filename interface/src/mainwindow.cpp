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
    const QString dbName="game.db";

    QFileInfo check_file(dbName);
    if(!check_file.exists() || !check_file.isFile())
    {
        try
        {
            Database::create(dbName);
        }
        catch(std::runtime_error& e)
        {
            std::ostringstream oss;
            oss << "Failed to create database : " << e.what();
            QMessageBox::critical(this, "Database error", QString::fromStdString(oss.str()));
        }
    }

    try
    {
        m_db = new Database("game.db");
    }
    catch(std::runtime_error& e)
    {
        std::ostringstream oss;
        oss << "Failed to open database : " << e.what();
        QMessageBox::critical(this, "Database error", QString::fromStdString(oss.str()));
    }

    ui->setupUi(this);

    ui->widget->hide();
    ui->horizontalWidget_answer->hide();
    ui->horizontalWidget_submit->hide();

    VLCPlayer::init(&m_analyser);
    m_analyser.addAnalyser(&m_acd);
    m_analyser.addAnalyser(&m_ahd);
}

MainWindow::~MainWindow()
{
    VLCPlayer::release();
    delete ui;
}

void MainWindow::on_actionLoad_triggered()
{
    m_videoFilename=QFileDialog::getOpenFileName(this, tr("Open Video"),
                                                 "/home",
                                                 tr("Videos (*.mp4 *.avi *.ogv)"));
    ui->lineEdit->setText(m_videoFilename);
}

void MainWindow::on_pushButton_clicked()
{
    on_actionLoad_triggered();
}

float MainWindow::hudMaskDistanceCalculation(const QImage *img1, const QImage *img2) const
{
    Q_ASSERT(img1!=NULL && img2!=NULL
            && img1->format()==QImage::Format_Grayscale8
            && img2->format()==QImage::Format_Grayscale8);

    int distance=0;
    quint8 *img1Ptr;
    quint8 *img2Ptr;
    int width, height;

    img1Ptr=(quint8 *) img1->bits();
    QImage img2Scaled=img2->scaled(img1->size());
    img2Ptr=(quint8 *) img2Scaled.bits();
    width= img1->width();
    height=img1->height();

    for (int x=0; x<width; ++x)
    {
        for (int y=0; y<height; ++y)
        {
            distance+=std::abs(img2Ptr[toPtrLocation(x, y, width)]-img1Ptr[toPtrLocation(x, y, width)]);
        }
    }
    return (float)distance/(width*height);
}

void MainWindow::on_pushButton_4_clicked() //Yes
{
    //We're happy
    ui->horizontalWidget_answer->hide();
    ui->widget->hide();

    eraseResults();
}

void MainWindow::on_pushButton_5_clicked() //No
{
    //We're sad and we ask for knowledge
    ui->widget->show();
    ui->horizontalWidget_answer->hide();
    ui->horizontalWidget_submit->show();

    eraseResults(false);
}

void MainWindow::on_pushButton_2_clicked() //Don't know
{
    //We're happy too
    on_pushButton_4_clicked();
}

void MainWindow::on_pushButton_3_clicked() //Results
{
    if(!m_videoFilename.isEmpty())
    {
        VLCPlayer::loadFile(m_videoFilename);

        m_analyser.produceOutputs();
        if(m_ahd.getImg()!=NULL)
        {
            ui->label_2->setPixmap(QPixmap::fromImage(*(m_ahd.getImg())));
            m_ahd.getImg()->save("output.png");
        }

        findGame();

        ui->widget->show();
        ui->horizontalWidget_answer->show();

    }
}

void MainWindow::on_pushButton_6_clicked() //Submit
{
    QImage emptyImage;
    QString *editor=!ui->lineEdit_editor->text().isEmpty() ? new QString(ui->lineEdit_editor->text()) : NULL;
    QString *description=!ui->lineEdit_desc->text().isEmpty() ? new QString(ui->lineEdit_desc->text()) : NULL;
    int *year=new int(ui->spinBox_year->value());
    Game newGame(ui->lineEdit_game->text(), *m_ahd.getImg(), editor, description, emptyImage, year);
    try{
        m_db->insert_game(newGame);
        ui->horizontalWidget_submit->hide();
        ui->widget->hide();
        eraseResults(true);
    }
    catch(std::runtime_error& e){
        std::ostringstream oss;
        oss << "Failed to submit : " << e.what();
        QMessageBox::critical(this, "Database error", QString::fromStdString(oss.str()));
    }
}

void MainWindow::eraseResults(bool readOnly)
{
    ui->lineEdit_game->setText(QString(""));
    ui->lineEdit_editor->setText(QString(""));
    ui->spinBox_year->setValue(QDate::currentDate().year());
    ui->lineEdit_desc->setText(QString(""));

    ui->lineEdit_game->setReadOnly(readOnly);
    ui->lineEdit_editor->setReadOnly(readOnly);
    ui->spinBox_year->setReadOnly(readOnly);
    ui->lineEdit_desc->setReadOnly(readOnly);
}

void MainWindow::findGame()
{
    float minDist=std::numeric_limits<float>::max();
    std::vector<QString> games=m_db->games();

    if(games.size()==0)
        return;

    Game fetchGame;
    Game bestGame;

    for(QString gameString: games)
    {
        fetchGame=m_db->game(gameString);
        float currentDist=hudMaskDistanceCalculation(m_ahd.getImg(), &(fetchGame.analysis()));
        if(minDist>currentDist)
        {
            minDist=currentDist;
            bestGame=fetchGame;
        }
    }

    ui->lineEdit_game->setText(bestGame.name());
    ui->lineEdit_editor->setText(bestGame.editor()!=NULL ? *bestGame.editor() : QString(""));
    ui->spinBox_year->setValue(bestGame.year()!=NULL ? *bestGame.year() : QDate::currentDate().year());
    ui->lineEdit_desc->setText(bestGame.description()!=NULL ? *bestGame.description() : QString(""));
}

void MainWindow::on_actionGod_mode_triggered()
{
    m_ahd.debug_setImgfromFile("output.png");
    ui->label_2->setPixmap(QPixmap::fromImage(*(m_ahd.getImg())));
    on_pushButton_5_clicked();
}

void MainWindow::on_actionLoad_fake_comparasion_triggered()
{
    m_ahd.debug_setImgfromFile("output.png");
    findGame();
}

void MainWindow::on_actionTest_distance_with_two_images_triggered()
{
    m_ahd.debug_setImgfromFile("output.png");
    QImage *img=new QImage("lol.png");

    qDebug() << hudMaskDistanceCalculation(m_ahd.getImg(), img);
    delete img;
}
