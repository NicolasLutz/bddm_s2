#ifndef ANALYSER_H
#define ANALYSER_H



#include "Common.h"
#include "VLCHandler.h"


class Analyser : public VLCHandler {
public:
    Analyser();
	~Analyser();

	virtual void cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts );


private:
};





#endif // ANALYSER_H
