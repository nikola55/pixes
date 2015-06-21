#include "ImageIO.h"
#include "ImageRGB.h"
#include "Matrix.h"

#include <stdlib.h>
#include <string.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

namespace pix
{

void loadFromFile(const char * fileName, ImageRGB &img)
{
    g_type_init();

    GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file(fileName, NULL);
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int data_length = height*rowstride;

    unsigned char *data = gdk_pixbuf_get_pixels(pixbuf);
    unsigned char *img_data = new unsigned char[data_length];

    memcpy(img_data, data, sizeof(unsigned char)*data_length);

    img = ImageRGB(width, height, rowstride, img_data);

    gdk_pixbuf_unref(pixbuf);
}

void loadFromFile(const char * fileName, Matrix &gray) {

    ImageRGB rgb;
    loadFromFile(fileName, rgb);
    gray = Matrix(rgb.getHeight(), rgb.getWidth());
    for(int x = 0 ; x < rgb.getWidth() ; x++)
    {
        for(int y = 0 ; y < rgb.getHeight() ; y++)
        {
            const unsigned char *mem = rgb(x,y);
            *gray(y,x)=mem[0] * 0.2126 + mem[1] * 0.7125 + mem[2] * 0.0722;
        }
    }
}

void loadFromFile(const char * fileName, Matrix &r, Matrix &g, Matrix &b)
{
    ImageRGB img;
    loadFromFile(fileName, img);
    r = Matrix(img.getHeight(), img.getWidth());
    g = Matrix(img.getHeight(), img.getWidth());
    b = Matrix(img.getHeight(), img.getWidth());
    for(int x = 0 ; x < img.getWidth() ; x++)
    {
        for(int y = 0 ; y < img.getHeight() ; y++)
        {
            const unsigned char *mem = img(x,y);
            *r(y, x) = mem[0];
            *g(y, x) = mem[1];
            *b(y, x) = mem[2];
        }
    }
}
void saveToFile(const ImageRGB &img, const char * format, const char * fileName)
{
    g_type_init();
    GdkPixbuf * pixbuf =
        gdk_pixbuf_new_from_data(img.getData(), GDK_COLORSPACE_RGB, 0, 8,
                                 img.getWidth(), img.getHeight(), img.getRowstride(), NULL, NULL);
    gdk_pixbuf_save(pixbuf, fileName, format, NULL, NULL, NULL, NULL);
    gdk_pixbuf_unref(pixbuf);
}
void saveToFile(const Matrix &r, const Matrix &g, const Matrix &b,
                         const char * fileName, const char * format)
{
    ImageRGB rgb(r.nCols, r.nRows, r.nCols*3);

    for(int x = 0 ; x < rgb.getWidth() ; x++)
    {
        for(int y = 0 ; y < rgb.getHeight() ; y++)
        {
            unsigned char *memDest = rgb(x, y);
            float redVal = *r(y, x);
            float greenVal = *g(y, x);
            float blueVal = *b(y, x);

            memDest[0] = redVal < 0 ? 0 : redVal > 255 ? 255 : redVal;
            memDest[1] = greenVal < 0 ? 0 : greenVal > 255 ? 255 : greenVal;
            memDest[2] = blueVal < 0 ? 0 : blueVal > 255 ? 255 : blueVal;

        }
    }
    saveToFile(rgb, format, fileName);
}

} /* namespace pix */
