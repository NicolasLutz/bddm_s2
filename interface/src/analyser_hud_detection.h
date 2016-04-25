#ifndef ANALYSER_HUD_DETECTION_H
#define ANALYSER_HUD_DETECTION_H

#include "Common.h"
#include "FrameValue.h"
#include <QImage>
#include <QLabel>
#include "abstractsubanalyser.h"

class Analyser_HUD_Detection : public ASubAnalyser
{
public:
    Analyser_HUD_Detection();
    ~Analyser_HUD_Detection();

    void updateVideoDetection(uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch);
    void produceOutput();

    QImage *getImg() const;
    void debug_setImgfromFile(const QString& filename);

private:
    QImage          *m_hudMaskGreyscale;

    uint8_t         *m_previousFrame;

    uint32_t        *m_hudMask;
    unsigned int    m_frameCount;
};

#endif // ANALYSER_HUD_DETECTION_H
