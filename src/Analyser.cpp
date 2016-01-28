#include "Analyser.h"

Analyser::Analyser(){



}

Analyser::~Analyser(){

}


void Analyser::cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts ){

	qDebug() << "XD";
}
