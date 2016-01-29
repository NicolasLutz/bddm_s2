#include "mainwindow.h"
#include <QApplication>
#include "Analyser.h"
#include "VLCPlayer.h"

int main(int argc, char *argv[]){

	/*
	Analyser analyser;
	VLCPlayer::init(&analyser);
	VLCPlayer::loadFile("test.mp4");
	VLCPlayer::release();
	*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
