#include <Matrix.h>
#include <AffineTransform.h>
#include <Misc.h>

void pix::createScaleMatrix(float sx, float sy, 
							pix::Matrix &sm
							) 
{
	if(sm.nCols != 3 || sm.nRows != 3)
		sm = Matrix(3,3,0.0f);
	*sm(0,0)=sx;
	*sm(1,1)=sy;
	*sm(2,2)=1.0f;
}

void createTranslateMatrix(float tx, float ty,
						   pix::Matrix &tm
						   )
{
	if(tm.nCols != 3 || tm.nRows != 3)
		tm = pix::Matrix(3,3,0.0f);

	*tm(0, 2) = tx;
	*tm(1, 2) = ty;
	
	*tm(0, 0) = 1.0f;
	*tm(1, 1) = 1.0f;
	*tm(2, 2) = 1.0f;
}

inline float bilinear(float sx, float sy, const pix::Matrix &si){ return 0.0f; }
inline float nearestNeighbour(float sx, float sy, const pix::Matrix &si)
{ 
	int ix = Real2Int(sx);
	int iy = Real2Int(sy);
	return *si(iy, ix);
}
inline float bicubic(float sx, float sy, const pix::Matrix &si){ return 0.0f; }


void pix::affineTransform(const Matrix &src,
						  Matrix &dst,
						  const Matrix &tm,
						  Interpolation iType
						  ) 
{
	
	const float *m = tm(0,0);
	for(int x = 0 ; x < dst.nCols ; x++) {
		for(int y = 0 ; y < dst.nRows ; y++) {
			
			float nx = m[0]*x + m[1]*y + m[2];
			float ny = m[3]*x + m[4]*y + m[5];
			float nw = m[6]*x + m[7]*y + m[8];
			nx/=nw;
			ny/=nw;
			
			if(0 > nx || nx >= src.nCols ||
			   0 > ny || ny >= src.nRows)
				continue;
			
			float newVal;
			
			switch(iType) {
			case pix::NEAREST_NEIGHBOUR :
				newVal = nearestNeighbour(nx, ny, src);
				break;
			case pix::BILINEAR :
				newVal = bilinear(nx, ny, src);
				break;
			case pix::BICUBIC :
				newVal = bicubic(nx, ny, src);
				break;
			}
						
		    *dst(y,x)=newVal;
		}
	}
}