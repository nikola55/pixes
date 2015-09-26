#include <string>
#include <vector>
#include <iostream>

#include <cstdlib>

#include <Matrix.h>
#include <ImageIO.h>
#include <AffineTransform.h>

using namespace std;
using namespace pix;

void uErr(const char *msg) {
	cerr << msg << endl;
	exit(1);
}

struct FileName {
	string name;
	string ext;
	string path;
};

FileName fileName(const string &path) {
	
	int start, end;
	
	start = path.find_last_of("/");
	
	if((start = path.find_last_of("/")) != string::npos ||
		(start = path.find_last_of("\\")) != string::npos
		) {
		start += 1;
	} else {
		start = 0;
	}
	
	if(!((end = path.find_last_of(".")) != string::npos &&
		start < end)
		) {
		end = path.length();
	}
	
	FileName fn = {
		path.substr(start, end-start),
		path.substr(end, path.length()-end),
		path.substr(0, start)
	};
	
	return fn;
}

int main(int argc, char * argv[]) {
	
	if(argc < 3) 
		uErr("rescale SIZE file[, file, ...]");
	
	int newSize = atoi(argv[1]);
	
	if(newSize <= 0)
		uErr("Not valid newSize");
	
	cout << "New size: " << newSize << endl;
	
	Matrix  dstR(newSize, newSize),
			dstG(newSize, newSize),
			dstB(newSize, newSize);
	
	cout << "Scale:" << endl;
	
	float sz = newSize;
	
	for(int i = 2 ; i < argc ; i++) {
	
		string filePath = argv[i];
		FileName fn = fileName(filePath);
		
		Matrix r, g, b;
		loadFromFile(filePath.c_str(), r, g, b);
		
		cout << "\tImage: " << filePath << "[ "
			 << r.nCols << ", " << r.nRows << " ]" << endl;
		
		float fszw = r.nCols/sz;
		float fszh = r.nRows/sz;
		
		cout << "\tScale : " << fszw << " " << fszh << endl;
		
		Matrix scale;
		createScaleMatrix(fszw, fszh, scale);
		
		affineTransform(r, dstR, 
						scale, 
						NEAREST_NEIGHBOUR
						);
						
		affineTransform(g, dstG, 
						scale, 
						NEAREST_NEIGHBOUR
						);
						
		affineTransform(b, dstB, 
						scale, 
						NEAREST_NEIGHBOUR
						);
		
		saveToFile(dstR, dstG, dstB, filePath.c_str(), "jpeg");
	}
}
