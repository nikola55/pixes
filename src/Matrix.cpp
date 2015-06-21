#include "Matrix.h"

#include <cstring>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>

namespace pix
{

Matrix::Matrix(int nR, int nC, float defaultValue) :
        _nR(nR), _nC(nC), nRows(_nR), nCols(_nC), data(new float[nRows*nCols])
{
    memset(data, *((int*)&defaultValue), sizeof(float)*nRows*nCols);
}

Matrix Matrix::inverse(const Matrix &A)
{
    gsl_matrix * gsl_A = gsl_matrix_alloc(A.nRows, A.nCols);
    for (int i = 0; i < A.nRows; ++i)
    {
        for (int j = 0; j < A.nCols; ++j)
        {
            gsl_matrix_set(gsl_A, i, j, *A.element(i, j));
        }
    }

    gsl_matrix * gsl_AI = gsl_matrix_alloc(A.nRows, A.nCols);
    gsl_permutation * p = gsl_permutation_alloc(A.nRows);
    int signum;
    gsl_linalg_LU_decomp (gsl_A, p, &signum);
    gsl_linalg_LU_invert (gsl_A, p, gsl_AI);
    gsl_permutation_free(p);

    Matrix AI(A.nRows, A.nCols);
    for (int i = 0; i < AI.nRows; ++i)
    {
        for (int j = 0; j < AI.nCols; ++j)
        {
            *AI.element(i,j)=gsl_matrix_get(gsl_AI, i, j);
        }
    }
    return AI;
}

Matrix Matrix::solveLU(const Matrix &A, const Matrix &b)
{
    gsl_matrix * gsl_A = gsl_matrix_alloc(A.nRows, A.nCols);
    for (int i = 0; i < A.nRows; ++i)
    {
        for (int j = 0; j < A.nCols; ++j)
        {
            gsl_matrix_set(gsl_A, i, j, *A.element(i, j));
        }
    }

    gsl_vector * gsl_b = gsl_vector_alloc(A.nRows);
    for (int i = 0; i < A.nRows; ++i)
    {
        gsl_vector_set(gsl_b, i, *b.element(i, 0));
    }


    gsl_vector * gsl_solVec = gsl_vector_alloc(A.nRows);
    gsl_permutation * p = gsl_permutation_alloc(A.nRows);
    int s;
    gsl_linalg_LU_decomp (gsl_A, p, &s);
    gsl_linalg_LU_solve (gsl_A, p, gsl_b, gsl_solVec);

    Matrix solVec(A.nRows, 1);
    for (int i = 0; i < solVec.nRows; ++i)
    {
        *solVec.element(i, 0) = gsl_vector_get(gsl_solVec, i);
    }
    return solVec;
}

void Matrix::copyElements(const float *data)
{
    for (int i = 0; i < nRows; ++i)
    {
        for (int j = 0; j < nCols; ++j)
        {
            *element(i, j) = *(data + i*nCols + j);
        }
    }
}

}
