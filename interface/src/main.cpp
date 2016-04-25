#include "mainwindow.h"
#include <QApplication>
#include "Analyser.h"

// Dirty workaround libav being stuck in the 1970's
extern "C" {
#include <libavformat/avformat.h>
}

int main(int argc, char *argv[])
{
    // Init FFmpeg
    av_register_all();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();;
}
