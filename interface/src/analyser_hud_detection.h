#ifndef ANALYSER_HUD_DETECTION_H
#define ANALYSER_HUD_DETECTION_H

#include "Common.h"
#include "VLCHandler.h"
#include "FrameValue.h"
#include <QImage>
#include <QLabel>
#include "abstractsubanalyser.h"

class Analyser_HUD_Detection : public ASubAnalyser
{
public:
    Analyser_HUD_Detection(QLabel* label=0);

    void updateVideoDetection(void * p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts);
    void produceOutput();

    void setLabel(QLabel* label);

private:
    QImage          *m_hudMaskGreyscale;

    uint8_t         *m_previousFrame;

    uint32_t        *m_hudMask;
    unsigned int    m_frameCount;
    QLabel          *m_label;
};

#endif // ANALYSER_HUD_DETECTION_H
