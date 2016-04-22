#ifndef ANALYSER_H
#define ANALYSER_H

#include <QVector>

#include "Common.h"
#include "VLCHandler.h"
#include "FrameValue.h"
#include "abstractsubanalyser.h"

class Analyser : public VLCHandler {
public:
    Analyser();
	~Analyser();

	virtual void cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts );
	virtual void cbVideoPostrender(void *p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts);

    void addAnalyser(ASubAnalyser *analyser);
    void produceOutputs();

private:

    std::vector<ASubAnalyser *> m_videoAnalysers;
};

#endif // ANALYSER_H
