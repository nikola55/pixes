#ifndef TRANSFORM_PERSPECTIVETRANSFORM_H_
#define TRANSFORM_PERSPECTIVETRANSFORM_H_

#include "Misc.h"
#include "Matrix.h"

namespace pix
{

class ImageRGB;


class PerspectiveTransform
{
public:

    PerspectiveTransform(Vec2 sp1, Vec2 sp2, Vec2 sp3, Vec2 sp4,
                         Vec2 dp1, Vec2 dp2, Vec2 dp3, Vec2 dp4);
    virtual ~PerspectiveTransform();

    Matrix getTransform() const
    {
        return transform;
    }
    Matrix getInverseTransform() const
    {
        return inverseTransform;
    }
    void unwrapImage(const Matrix &src, Matrix &dest);

private:

    void createTransform(Vec2 sp1, Vec2 sp2, Vec2 sp3, Vec2 sp4,
                         Vec2 tp1, Vec2 tp2, Vec2 tp3, Vec2 tp4);

    Vec2 sourcePoints[4];
    Vec2 destinationsPoints[4];

    Matrix transform;
    Matrix inverseTransform;
};

} /* namespace pix */

#endif /* TRANSFORM_PERSPECTIVETRANSFORM_H_ */
