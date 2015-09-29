#include "HoughTransform.h"
#include "Matrix.h"
#include <cmath>
#include <algorithm>
#include <iostream>
namespace pix
{

extern const float costable[180];
extern const float sintable[180];

static bool isMaxIn(const HoughAccumulator &a, int r, int t,
                             int trange, int rrange, int val);

 void getLines(const std::vector<NormalLine2> &a,
              int maxWidth, int maxHeigh, std::vector<Line2> &lines);

inline Vec2 trimToY(Vec2 p, Vec2 d, float ty);

void houghTransform(const Matrix &image, float threshold, int angleQuantization, HoughAccumulator &acc)
{

    int maxR = sqrt(2) * (image.nCols > image.nRows ? image.nCols : image.nRows);
    int maxTheta = 180;
    acc = HoughAccumulator(maxR*2, maxTheta, image.nCols, image.nRows, angleQuantization);

    for (int x = 2; x < image.nCols - 2; x++)
    {
        for (int y = 2; y < image.nRows - 2; y++)
        {
            float val = *image(y, x);
            if (val < threshold) continue;

            for (int theta = 0; theta < maxTheta; theta+=angleQuantization)
            {
                double sinval = sintable[theta];
                double cosval = costable[theta];
                double r = cosval * x + sinval * y + maxR;
                acc.increment(r, theta);
            }
        }
    }
}

void getNormalLines(const HoughAccumulator &a, int threshold, std::vector<NormalLine2> &normalLines)
{
    for (int r = 0; r < a.getMaxR(); ++r)
    {
        for (int theta = 0; theta < a.getMaxTheta(); theta+=a.getAngleQuantization())
        {
            int val = a.getValue(r, theta);
            if (val > threshold && isMaxIn(a, r, theta, a.getAngleQuantization(), 15, val))
            {	
                normalLines.push_back(NormalLine2(r - a.getMaxR()/2, theta-90));
            }
        }
    }
}

void getLines(const HoughAccumulator &a, int threshold, std::vector<Line2> &lines)
{
    std::vector<NormalLine2> normalLines;
    getNormalLines(a, threshold, normalLines);
    getLines(normalLines, a.getTargetWidth(), a.getTargetHeight(), lines);
}

void getLines(const std::vector<NormalLine2> &a,
              int maxWidth, int maxHeigh, std::vector<Line2> &lines)
{
    for (int i = 0; i < a.size(); ++i)
    {
        NormalLine2 line = a[i];
        int x1 = 0;
        int x2 = maxWidth - 1;
        int y1, y2;

        if(line.theta == 0)
        {
            x1 = x2 = line.r;
            y1 = 0;
            y2 = maxHeigh-1;
            lines.push_back(Line2(Vec2(x1, y1), Vec2(x2, y2)));
        }
        else
        {
            float cosTheta = costable[line.theta+90];
            float sinTheta = sintable[line.theta+90];
            y1 = (line.r / sinTheta);
            y2 = (-cosTheta /  sinTheta) * x2 + (line.r /  sinTheta);
        }
        Vec2 p1(x1, y1);
        Vec2 p2(x2, y2);
        Vec2 d = p2-p1;

        if(p1.y < 0)
        {
            p1 = trimToY(p1, d, 0);
        }
        if(p1.y > maxHeigh-1)
        {
            p1 = trimToY(p1, d, maxHeigh-1);
        }
        if(p2.y < 0)
        {
            p2 = trimToY(p2, d, 0);
        }
        if(p2.y > maxHeigh-1)
        {
            p2 = trimToY(p2, d, maxHeigh-1);
        }

        lines.push_back(Line2(p1, p2));
    }
}

static bool isMaxIn(const HoughAccumulator &a, int r, int t,
                             int trange, int rrange, int val)
{

    int startr, startt, endr, endt, ti, ri, cval;

    startr = r - rrange / 2;
    startr = startr < 0 ? 0 : startr;
    startt = t - trange / 2;
    startt = startt < 0 ? 0 : startt;

    endr = r + rrange / 2;
    endr = endr > a.getMaxR() - 1 ? a.getMaxR() - 1 : endr;
    endt = t + trange / 2;
    endt = endt > a.getMaxTheta() - 1 ? a.getMaxTheta() - 1 : endt;

    for (ri = startr; ri < endr; ++ri)
    {
        for (ti = startt; ti < endt; ++ti)
        {
            cval = a.getValue(ri, ti);
            if (cval > val)
                return false;
        }
    }
    return true;
}

inline Vec2 trimToY(Vec2 p, Vec2 d, float ty)
{
    return Vec2((p.x) + (-(((p.y) - (ty)) / (d.y)))*((d.x)), (ty));
}

HoughAccumulator::~HoughAccumulator()
{
    delete[] data;
}

HoughAccumulator::
HoughAccumulator(int maxR, int maxTheta,
               int targetWidth, int targetHeight,
               int angleQuantization
			   )
{
    init(maxR, maxTheta, targetWidth,
         targetHeight, -1, angleQuantization,
         new unsigned int[maxR*maxTheta]
		 );
		 
    for (int i = 0; i < maxR*maxTheta; ++i)
    {
        data[i]=0;
    }
}

HoughAccumulator::
HoughAccumulator(const HoughAccumulator &a)
{

    init(a.maxR, a.maxTheta, a.targetWidth,
         a.targetHeight, a.maxVal, a.angleQuantization,
         new unsigned int[a.maxR*a.maxTheta]
		 );

	std::copy(&a.data[0], &a.data[a.maxR*a.maxTheta], data);
}

HoughAccumulator& HoughAccumulator
::operator=(const HoughAccumulator &a)
{
    
	delete []data;
    init(a.maxR, a.maxTheta, a.targetWidth,
         a.targetHeight, a.maxVal, a.angleQuantization,
         new unsigned int[a.maxR*a.maxTheta]
		 );

	std::copy(&a.data[0], &a.data[a.maxR*a.maxTheta], data);
    return *this;
}


HoughAccumulator::HoughAccumulator()
{
    init(0,0,0,0,-1,1,(unsigned int*)0);
}

void HoughAccumulator
::init(int maxR, int maxTheta, int targetWidth,
       int targetHeight, int maxVal, int angleQuantization,
	   unsigned int *data)
{
    // std::cout << "init(" << 
			// maxR << ", " << 
			// maxTheta << ", " << 
			// targetWidth << ", " << 
			// targetHeight << ", " << 
			// "data" << ", " << 
			// maxVal << ")" <<  std::endl;
	
	this->maxR = maxR;
    this->maxTheta = maxTheta;
    this->targetWidth = targetWidth;
    this->targetHeight = targetHeight;
    this->data = data;
    this->maxVal = maxVal;
	this->angleQuantization = angleQuantization;
}

void HoughAccumulator
::copyValues(const HoughAccumulator &a)
{
	std::copy(&a.data[0], &a.data[a.maxR*a.maxTheta], data);
}

} /* namespace pix */
