#include "VLCHandler.h"

VLCHandler::VLCHandler(){

}

void VLCHandler::cbAudioPostrender(void * p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts){

}

void VLCHandler::cbVideoPostrender(void * p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts){

}

void VLCHandler::handleEvent(const libvlc_event_t * pEvt, void * pUserData){

}
