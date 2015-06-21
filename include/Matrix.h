/*
 * Matrix.h
 *
 *  Created on: May 24, 2015
 *      Author: Nikola
 */

#ifndef STRUCTS_MATRIX_H_
#define STRUCTS_MATRIX_H_

namespace pix
{

class Matrix
{
public:
    Matrix() :
        _nR(0), _nC(0), nRows(_nR), nCols(_nC), data(0)
    {
    }
    Matrix(int nR, int nC) :
        _nR(nR), _nC(nC), nRows(_nR), nCols(_nC), data(new float[_nR*_nC])
    {
    }

    Matrix(int nR, int nC, float defaultValue);

    Matrix(int nR, int nC, const float *data) :
        _nR(nR), _nC(nC), nRows(_nR), nCols(_nC), data(new float[_nR*_nC])
    {
        copyElements(data);
    }

    Matrix(const Matrix& m) :
        _nR(m.nRows), _nC(m.nCols), nRows(_nR), nCols(_nC), data(new float[_nR*_nC])
    {
        copyElements(m.data);
    }

    Matrix & operator=(const Matrix &m)
    {
        _nR = m.nRows;
        _nC = m.nCols;
        delete []data;
        data = new float[_nR*_nC];
        copyElements(m.data);
        return *this;
    }

    virtual ~Matrix()
    {
        delete []data;
    }
    inline float * operator()(int i, int j)
    {
        return data + i*_nC + j;
    }
    inline const float * operator()(int i, int j) const
    {
        return data + i*_nC + j;
    }
    inline float * element(int i, int j)
    {
        return data + i*_nC + j;
    }
    inline const float * element(int i, int j) const
    {
        return data + i*_nC + j;
    }
    const float * getData() const
    {
        return data;
    }
    static Matrix solveLU(const Matrix &A, const Matrix &b);
    static Matrix inverse(const Matrix &A);

private:
    void copyElements(const float *data) ;
    int _nR;
    int _nC;
public:
    const int &nRows, &nCols;
private:
    float * data;
};

} /* namespace pix */

#endif /* STRUCTS_MATRIX_H_ */
