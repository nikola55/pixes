#ifndef CONVOLVE_CONVOLVEUTILS_H_
#define CONVOLVE_CONVOLVEUTILS_H_

namespace pix
{
class Matrix;

enum EdgeDetectType {
    SOBEL, LOG
};

void convolve(const Matrix &img, const Matrix &k, Matrix &destination);

void edgeDetect(const Matrix &, Matrix &, EdgeDetectType type);

}

#endif /* CONVOLVE_CONVOLVEUTILS_H_ */
