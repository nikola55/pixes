#ifndef IMAGE_IMAGEIO_H_
#define IMAGE_IMAGEIO_H_

namespace pix
{
class ImageRGB;
class Matrix;

void loadFromFile(const char * fileName, ImageRGB &img);
void loadFromFile(const char * fileName, Matrix &gray);
void loadFromFile(const char * fileName, Matrix &r, Matrix &g, Matrix &b);
void saveToFile(const ImageRGB &img, const char * format, const char * fileName);
void saveToFile(const Matrix &r, const Matrix &g, const Matrix &b, const char * fileName, const char * format);
}

#endif
