#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>

#include "Common.h"
#include "Analyser.h"
#include "VLCPlayer.h"

#include "analyser_color_detection.h"
#include "analyser_hud_detection.h"
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLoad_triggered();

    //void on_actionNew_triggered();

    void on_pushButton_clicked();

    void on_actionNew_triggered();

    void on_actionOpen_database_triggered();

    private:

    float hudMaskDistanceCalculation(QImage *img1, QImage *img2) const;
    float fromIntCoordinate(int value, int size) const {return value/size;}
    int toIntCoordinate(float value, int size) const {return value*size;}
    int toPtrLocation(int x, int y, int width) const {return y*width+x;}

    Ui::MainWindow *ui;
    QString m_videoFilename;

    Analyser m_analyser;

    Analyser_Color_Detection m_acd;
    Analyser_HUD_Detection m_ahd;

    Database* m_db;
};

#endif // MAINWINDOW_H
