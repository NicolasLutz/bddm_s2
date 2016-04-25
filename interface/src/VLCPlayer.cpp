
#include "VLCPlayer.h"





uint8_t * VLCPlayer::videoBuffer = NULL;
uint8_t * VLCPlayer::audioBuffer = NULL;
size_t VLCPlayer::videoBufferSize = 0;
size_t VLCPlayer::audioBufferSize = 0;
libvlc_instance_t * VLCPlayer::inst = NULL;
libvlc_media_player_t * VLCPlayer::media_player = NULL;
bool VLCPlayer::playbackInProgress = false;
QMutex VLCPlayer::mutex;
VLCHandler * VLCPlayer::handler = NULL;


void VLCPlayer::release(){
    VLCPlayer::mutex.lock();

    if (VLCPlayer::inst == NULL){
        qDebug() << "VLCPlayer already released !";
        VLCPlayer::mutex.unlock();
        return;
    }
    //Free the media_player
    libvlc_media_player_release (VLCPlayer::media_player);

    //release the context
    libvlc_release (VLCPlayer::inst);

    VLCPlayer::inst = NULL;
    VLCPlayer::handler = NULL;

    VLCPlayer::mutex.unlock();
}

void VLCPlayer::init(VLCHandler * handler){
    VLCPlayer::mutex.lock();

    VLCPlayer::handler = handler;


    if (VLCPlayer::inst != NULL){
        qDebug() << "VLCPlayer already initialized !";
        VLCPlayer::mutex.unlock();
        return ;
    }

    char smem_options[1000];

    // VLC options
    sprintf(smem_options
      , "#transcode{vcodec=RV24,acodec=s16l}:smem{"
         "video-prerender-callback=%lld,"
         "video-postrender-callback=%lld,"
         "audio-prerender-callback=%lld,"
         "audio-postrender-callback=%lld},"
      , (long long int)(intptr_t)(void*)&VLCPlayer::cbVideoPrerender
      , (long long int)(intptr_t)(void*)&VLCPlayer::cbVideoPostrender
      , (long long int)(intptr_t)(void*)&VLCPlayer::cbAudioPrerender
      , (long long int)(intptr_t)(void*)&VLCPlayer::cbAudioPostrender); /*
      , (long long int)100		//This would normally be useful data, 100 is just test data
      , (long long int)200);	//Test data
*/
    const char * const vlc_args[] = {
          "-I", "dummy", // Don't use any interface
          "--ignore-config", // Don't use VLC's config
          "--extraintf=logger", // Log anything
          "--verbose=1", // Be verbose
          "--sout", smem_options // Stream to memory
           };

    //initialize the VLC context
    VLCPlayer::inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    //Generate a new Media Player
    VLCPlayer::media_player = libvlc_media_player_new(VLCPlayer::inst);

    VLCPlayer::mutex.unlock();
}






bool VLCPlayer::loadFile(const QString & filePath){
    VLCPlayer::mutex.lock();

    if (VLCPlayer::playbackInProgress == true){
        qDebug() << "Playback already in progress !";
        VLCPlayer::mutex.unlock();
        return false;
    }

    void *pUserData = 0;


    libvlc_event_manager_t* eventManager = libvlc_media_player_event_manager(VLCPlayer::media_player);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerTimeChanged, handleEvent, pUserData);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached, handleEvent, pUserData);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerPositionChanged, handleEvent, pUserData);


    QFile mediaFile(filePath);
    if (!mediaFile.open(QIODevice::ReadOnly)){
        qDebug() << "Unable to open the file : " << filePath;
        VLCPlayer::mutex.unlock();
        return false;
    }

    libvlc_media_t * media = libvlc_media_new_fd(VLCPlayer::inst, mediaFile.handle());
    libvlc_media_player_set_media(VLCPlayer::media_player, media);
    libvlc_media_player_play(VLCPlayer::media_player);


    VLCPlayer::playbackInProgress = true;
    VLCPlayer::mutex.unlock();

    while(VLCPlayer::playbackInProgress);
    mediaFile.close();

    return true;
}

void VLCPlayer::setHandler(VLCHandler * handler){
    VLCPlayer::handler = handler;
}

void VLCPlayer::stop(){
    libvlc_media_player_stop(VLCPlayer::media_player);
}

// Audio prerender callback
void VLCPlayer::cbAudioPrerender (void * p_audio_data, uint8_t ** pp_pcm_buffer , unsigned int size){
    mutex.lock();

    if (size > VLCPlayer::audioBufferSize || !VLCPlayer::audioBuffer){
        qDebug() << "Reallocate raw audio buffer";
        if(VLCPlayer::audioBuffer) delete[] VLCPlayer::audioBuffer;
        VLCPlayer::audioBuffer = new uint8_t[size];
        VLCPlayer::audioBufferSize = size;
    }
    *pp_pcm_buffer = VLCPlayer::audioBuffer;
}

// Audio postrender callback
void VLCPlayer::cbAudioPostrender(void * p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts ){

    mutex.unlock();
    if (VLCPlayer::handler)
        VLCPlayer::handler -> cbAudioPostrender(p_audio_data, p_pcm_buffer, channels, rate, nb_samples, bits_per_sample, size, pts);
}

void VLCPlayer::cbVideoPrerender(void * p_video_data, uint8_t **pp_pixel_buffer, int size) {
    mutex.lock();

    if (size > VLCPlayer::videoBufferSize || !VLCPlayer::videoBuffer){
        qDebug() << "Reallocate raw video buffer\n";
        if(VLCPlayer::audioBuffer) delete [] VLCPlayer::videoBuffer;
        VLCPlayer::videoBuffer = new uint8_t[size];
        VLCPlayer::videoBufferSize = size;
    }
    *pp_pixel_buffer = VLCPlayer::videoBuffer;
}

void VLCPlayer::cbVideoPostrender(void * p_video_data, uint8_t *p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts) {

    mutex.unlock();

    if (VLCPlayer::handler)
        VLCPlayer::handler -> cbVideoPostrender(p_video_data, p_pixel_buffer, width, height, pixel_pitch, size, pts);
}

void VLCPlayer::handleEvent(const libvlc_event_t * pEvt, void * pUserData){
    libvlc_time_t time;
    switch(pEvt -> type){
    case libvlc_MediaPlayerTimeChanged:
        time = libvlc_media_player_get_time(VLCPlayer::media_player);
        break;
    case libvlc_MediaPlayerEndReached:
        qDebug() << "MediaPlayerEndReached";
        VLCPlayer::mutex.lock();
        VLCPlayer::playbackInProgress = false;
        VLCPlayer::mutex.unlock();
        break;
    default:
        printf("%s\n", libvlc_event_type_name(pEvt->type));
    }

    if (VLCPlayer::handler)
        VLCPlayer::handler -> handleEvent(pEvt, pUserData);
}

