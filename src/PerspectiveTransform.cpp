/*
 * PerspectiveTransform.cpp
 *
 *  Created on: May 24, 2015
 *      Author: Nikola
 */

#include "PerspectiveTransform.h"
#include "ImageRGB.h"
#include <stdio.h>

namespace pix
{

PerspectiveTransform::PerspectiveTransform(
    Vec2 sp1, Vec2 sp2, Vec2 sp3, Vec2 sp4,
    Vec2 dp1, Vec2 dp2, Vec2 dp3, Vec2 dp4)
    : sourcePoints((Vec2[])
{
    sp1, sp2, sp3, sp4
}),
destinationsPoints((Vec2[])
{
    dp1, dp2, dp3, dp4
})
{
    createTransform(sp1, sp2, sp3, sp4,
                    dp1, dp2, dp3, dp4);
}
PerspectiveTransform::~PerspectiveTransform() { }

void PerspectiveTransform::unwrapImage(const Matrix &src, Matrix &dest)
{
	if(dest.nRows == 0 && dest.nCols == 0)
		return;
	
    Matrix &A = transform;
    for (int x = 0; x < dest.nCols; ++x)
    {
        for (int y = 0; y < dest.nRows; ++y)
        {
            float sVec3[3] = { float(x), float(y), 1.0f };
            float dVec3[3] =
            {
                *A.element(0, 0)*sVec3[0] + *A.element(0, 1)*sVec3[1] + *A.element(0, 2)*sVec3[2],
                *A.element(1, 0)*sVec3[0] + *A.element(1, 1)*sVec3[1] + *A.element(1, 2)*sVec3[2],
                *A.element(2, 0)*sVec3[0] + *A.element(2, 1)*sVec3[1] + *A.element(2, 2)*sVec3[2],
            };

            if(dVec3[0] < 0 || dVec3[0] >= src.nCols || 
			   dVec3[1] < 0 || dVec3[1] >= src.nRows) continue;

            int dx = dVec3[0]/dVec3[2];
            int dy = dVec3[1]/dVec3[2];

            *dest(y, x) = *src(dy, dx);

        }
    }
}

void PerspectiveTransform::createTransform(
    Vec2 sp1, Vec2 sp2, Vec2 sp3, Vec2 sp4,
    Vec2 dp1, Vec2 dp2, Vec2 dp3, Vec2 dp4)
{

    float AData[64] =
    {
        sp1.x,	sp1.y,	1,	0,		0,		0,	-dp1.x*sp1.x,	-dp1.x*sp1.y,
        0,		0,		0,	sp1.x, 	sp1.y, 	1, 	-dp1.y*sp1.x, 	-dp1.y*sp1.y,
        sp2.x,	sp2.y,	1,	0,		0,		0,	-dp2.x*sp2.x,	-dp2.x*sp2.y,
        0,		0,		0,	sp2.x, 	sp2.y, 	1, 	-dp2.y*sp2.x, 	-dp2.y*sp2.y,
        sp3.x,	sp3.y,	1,	0,		0,		0,	-dp3.x*sp3.x,	-dp3.x*sp3.y,
        0,		0,		0,	sp3.x, 	sp3.y, 	1, 	-dp3.y*sp3.x, 	-dp3.y*sp3.y,
        sp4.x,	sp4.y,	1,	0,		0,		0,	-dp4.x*sp4.x,	-dp4.x*sp4.y,
        0,		0,		0,	sp4.x, 	sp4.y, 	1, 	-dp4.y*sp4.x, 	-dp4.y*sp4.y,
    };

    float bData[8] =
    {
        dp1.x,
        dp1.y,
        dp2.x,
        dp2.y,
        dp3.x,
        dp3.y,
        dp4.x,
        dp4.y
    };

    Matrix A(8, 8, AData);
    Matrix b(8, 1, bData);
    Matrix solVec = Matrix::solveLU(A, b);

    float solData[9] =
    {
        *solVec.element(0, 0), *solVec.element(0, 1), *solVec.element(0, 2),
        *solVec.element(0, 3), *solVec.element(0, 4), *solVec.element(0, 5),
        *solVec.element(0, 6), *solVec.element(0, 7), 1.0f,
    };

    inverseTransform = Matrix(3, 3, solData);
    transform = Matrix::inverse(inverseTransform);

}

} /* namespace pix */
