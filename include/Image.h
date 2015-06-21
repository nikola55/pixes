/*
 * Image.h
 *
 *  Created on: May 23, 2015
 *      Author: Nikola
 */

#ifndef IMAGE_IMAGE_H_
#define IMAGE_IMAGE_H_

namespace pix
{

class Image
{
public:
    Image() : width(0), height(0),
        nChannels(0), rowstride(0), dataLength(0)
    {  }
    Image(int width, int height, int rowstride, int nChannels)
        :  width(width), height(height), nChannels(nChannels),
           rowstride(rowstride), dataLength(height*rowstride)
    { }
    inline virtual unsigned char * getPixel(int x, int y) = 0;
    inline virtual const unsigned char * getPixel(int x, int y) const = 0;

    inline virtual unsigned char * operator()(int x, int y) = 0;
    inline virtual const unsigned char * operator()(int x, int y) const = 0;

    virtual int getWidth() const
    {
        return width;
    }
    virtual int getHeight() const
    {
        return height;
    }
    virtual int getNChanels() const
    {
        return nChannels;
    }
    virtual int getRowstride() const
    {
        return rowstride;
    }
    virtual int getDataLength() const
    {
        return dataLength;
    }
    virtual ~Image() { } ;
protected :
    int width, height;
    int nChannels;
    int rowstride;
    int dataLength;
};

}

#endif /* IMAGE_IMAGE_H_ */
