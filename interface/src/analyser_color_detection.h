#ifndef ANALYSER_COLOR_DETECTION_H
#define ANALYSER_COLOR_DETECTION_H

#include "abstractsubanalyser.h"
#include "FrameValue.h"

class Analyser_Color_Detection : public ASubAnalyser
{

public:
    Analyser_Color_Detection();
    ~Analyser_Color_Detection();

    void updateVideoDetection(uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch);
    void produceOutput();

private:

    QColor averageColor;
    QVector<FrameValue*> framesValuesVector;
};

#endif // ANALYSER_COLOR_DETECTION_H
