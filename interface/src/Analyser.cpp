#include "Analyser.h"

Analyser::Analyser(): m_videoAnalysers()
{}

Analyser::~Analyser()
{}


void Analyser::cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts )
{

}

void Analyser::cbVideoPostrender(void * p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts)
{
    for(ASubAnalyser *asa : m_videoAnalysers)
    {
        if(asa!=NULL)
            asa->updateVideoDetection(p_video_data, p_pixel_buffer, width, height, pixel_pitch, size, pts);
    }
}

void Analyser::addAnalyser(ASubAnalyser *analyser)
{
    m_videoAnalysers.push_back(analyser);
}

void Analyser::produceOutputs()
{
    for(ASubAnalyser *asa : m_videoAnalysers)
    {
        if(asa!=NULL)
            asa->produceOutput();
    }
}
