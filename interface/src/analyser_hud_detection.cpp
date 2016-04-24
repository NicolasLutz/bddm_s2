#include "analyser_hud_detection.h"

Analyser_HUD_Detection::Analyser_HUD_Detection():
    ASubAnalyser(),
    m_hudMaskGreyscale(0),
    m_previousFrame(0),
    m_hudMask(0),
    m_frameCount(0)
{
}

Analyser_HUD_Detection::~Analyser_HUD_Detection()
{
    delete m_hudMask;
    delete m_hudMaskGreyscale;
    delete m_previousFrame;
}

void Analyser_HUD_Detection::updateVideoDetection(void * p_video_data, uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts)
{
    //static const double maxN2=sqrt(255*255*3);
    quint64 pixelCount = width * height;
    int cmpnents=(pixel_pitch/8);
    Q_ASSERT(cmpnents==3); //if this case happens; make the program more flexible!

    if(m_previousFrame==NULL)
    {
        if(m_hudMaskGreyscale!=NULL)
        {
            delete m_hudMaskGreyscale;
        }
        m_hudMask=new uint32_t[width*height];
        m_hudMaskGreyscale = new QImage(width, height, QImage::Format_Grayscale8);
        m_previousFrame = new uint8_t[width*height*cmpnents];
        memcpy(m_previousFrame, p_pixel_buffer, width*height*cmpnents*sizeof(uint8_t));
        return;
    }

    for (quint64 p = 0; p < pixelCount; ++p)
    {
        double r, g, b;
        r=(double)p_pixel_buffer[cmpnents*p]/255.0-(double)m_previousFrame[cmpnents*p]/255.0;
        g=(double)p_pixel_buffer[cmpnents*p+1]/255.0-(double)m_previousFrame[cmpnents*p+1]/255.0;
        b=(double)p_pixel_buffer[cmpnents*p+2]/255.0-(double)m_previousFrame[cmpnents*p+2]/255.0;

        m_hudMask[p]+= (r==0 && g==0 && b==0) ? 1 : 0;
    }
    m_frameCount+=1;

    memcpy(m_previousFrame, p_pixel_buffer, width*height*cmpnents*sizeof(uint8_t));
}

void Analyser_HUD_Detection::produceOutput()
{
    if(m_hudMaskGreyscale==NULL || m_hudMask==NULL)
        return;
    delete m_previousFrame;
    m_previousFrame=0;

    quint8 *hudMaskGreyScalePtr = (quint8 *)m_hudMaskGreyscale->bits();
    quint64 pixelCount = m_hudMaskGreyscale->width() * m_hudMaskGreyscale->height();
    for (quint64 p = 0; p < pixelCount; ++p)
    {
        double dValue=(double)m_hudMask[p]/m_frameCount;
        hudMaskGreyScalePtr[p]=(quint8)(dValue*255);
    }

    m_frameCount=0;
    delete m_hudMask;
    m_hudMask=0;
    delete m_previousFrame;
    m_previousFrame=0;
}

QImage *Analyser_HUD_Detection::getImg() const
{
    return m_hudMaskGreyscale;
}
