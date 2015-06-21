/*
 * ImageConvert.h
 *
 *  Created on: May 23, 2015
 *      Author: Nikola
 */

#ifndef IMAGE_IMAGECONVERT_H_
#define IMAGE_IMAGECONVERT_H_

namespace pix
{

class ImageRGB;
class ImageGray;
class ImageBinary;
class Matrix;

void convert(const ImageRGB &, ImageGray &);
void convert(const ImageGray &, ImageRGB &);
void convert(const ImageGray &, ImageBinary &, int threshold);

void convert(const ImageGray &, Matrix &);
void convert(const Matrix &, ImageGray &);
void convert(const Matrix &r, const Matrix &g, const Matrix &b, ImageRGB &img);

} /* namespace pix */

#endif /* IMAGE_IMAGECONVERT_H_ */
