/*
 * Misc.h
 *
 *  Created on: May 23, 2015
 *      Author: Nikola
 */

#ifndef STRUCTS_MISC_H_
#define STRUCTS_MISC_H_
#include <math.h>

struct Vec2
{
    Vec2() : x(0), y(0) { }
    Vec2(float x, float y) :
        x(x), y(y) {  }

    inline static float length2(Vec2 vec)
    {
        return vec.x*vec.x + vec.y*vec.y;
    }
    inline static float length(Vec2 vec)
    {
        return sqrtf(length2(vec));
    }
    inline static Vec2 normalize(Vec2 vec)
    {
        float len = length(vec);
        return Vec2(vec.x/len, vec.y/len);
    }

    float x;
    float y;
};

inline Vec2 operator+(Vec2 a, Vec2 b)
{
    return Vec2(a.x+b.x, a.y+b.y);
}
inline Vec2 operator-(Vec2 a, Vec2 b)
{
    return Vec2(a.x-b.x, a.y-b.y);
}
inline Vec2 operator-(Vec2 vec)
{
    return Vec2(-vec.x, -vec.y);
}
inline float operator*(Vec2 a, Vec2 b)
{
    return a.x*b.x+a.y*b.y;
}
inline Vec2 operator*(Vec2 a, float s)
{
    return Vec2(a.x*s, a.y*s);
}
inline Vec2 operator*(float s, Vec2 a)
{
    return Vec2(a.x*s, a.y*s);
}
inline float operator==(Vec2 a, Vec2 b)
{
    return  fabs(a.x-b.x) < 1e-4 &&
            fabs(a.y-b.y) < 1e-4;
}

struct Line2
{
    Line2() { }
    Line2(Vec2 p1, Vec2 p2) :
        p1(p1), p2(p2) { }
    Vec2 p1;
    Vec2 p2;
};

inline Vec2
lineIntersect(Line2 l1, Line2 l2)
{

    float x1 = l1.p1.x;
    float y1 = l1.p1.y;
    float x2 = l1.p2.x;
    float y2 = l1.p2.y;

    float x3 = l2.p1.x;
    float y3 = l2.p1.y;
    float x4 = l2.p2.x;
    float y4 = l2.p2.y;

    float x_intersection =
        ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))
        /
        ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));

    float y_intersection =
        ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))
        /
        ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));

    return Vec2( x_intersection, y_intersection );

}

struct NormalLine2
{
public:
    NormalLine2() : r(0), theta(0) { };
    NormalLine2(float r, int theta) :
        r(r), theta(theta)
    { };

    float r;
    int theta;
};

typedef double lreal;
typedef float  real;
typedef unsigned long uint32;
typedef long int32;

const lreal _double2fixmagic = 68719476736.0*1.5;     //2^36 * 1.5,  (52-_shiftamt=36) uses limited precisicion to floor
const int32 _shiftamt        = 16;                    //16.16 fixed point representation,

#if BigEndian_
	#define iexp_				0
	#define iman_				1
#else
	#define iexp_				1
	#define iman_				0
#endif //BigEndian_

// ================================================================================================
// Real2Int
// ================================================================================================
inline int32 Real2Int(lreal val)
{
#if DEFAULT_CONVERSION
	return val;
#else
	val		= val + _double2fixmagic;
	return ((int32*)&val)[iman_] >> _shiftamt; 
#endif
}

// ================================================================================================
// Real2Int
// ================================================================================================
inline int32 Real2Int(real val)
{
#if DEFAULT_CONVERSION
	return val;
#else
	return Real2Int ((lreal)val);
#endif
}

inline float fastabs(float f) 
{int i=((*(int*)&f)&0x7fffffff);return (*(float*)&i);}

#endif /* STRUCTS_MISC_H_ */
