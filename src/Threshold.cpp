#include "Threshold.h"
#include "Matrix.h"

#include <cstring>
#include <cmath>

void clusteringThreshold(const pix::Matrix &src, pix::Matrix &dest, int niters) ;

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