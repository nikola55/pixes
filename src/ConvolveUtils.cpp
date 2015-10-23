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
				float p;\
				if(iy < 0 || iy >= img.nRows || \
				   ix < 0 || ix >= img.nCols) p = 0; \
				else p = *img(iy, ix); \
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

    for (int x = 0; x < img.nCols; ++x)
    {
        for (int y = 0; y < img.nRows; ++y)
        {
            float a[1];
            ACCUMULATE(k, img, x, y, kwh, khh, a);
            *gray(y, x)=*a;
        }
    }

    // int tc;
    // for(int y = khh ; y < img.nRows - khh ; y++)
    // {
        // for(int x = 0 ; x < kwh ; x++)
        // {
            // float a_left[1], a_right[1];
            // ACCUMULATE(k, img, x+kwh, y, kwh, khh, a_left);
            // ACCUMULATE(k, img, img.nCols-1-x-kwh, y, kwh, khh, a_right);
            // *gray(y, x)=*a_left;
            // *gray(y, img.nCols-1-x)=*a_right;
        // }
    // }

    // for(int x = kwh ; x < img.nCols - kwh ; x++)
    // {
        // for(int y = 0 ; y < khh ; y++)
        // {
            // float a_top[1], a_bottom[1];
            // ACCUMULATE(k, img, x, y + khh, kwh, khh, a_top);
            // ACCUMULATE(k, img, x, img.nRows - 1 - y - khh, kwh, khh, a_bottom);
            // *gray(y, x)=*a_top;
            // *gray(img.nRows - 1 - y, x)=*a_bottom;
        // }
    // }
}

void convolveSeparable(const Matrix &src, 
					   Matrix &dst, 
					   const Matrix &vk, 
					   const Matrix &hk
					   )
{
	if(src.nRows != dst.nRows ||
	   src.nCols != dst.nCols)
	{
		return;
	}
	
	Matrix work(src.nRows, src.nCols, 0.0f);
	convolve(src, vk, work);
	convolve(work, hk, dst);
	
}

} /* namespace pix */
