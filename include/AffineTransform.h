#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H
namespace pix {

class Matrix;

void createTranslateMatrix(float tx, float ty,
						   Matrix &tm
						   ) ;

void createScaleMatrix(float sx, float sy, 
					   Matrix &sm
					   ) ;

enum Interpolation {
	NEAREST_NEIGHBOUR,
	BILINEAR,
	BICUBIC
};
					   
void affineTransform(const Matrix &src,
					 Matrix &dst,
					 const Matrix &tm,
					 Interpolation interpolationType
					 ) ;

}
#endif