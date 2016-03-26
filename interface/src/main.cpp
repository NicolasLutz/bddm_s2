#include "mainwindow.h"
#include <QApplication>
#include "Analyser.h"
#include "VLCPlayer.h"

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

	return a.exec();;
}
