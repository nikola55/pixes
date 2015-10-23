#ifndef CONVOLVE_CONVOLVEUTILS_H_
#define CONVOLVE_CONVOLVEUTILS_H_

namespace pix
{
class Matrix;

enum EdgeDetectType {
    SOBEL, LOG
};

void convolve(const Matrix &img, const Matrix &k, Matrix &destination);

void convolveSeparable(const Matrix &src, 
					   Matrix &dst, 
					   const Matrix &vk, 
					   const Matrix &hk
					   );

void edgeDetect(const Matrix &, Matrix &, EdgeDetectType type);

void blur(const Matrix &src, Matrix &dst, int sz);

}

#endif /* CONVOLVE_CONVOLVEUTILS_H_ */
