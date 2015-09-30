#include "ConvolveUtils.h"
#include "Matrix.h"
#include <cmath>

static const float gaussian_15_v[15] = {
	-0.062279,                                                                                                                                                                                                              
	-0.064104,                                                                                                                                                                                                              
	-0.065691,                                                                                                                                                                                                              
	-0.067018,                                                                                                                                                                                                              
	-0.068068,                                                                                                                                                                                                              
	-0.068829,                                                                                                                                                                                                              
	-0.069289,                                                                                                                                                                                                              
	-0.069443,                                                                                                                                                                                                              
	-0.069289,                                                                                                                                                                                                              
	-0.068829,                                                                                                                                                                                                              
	-0.068068,                                                                                                                                                                                                              
	-0.067018,                                                                                                                                                                                                              
	-0.065691,                                                                                                                                                                                                              
	-0.064104,                                                                                                                                                                                                              
	-0.062279
};

static const float gaussian_15_h[15] = {
	 -0.062279,  -0.064104,  -0.065691,  -0.067018,  -0.068068,  -0.068829,  -0.069289,  -0.069443,  -0.069289,  -0.068829,  -0.068068,  -0.067018,  -0.065691,  -0.064104,  -0.062279,
};

static const pix::Matrix gaussian15Kernel_v(15, 1, gaussian_15_v);
static const pix::Matrix gaussian15Kernel_h(1, 15, gaussian_15_h);

void gaussian(const pix::Matrix &src, pix::Matrix &dst, 
			  const pix::Matrix &vk, const pix::Matrix &hk
			  ) 
{
	 convolveSeparable(src, dst, vk, hk);
}

void pix::blur(const Matrix &src, Matrix &dst, BlurType t)
{
	switch(t) {
		case GAUSSIAN_15x15:
			gaussian(src, dst, gaussian15Kernel_v, gaussian15Kernel_h);
			break;
		default:
			break;
	}
}

