#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "Analyser.h"
#include "VLCPlayer.h"

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

    void on_actionCompute_similarity_spectrum_triggered();
    
private:
    Ui::MainWindow *ui;
    QString m_videoFilename;
};

#endif // MAINWINDOW_H
