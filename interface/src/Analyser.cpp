#include "Analyser.h"

Analyser::Analyser(){



}

Analyser::~Analyser(){
	for (size_t i = 0; i < this -> framesValuesVector.size(); i++){
		delete this -> framesValuesVector[i];
	}
}


void Analyser::cbAudioPostrender(void* p_audio_data, uint8_t * p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts ){



}

void Analyser::cbVideoPostrender(void * p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts){


	unsigned long long sumRed = 0;
	unsigned long long sumGreen = 0;
	unsigned long long sumBlue = 0;

	unsigned long long numPixels = width * height;

	size_t pixelsCounted = 0;
	for (size_t i = 0; i < size / (pixel_pitch / 8); i++){
		size_t offset = i * (pixel_pitch / 8);


		uint8_t r = p_pixel_buffer[ offset + sizeof(uint8_t) * 0 ];
		uint8_t g = p_pixel_buffer[ offset + sizeof(uint8_t) * 1 ];
		uint8_t b = p_pixel_buffer[ offset + sizeof(uint8_t) * 2 ];

		if (r == 0 && g == 0 && b == 0)
			continue;

		sumRed += r;
		sumGreen += g;
		sumBlue += b;
		pixelsCounted++;
	}

	QColor averageFrameColor = QColor::fromRgb(sumRed / pixelsCounted, sumGreen / pixelsCounted, sumBlue / pixelsCounted);
	this -> framesValuesVector.push_back(new FrameValue(averageFrameColor));

	//QColor averageFrameColor(sumRed / pixelsCounted, sumGreen / pixelsCounted, sumBlue / pixelsCounted);


	//qDebug() << averageFrameColor;

}

