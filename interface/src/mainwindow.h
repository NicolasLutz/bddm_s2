#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QFileInfo>

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

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_actionGod_mode_triggered();

private:

    float hudMaskDistanceCalculation(const QImage *img1, const QImage *img2) const;
    float fromIntCoordinate(int value, int size) const {return value/size;}
    int toIntCoordinate(float value, int size) const {return value*size;}
    int toPtrLocation(int x, int y, int width) const {return y*width+x;}

    //erase result fields associated with the game
    void eraseResults(bool readOnly=false);

    //Finds the game from the db using the computed hud mask and fills the empty spaces
    void findGame();

    Ui::MainWindow *ui;
    QString m_videoFilename;
    QImage m_closestHudMask;

    Analyser m_analyser;

    Analyser_Color_Detection m_acd;
    Analyser_HUD_Detection m_ahd;

    Database* m_db;

    QString m_name;
    QString m_editor;
    QString m_description;
    int m_year;
};

#endif // MAINWINDOW_H
