#ifndef VLCPLAYER_H
#define VLCPLAYER_H


#include <QFile>
#include <QMutex>

#include "Common.h"
#include "VLCHandler.h"


class VLCPlayer{
public:
	///
	/// \brief init the player context, to be reader to play something.
	/// \param handler a pointer to a VLCHandler object for function callback
	///
	static void init(VLCHandler * handler = NULL);


	///
	/// \brief release the player context and free the memory
	///
	static void release();

	///
	/// \brief loadFile take a file and start the analysis
	/// \param filePath complete path of the media file to read.
	/// \return true if all is successfull.
	///
	static bool loadFile(const QString & filePath);

	///
	/// \brief change the callback object
	/// \param handler
	///
	static void setHandler(VLCHandler * handler);


	static void stop();

private:
	void static cbAudioPrerender(void * p_audio_data, uint8_t ** pp_pcm_buffer , unsigned int size);
	void static cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts );
	void static cbVideoPrerender(void *p_video_data, uint8_t ** pp_pixel_buffer, int size);
	void static cbVideoPostrender(void *p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts);
	void static handleEvent(const libvlc_event_t * pEvt, void * pUserData);


	static libvlc_instance_t * inst;				//instance of libVLC
	static libvlc_media_player_t * media_player;

	static uint8_t * videoBuffer;
	static uint8_t * audioBuffer;
	static size_t videoBufferSize;
	static size_t audioBufferSize;
	static bool playbackInProgress;
	static QMutex mutex;
	static VLCHandler * handler;
};








#endif // VLCPLAYER_H
