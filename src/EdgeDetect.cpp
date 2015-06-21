#include "ConvolveUtils.h"
#include "Matrix.h"
#include <cmath>

static const float sobelVertical_v[] =
{
    -0.75984,
    -1.51967,
    -0.75984,
};

static const float sobelVertical_h[] =
{
    1.31607, 0.00000, -1.31607
};

static const pix::Matrix sobelVerticalKernel_v(3, 1, sobelVertical_v);
static const pix::Matrix sobelVerticalKernel_h(1, 3, sobelVertical_h);

static const float sobelHorizontal_v[] =
{
    -1.31607,
    0.00000,
    1.31607,
};

static const float sobelHorizontal_h[] =
{
    0.75984, 1.51967, 0.75984,
};

static const pix::Matrix sobelHorizontalKernel_v(3, 1, sobelHorizontal_v);
static const pix::Matrix sobelHorizontalKernel_h(1, 3, sobelHorizontal_h);

static void sobelDetect(const pix::Matrix &src, pix::Matrix &dst)
{
    pix::Matrix work = pix::Matrix(src.nRows, src.nCols);
    pix::Matrix workV = pix::Matrix(src.nRows, src.nCols);
    pix::Matrix workH = pix::Matrix(src.nRows, src.nCols);

    convolve(src, sobelVerticalKernel_v, work);
    convolve(work, sobelVerticalKernel_h, workV);

    convolve(src, sobelHorizontalKernel_v, work);
    convolve(work, sobelHorizontalKernel_h, workH);

    for (int x = 0; x < src.nCols; ++x)
    {
        for (int y = 0; y < src.nRows; ++y)
        {
            float xgradmem = *workH(y, x);
            float ygradmem = *workV(y, x);
            *dst(y, x) = fabs(xgradmem + ygradmem);
        }
    }

}

static const float LOG_f[] =
{
    0.16667,   0.66667,   0.16667,
    0.66667,  -3.33333,   0.66667,
    0.16667,   0.66667,   0.16667
};

pix::Matrix LOGKernel(3, 3, LOG_f);

static void LOGDetect(const pix::Matrix &src, pix::Matrix &dst)
{
    convolve(src, LOGKernel, dst);
}

void pix::edgeDetect(const Matrix &src, Matrix &dst, EdgeDetectType type)
{
    if(src.nRows != dst.nRows &&
       src.nCols != dst.nCols)
    {
        dst = Matrix(src.nRows, src.nCols);
    }

    switch(type)
    {
    case SOBEL :
        sobelDetect(src, dst);
        break;
    case LOG :
        LOGDetect(src, dst);
        break;
    default:
        return;
    }
}
