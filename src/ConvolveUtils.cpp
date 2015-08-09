#include "ConvolveUtils.h"
#include "Matrix.h"

#define ACCUMULATE(kernel, img, x, y, hw, hh, OUT_val) \
	do { \
		float accumulator = 0; \
		int xstart = x - hw; \
		int ystart = y - hh; \
		int xend = x + hw; \
		int yend = y + hh; \
		for (int iy = ystart; iy <= yend; iy++) { \
			for (int ix = xstart; ix <= xend; ix++) { \
				float p = *img(iy, ix); \
				float kv = *kernel(iy - ystart, ix - xstart); \
				float value = p * kv; \
				accumulator += value; \
			} \
		} \
		OUT_val[0]=accumulator; \
	} while(0)

namespace pix
{


void convolve(const Matrix &img, const Matrix &k, Matrix &gray)
{

    int kwh = k.nCols/2;
    int khh = k.nRows/2;

    for (int x = kwh; x < img.nCols-kwh; ++x)
    {
        for (int y = khh; y < img.nRows-khh; ++y)
        {
            float a[1];
            ACCUMULATE(k, img, x, y, kwh, khh, a);
            *gray(y, x)=*a;
        }
    }

    int tc;
    for(int y = khh ; y < img.nRows - khh ; y++)
    {
        for(int x = 0 ; x < kwh ; x++)
        {
            float a_left[1], a_right[1];
            ACCUMULATE(k, img, x+kwh, y, kwh, khh, a_left);
            ACCUMULATE(k, img, img.nCols-1-x-kwh, y, kwh, khh, a_right);
            *gray(y, x)=*a_left;
            *gray(y, img.nCols-1-x)=*a_right;
        }
    }

    for(int x = kwh ; x < img.nCols - kwh ; x++)
    {
        for(int y = 0 ; y < khh ; y++)
        {
            float a_top[1], a_bottom[1];
            ACCUMULATE(k, img, x, y + khh, kwh, khh, a_top);
            ACCUMULATE(k, img, x, img.nRows - 1 - y - khh, kwh, khh, a_bottom);
            *gray(y, x)=*a_top;
            *gray(img.nRows - 1 - y, x)=*a_bottom;
        }
    }
}
} /* namespace pix */