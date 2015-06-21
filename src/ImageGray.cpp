/*
 * Legacy
 */

#include "ImageGray.h"

namespace pix
{
ImageGray::ImageGray() : Image(), data(0)
{ }

ImageGray::ImageGray(int width, int height) :
    Image(width, height, width, 1), data(new unsigned char[width*height])
{ }

ImageGray::~ImageGray()
{
    delete []data;
}

} /* namespace pix */
