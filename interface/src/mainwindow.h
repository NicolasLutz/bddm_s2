#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "Common.h"
#include "Analyser.h"
#include "VLCPlayer.h"

#include "analyser_color_detection.h"
#include "analyser_hud_detection.h"

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
    
    void on_pushButton_clicked();

private:

    float hudMaskDistanceCalculation(QImage *img1, QImage *img2) const;

    Ui::MainWindow *ui;
    QString m_videoFilename;

    Analyser m_analyser;

    Analyser_Color_Detection m_acd;
    Analyser_HUD_Detection m_ahd;
};

#endif // MAINWINDOW_H
