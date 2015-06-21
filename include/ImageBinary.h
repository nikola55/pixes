/*
 * ImageBinary.h
 *
 *  Created on: May 23, 2015
 *      Author: Nikola
 */

#ifndef IMAGE_IMAGEBINARY_H_
#define IMAGE_IMAGEBINARY_H_

#include "ImageGray.h"

namespace pix
{

class ImageBinary: public ImageGray
{
public:
    ImageBinary(int width, int height);
    virtual ~ImageBinary();

    enum Value
    {
        Foreground = 255,
        Background = 0
    };

};

} /* namespace pix */

#endif /* IMAGE_IMAGEBINARY_H_ */
