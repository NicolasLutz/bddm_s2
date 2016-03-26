#ifndef FRAMEVALUE_H
#define FRAMEVALUE_H

#include "Common.h"

class FrameValue{
public:
	FrameValue(const QColor & colorRGB);


	int getRed() const;
	int getBlue() const;
	int getGreen() const;
	int getHue() const;
	int getSaturation() const;
	int getBrightness() const;

private:
	QColor averageColorRGB;
	QColor averageColorHSV;
};

#endif // FRAMEVALUE_H
