/*
 * ImageGray.h
 *
 *  Created on: May 23, 2015
 *      Author: Nikola
 */

#ifndef IMAGE_IMAGEGRAY_H_
#define IMAGE_IMAGEGRAY_H_

#include "Image.h"

namespace pix
{

class ImageGray: public Image
{
public:
    ImageGray();
    ImageGray(int width, int height);
    inline virtual unsigned char * getPixel(int x, int y)
    {
        return &data[y*width+x];
    }
    inline virtual const unsigned char * getPixel(int x, int y) const
    {
        return &data[y*width+x];
    }
    inline virtual unsigned char * operator()(int x, int y)
    {
        return &data[y*width+x];
    }
    inline virtual const unsigned char * operator()(int x, int y) const
    {
        return &data[y*width+x];
    }

    virtual ~ImageGray();
private:
    unsigned char *data;
};

} /* namespace pix */

#endif /* IMAGE_IMAGEGRAY_H_ */
