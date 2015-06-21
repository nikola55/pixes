#include "Morph.h"
#include "Matrix.h"

static inline bool OR(int i, int j, int hh, int hw, float thres,
                      const pix::Matrix &src, const pix::Matrix &str)
{
    for(int si = 0 ; si < str.nRows ; si++)
    {
        for(int sj = 0  ; sj < str.nCols ; sj++)
        {
            bool srcb = *src(i+si-hh, j+sj-hw) > thres;
            bool strb = *str(si, sj) > thres;
            if(srcb && strb) return true;
        }
    }
    return false;
}

static inline bool AND(int i, int j, int hh, int hw, float thres,
                       const pix::Matrix &src, const pix::Matrix &str)
{
    for(int si = 0 ; si < str.nRows ; si++)
    {
        for(int sj = 0 ; sj < str.nCols ; sj++)
        {
            if(*str(si, sj) == 0.0f) continue;
            bool strb = *str(si, sj) > thres;
            bool srcb = *src(i+si-hh, j+sj-hw) > thres;
            if(!(strb && srcb)) return false;
        }
    }
    return true;
}

void pix::dilate(const Matrix &src, float thres,
            const Matrix &str, Matrix &dst)
{
    int hh = str.nRows/2;
    int hw = str.nCols/2;

    for(int i = hh ; i  < src.nRows - hh ; i++)
    {
        for(int j = hw  ; j < src.nCols - hw ; j++)
        {
            bool b = OR(i, j, hh, hw, thres, src, str);
            *dst(i, j) = b*255;
        }
    }
}

void pix::erode(const Matrix &src, float thres,
           const Matrix &str, Matrix &dst)
{
    int hh = str.nRows/2;
    int hw = str.nCols/2;

    for(int i = hh ; i  < src.nRows - hh ; i++)
    {
        for(int j = hw  ; j < src.nCols - hw ; j++)
        {
            bool b = AND(i, j, hh, hw, thres, src, str);
            *dst(i, j) = b*255;
        }
    }
}
