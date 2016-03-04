#ifndef VLCHANDLER_H
#define VLCHANDLER_H

#include <vlc/vlc.h>

#include "Common.h"

class VLCHandler {
public:
	VLCHandler();


	///
	/// \brief Called when binded to the VLCPlayer after decoding each audio part of the media.
	/// \param p_audio_data
	/// \param p_pcm_buffer
	/// \param channels
	/// \param rate
	/// \param nb_samples
	/// \param bits_per_sample
	/// \param size
	/// \param pts
	///
	virtual void cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts );


	///
	/// \brief Called when binded to the VLCPlayer after deconding each video frame.
	/// \param p_video_data
	/// \param p_pixel_buffer
	/// \param width
	/// \param height
	/// \param pixel_pitch
	/// \param size
	/// \param pts
	///
	virtual void cbVideoPostrender(void *p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts);


	///
	/// \brief Called after some events (end of media...)
	/// \param pEvt
	/// \param pUserData
	///
	virtual void handleEvent(const libvlc_event_t * pEvt, void * pUserData);
};

#endif // VLCHANDLER_H
