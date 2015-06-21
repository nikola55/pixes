/*
 * ImageRGB.h
 *
 *  Created on: May 23, 2015
 *      Author: Nikola
 */

#ifndef IMAGE_IMAGERGB_H_
#define IMAGE_IMAGERGB_H_

#include "Image.h"

namespace pix
{

class ImageRGB: public Image
{
    friend class ImageIO;
public:

    ImageRGB();
    ImageRGB(int width, int height, int rowstride);
    ImageRGB(int width, int height, int rowstride, unsigned char *data);
    ImageRGB(const ImageRGB &);

    ImageRGB & operator=(const ImageRGB &);

    inline virtual unsigned char * getPixel(int x, int y)
    {
        return &data[y*rowstride + x*nChannels];
    }
    inline virtual const unsigned char * getPixel(int x, int y) const
    {
        return &data[y*rowstride + x*nChannels];
    }
    const unsigned char * getData() const ;
    inline virtual unsigned char * operator()(int x, int y)
    {
        return &data[y*rowstride + x*nChannels];
    }
    inline virtual const unsigned char * operator()(int x, int y) const
    {
        return &data[y*rowstride + x*nChannels];
    }

    virtual ~ImageRGB();
private:

    int bitsPerSample;
    unsigned char * data;

};

} /* namespace pix */

#endif /* IMAGE_IMAGERGB_H_ */
