#ifndef ABSTRACTSUBANALYSER_H
#define ABSTRACTSUBANALYSER_H

#include "Common.h"

class ASubAnalyser
{
public:
    ASubAnalyser();
    virtual ~ASubAnalyser();

    virtual void updateVideoDetection(uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch);
    virtual void produceOutput();

private:
};

#endif // ABSTRACTSUBANALYSER_H
