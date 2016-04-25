#include "analyser_color_detection.h"

Analyser_Color_Detection::Analyser_Color_Detection():
    ASubAnalyser()
{

}

Analyser_Color_Detection::~Analyser_Color_Detection()
{
    /*
    for (size_t i = 0; i < this -> framesValuesVector.size(); i++){
        delete this -> framesValuesVector[i];
    }
    */
}

void Analyser_Color_Detection::updateVideoDetection(uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch)
{
    /*
     *We used to check the colors!
     *
    if(framesValuesVector.size()==0)
        qDebug() << "pixel pitch is " << pixel_pitch;

    unsigned long long sumRed = 0;
    unsigned long long sumGreen = 0;
    unsigned long long sumBlue = 0;

    size_t pixelsCounted = 0;
    for (int i = 0; i < size / (pixel_pitch / 8); i++){
        int offset = i * (pixel_pitch / 8);


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

    if(pixelsCounted!=0)
    {
        QColor averageFrameColor = QColor::fromRgb(sumRed / pixelsCounted, sumGreen / pixelsCounted, sumBlue / pixelsCounted);
        this -> framesValuesVector.push_back(new FrameValue(averageFrameColor));
    }
    */
}

void Analyser_Color_Detection::produceOutput()
{

}
