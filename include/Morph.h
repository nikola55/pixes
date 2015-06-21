#ifndef MORPH_H
#define MORPH_H

namespace pix {

    class Matrix;

    void erode(const Matrix &src, float thres,
           const Matrix &str, Matrix &dst);

    void dilate(const Matrix &src, float thres,
            const Matrix &str, Matrix &dst);

}

#endif // MORPH_H
