#include "FrameValue.h"

FrameValue::FrameValue(const QColor & colorRGB):
averageColorRGB(colorRGB),
averageColorHSV(colorRGB.toHsv())
{


}

int FrameValue::getRed() const{
	return this-> averageColorRGB.red();
}

int FrameValue::getBlue() const{
	return this-> averageColorRGB.blue();
}

int FrameValue::getGreen() const{
	return this-> averageColorRGB.green();
}

int FrameValue::getHue() const{
	return this -> averageColorHSV.hsvHue();
}

int FrameValue::getSaturation() const{
	return this -> averageColorHSV.hsvSaturation();
}

int FrameValue::getBrightness() const{
	return this -> averageColorHSV.value();
}
