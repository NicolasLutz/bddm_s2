#ifndef ANALYSER_H
#define ANALYSER_H

#include <QVector>

#include "Common.h"
#include "FrameValue.h"
#include "abstractsubanalyser.h"

class Analyser {
public:
    Analyser();
    ~Analyser();

    virtual void cbVideoPostrender(uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch);

    void addAnalyser(ASubAnalyser *analyser);
    void produceOutputs();

    void analyze_video(QString const& f);

private:
    std::vector<ASubAnalyser *> m_videoAnalysers;
    QString m_videoFile;
};

#endif // ANALYSER_H
