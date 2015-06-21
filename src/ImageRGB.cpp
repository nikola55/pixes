#include "ImageRGB.h"
#include <cstring>

namespace pix
{
ImageRGB::ImageRGB() : Image(), bitsPerSample(0), data(0)
{ }
ImageRGB::ImageRGB(int width, int height, int rowstride) :
    Image(width, height, rowstride, 3), bitsPerSample(8),
    data(new unsigned char[height * rowstride])
{ }

ImageRGB::ImageRGB(const ImageRGB &img) :
    Image(img.width, img.height, img.rowstride, img.nChannels), bitsPerSample(8)
{
    memcpy(data, img.data, sizeof(unsigned char)*height*rowstride);
}

ImageRGB & ImageRGB::operator=(const ImageRGB &img)
{
    delete[] data;
    width = img.width;
    height = img.height;
    rowstride = img.rowstride;
    nChannels = img.nChannels;
    bitsPerSample = img.bitsPerSample;
    data = new unsigned char[height*rowstride];
    memcpy(data, img.data, sizeof(unsigned char)*height*rowstride);
    return *this;
}

ImageRGB::ImageRGB(int width, int height, int rowstride,unsigned char *data) :
    Image(width, height, rowstride, 3), bitsPerSample(8), data(data)
{ }

const unsigned char * ImageRGB::getData() const {
    return data;
}

ImageRGB::~ImageRGB()
{
    delete[] data;
}

} /* namespace pix */
