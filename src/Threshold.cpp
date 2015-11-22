#include "Threshold.h"
#include "Matrix.h"
#include "Misc.h"

#include <cstring>
#include <cmath>
#include <inttypes.h>

void clusteringThreshold(const pix::Matrix &src, pix::Matrix &dest, int niters) ;
void otsuThreashold(const pix::Matrix &img, pix::Matrix &dst);

void pix::threshold(const Matrix &src,
					Matrix &dst,
					ThresholdParams t
					)
{
	if(dst.nRows != src.nRows || dst.nCols != src.nCols)
	{
		dst = Matrix(src.nRows, src.nCols);
	}

	switch(t.type) {
	case CLUSTERING :
		clusteringThreshold(src, dst, 10);
		break;
    case OTSU:
        otsuThreashold(src, dst);
        break;
	default :
		break;
	}

}

void assignClasses(const pix::Matrix &src, char * assignments,
				float clusterCenter1, float clusterCenter2) {

	for(int y = 0 ; y < src.nRows ; y++) {
		int rOffset = y*src.nCols;
		for(int x = 0 ; x < src.nCols ; x++) {
			float val = *src(y, x);
			float distc1 = fabs(val-clusterCenter1);
			float distc2 = fabs(val-clusterCenter2);
			assignments[rOffset+x] = distc1 > distc2;
		}
	}
}

void recalculateCenters(const pix::Matrix &src, char * assignments, float *carr) {
	float sumcarr[2] = { 0.0f, 0.0f };
	int cass[2] = { 0, 0 };
	for(int y = 0 ; y < src.nRows ; y++) {
		int rOffset = y*src.nCols;
		for(int x = 0 ; x < src.nCols ; x++) {
			int c = assignments[rOffset+x];
			sumcarr[c] += *src(y, x);
			cass[c]++;
		}
	}
	carr[0] = sumcarr[0] / cass[0];
	carr[1] = sumcarr[1] / cass[1];
}

void clusteringThreshold(const pix::Matrix &src, pix::Matrix &dest, int niters) {

	if(src.nCols != dest.nCols || src.nRows != dest.nRows) {
		return;
	}

	char * assignments = new char[src.nCols*src.nRows];
	memset(assignments, 0, (src.nCols*src.nRows)*sizeof(char));

	float cc1 = 0.0f, cc2 = 255.0f;

	for(int i = 0 ; i < niters ; i++) {
		assignClasses(src, assignments, cc1, cc2);
		float carr[2] = { cc1, cc2 };
		recalculateCenters(src, assignments, carr);
		cc1 = carr[0];
		cc2 = carr[1];
	}

	float vals[2] = { 0, 255 };

	for(int y = 0 ; y < src.nRows ; y++) {
		int rOffset = y*src.nCols;
		for(int x = 0 ; x < src.nCols ; x++) {
			*dest(y, x) = vals[assignments[rOffset+x]];
		}
	}

	delete []assignments;
}

void computeHistogram(const pix::Matrix &img, uint32_t *hist) {
    memset(hist, 0, sizeof(uint32_t)*256);
    for(int x = 0 ; x <  img.nCols ; x++) {
        for(int y = 0 ; y < img.nRows ; y++) {
            uint8_t v  = (*img(y,x) < 0.0) ? (0) : ((*img(y,x) > 255.0) ? (255) : (Real2Int(*img(y, x))));
            hist[v]++;
        }
    }
}

// https://en.wikipedia.org/wiki/Otsu%27s_method
double thresholdLevel(uint32_t hist[], uint32_t total)
{
    uint32_t sum = 0;
    for(int i = 1 ; i < 256 ; i++) {
        sum += hist[i]*i;
    }

    uint32_t sumB = 0;
    uint32_t wB = 0;
    uint32_t wF = 0;
    double mB, mF;
    double max = 0.0;
    double between = 0.0;
    double threshold1 = 0.0;
    double threshold2 = 0.0;
    for(int i = 0 ; i < 256 ; i++) {
        wB += hist[i];
        if(wB == 0)
            continue;
        wF = total - wB;
        if(wF == 0)
            continue;
        sumB += i * hist[i];
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        between = wB * wF * (mB - mF) * (mB - mF);
        if(between >= max) {
            threshold1 = i;
            if(between > max) {
                threshold2 = i;
            }
            max = between;
        }
    }
    return (threshold1+threshold2) / 2.0;
}

void otsuThreashold(const pix::Matrix &img, pix::Matrix &dst)
{
    uint32_t hist[256];
    double prob[256];
    computeHistogram(img, hist);
    double level = thresholdLevel(hist, img.nRows*img.nCols);
    float vals[2] = {0.0f, 255.0f};
    for(int x = 0 ; x < img.nCols ; x++) {
        for(int y = 0 ; y < img.nRows ; y++) {
            *dst(y,x) = vals[*img(y,x) >= level];
        }
    }

}



