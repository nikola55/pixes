#include "ConvolveUtils.h"
#include "Matrix.h"
#include <cmath>
#include <vector>

inline double g(double x, double m, double s) {
	double dx = x-m;
	return exp(-(dx*dx/2.0*s*s));
}

void mkKernel(int len, float * vals) {
	
	float sigma = 1.0;
	float mean = 0.0;
	len += 1;
	// https://en.wikipedia.org/wiki/68%E2%80%9395%E2%80%9399.7_rule
	float stx = -3*sigma;
    float enx = 3*sigma;
	float inc = (enx-stx) / len;
	
	for(int i = 1 ; i < len ; i++) {
		vals[i-1] = g(stx+i*inc, mean, sigma);
	}
	
	// normalize to one
	float sum = 0.0;
	for(int i = 0 ; i < len - 1 ; i++)
		sum += vals[i];
	
	for(int i = 0 ; i < len - 1 ; i++)
		vals[i] /= sum;
	
}


void gaussian(const pix::Matrix &src, pix::Matrix &dst, 
			  const pix::Matrix &vk, const pix::Matrix &hk
			  ) 
{
	 convolveSeparable(src, dst, vk, hk);
}

void pix::blur(const Matrix &src, Matrix &dst, int sz)
{
	if(!(sz&1)) return;
	
	std::vector<float> v(sz);
	mkKernel(sz, &v[0]);
	
	Matrix vk(sz, 1, &v[0]);
	Matrix hk(1, sz, &v[0]);
	
	gaussian(src, dst, vk, hk);
	
}

