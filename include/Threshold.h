#ifndef THRESHOLD_
#define THRESHOLD_

namespace pix
{

class Matrix;

enum ThresholdType {
	CLUSTERING,
	OTSU
};

struct ThresholdParams {
	ThresholdType type;
	int threshold;
	ThresholdParams(ThresholdType type) :
		type(type), threshold(127) {
	}
	ThresholdParams(ThresholdType type, int threshold) :
		type(type), threshold(threshold) {
	}
};

void threshold(const Matrix &src,
			   Matrix &dst,
			   ThresholdParams t
			   );

}

#endif
