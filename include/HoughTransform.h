/*
 * HoughTransform.h
 *
 *  Created on: May 23, 2015
 *      Author: Nikola
 */

#ifndef FEATURE_DETECTION_HOUGHTRANSFORM_H_
#define FEATURE_DETECTION_HOUGHTRANSFORM_H_

#include "Misc.h"
#include <vector>

namespace pix
{

class Matrix;

class HoughAccumulator
{
public:
    HoughAccumulator();
    ~HoughAccumulator();
    HoughAccumulator(int maxR, int maxTheta, int targetWidth, int targetHeight, int angleQuantization);
    HoughAccumulator(const HoughAccumulator &a);
    HoughAccumulator& operator=(const HoughAccumulator &a);

    inline void increment(int r, int theta)
    {
        int val = ++data[r*maxTheta+theta];
        val > maxVal ? maxVal = val : maxVal = maxVal-0 ;
    }
    inline int getValue(int r, int theta) const
    {
        return data[r*maxTheta+theta];
    }

    int getMaxR() const
    {
        return maxR;
    }
    int getMaxTheta() const
    {
        return maxTheta;
    }
    int getTargetWidth() const
    {
        return targetWidth;
    }
    int getTargetHeight() const
    {
        return targetHeight;
    }
    int getMaxVal() const
    {
        return maxVal;
    }
    int getAngleQuantization() const
    {
        return angleQuantization;
    }

private:

    void init(int maxR, int maxTheta, int targetWidth, int targetHeight, int maxVal, unsigned int *data);
    void copyValues(const HoughAccumulator &a);

    int maxR;
    int maxTheta;
    int maxVal;
    int targetWidth;
    int targetHeight;
    int angleQuantization;
    unsigned int *data;
};

void houghTransform(const Matrix &image, float threshold, int angleQuantization, HoughAccumulator &dst);
void getNormalLines(const HoughAccumulator &, int, std::vector<NormalLine2> &);
void getLines(const HoughAccumulator &, int, std::vector<Line2> &);


}

#endif /* FEATURE_DETECTION_HOUGHTRANSFORM_H_ */
