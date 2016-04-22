#ifndef ABSTRACTSUBANALYSER_H
#define ABSTRACTSUBANALYSER_H

#include "Common.h"

class ASubAnalyser
{
public:
    ASubAnalyser();

    virtual void updateVideoDetection(void * p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts);
    virtual void produceOutput();

private:
};

#endif // ABSTRACTSUBANALYSER_H
