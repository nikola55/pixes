#include "ImageConvert.h"
#include "ImageRGB.h"
#include "ImageGray.h"
#include "ImageBinary.h"
#include "Matrix.h"

namespace pix
{

void convert(const ImageRGB &img, ImageGray &gray)
{
    for (int x = 0; x < img.getWidth(); x++)
    {
        for (int y = 0; y < img.getHeight(); ++y)
        {
            const unsigned char * srcmem = img.getPixel(x, y);
            float newval = srcmem[0] * 0.2126 + srcmem[1] * 0.7125 + srcmem[2] * 0.0722;
            unsigned char * destmem = gray.getPixel(x, y);
            destmem[0]=newval;
        }
    }
}

void convert(const ImageGray &img, ImageRGB &rgb)
{
    for (int x = 0; x < img.getWidth(); ++x)
    {
        for (int y = 0; y < img.getHeight(); ++y)
        {
            const unsigned char * srcmem = img.getPixel(x,y);
            unsigned char * destmem = rgb.getPixel(x,y);
            destmem[0]=destmem[1]=destmem[2]=srcmem[0];
        }
    }
}

void convert(const ImageGray &img, ImageBinary &binary, int threshold)
{
    ImageBinary::Value vals[2] =
    {
        ImageBinary::Background,
        ImageBinary::Foreground
    };
    for (int x = 0; x < img.getWidth(); ++x)
    {
        for (int y = 0; y < img.getHeight(); ++y)
        {
            *binary.getPixel(x,y)=vals[*img.getPixel(x, y) > threshold];
        }
    }
}

void convert(const ImageGray &src, Matrix &m)
{
    if(m.nCols != src.getWidth() && m.nRows != src.getHeight())
    {
        m = Matrix(src.getHeight(), src.getWidth());
    }
    for(int x = 0; x < src.getWidth() ; x++)
    {
        for(int y = 0  ; y < src.getHeight() ; y++)
        {
            *m(y,x)=*src(x,y);
        }
    }
}

void convert(const Matrix &src, ImageGray &img)
{
    if(img.getHeight() != src.nRows && img.getWidth() != src.nCols)
    {
        img = ImageGray(src.nCols, src.nRows);
    }
    for(int y = 0 ; y < src.nRows ; y++)
    {
        for(int x = 0 ; x < src.nCols ; x++)
        {
            float val = *src(y,x);
            *img(x,y)= val > 255.0f ? 255 : val < 0.0f ? 0 : val;
        }
    }
}

void convert(const Matrix &r, const Matrix &g, const Matrix &b, ImageRGB &img)
{
    for(int x = 0 ; x < img.getWidth() ; x++)
    {
        for(int y = 0 ; y < img.getHeight() ; y++)
        {
            unsigned char *memDest = img(x, y);
            float redVal = *r(y, x);
            float greenVal = *g(y, x);
            float blueVal = *b(y, x);

            memDest[0] = redVal < 0 ? 0 : redVal > 255 ? 255 : redVal;
            memDest[1] = greenVal < 0 ? 0 : greenVal > 255 ? 255 : greenVal;
            memDest[2] = blueVal < 0 ? 0 : blueVal > 255 ? 255 : blueVal;

        }
    }
}

} /* namespace pix */
