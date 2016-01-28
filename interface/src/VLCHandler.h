#ifndef VLCHANDLER_H
#define VLCHANDLER_H

#include <vlc/vlc.h>

#include "Common.h"

class VLCHandler {
public:
	VLCHandler();


	virtual void cbAudioPrerender(void * p_audio_data, uint8_t ** pp_pcm_buffer , unsigned int size);
	virtual void cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts );
	virtual void cbVideoPrerender(void *p_video_data, uint8_t ** pp_pixel_buffer, int size);
	virtual void cbVideoPostrender(void *p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts);
	virtual void handleEvent(const libvlc_event_t * pEvt, void * pUserData);
};

#endif // VLCHANDLER_H
