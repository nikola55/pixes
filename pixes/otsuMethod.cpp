#include <string>
#include <vector>
#include <iostream>

#include <cstdlib>
#include <cstdio>

#include <Pixes.h>

#define DUMP_MATRIX(mat, nrows, ncols) \
	do { \
		int __i; int __j; \
		for(__i = 0 ; __i < (nrows) ; __i++) { \
			for(__j = 0 ; __j < (ncols) ; __j++) {\
				printf("%f ", mat[__i*(ncols)+__j]);\
			}\
			printf("\n");\
		}\
	 } while(0);

using namespace std;
using namespace pix;

void uErr(const char *msg) {
	cerr << msg << endl;
	exit(1);
}

void histogram(const Matrix &img,
			   vector<double> &hist
			   )
{
	if(hist.size()<256) {
		hist = vector<double>(256, 0.0);
	}
	
	for(int x = 0 ; x < img.nCols ; x++) {
		for(int y = 0 ; y < img.nRows ; y++) {
			int i = Real2Int(*img(y,x));
			hist[i]++;
		}
	}
	
	float total = img.nRows*img.nCols;
	
	for(int i = 0 ; i < 256 ; i++) {
		hist[i]/=total;
	}
	
	for(int i = 1 ; i < 256 ; i++) {
		hist[i]+=hist[i-1];
	}
	
	for(int i = 0 ; i < 256 ; i++) {
		hist[i] = round(hist[i]*255);
	}
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

void equalize(Matrix &img, const vector<double> &hist) {
	for(int x = 0 ; x < img.nCols; x++) {
		for(int y = 0 ; y < img.nRows ; y++) {
			int i = Real2Int(*img(y,x));
			*img(y,x)=hist[i];
		}
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		uErr("empty argv array\n");
	}
	
	for(int i = 1 ; i < argc ; i++) {
	
		FileName fn = fileName(argv[i]);
		Matrix img;
		loadFromFile(argv[i], img);
		
		std::vector<double> hist;
		histogram(img, hist);
		
		equalize(img, hist) ;
		
		string filename = fn.path + "eq_" + fn.name + fn.ext;
		
		saveToFile(img, img, img, filename.c_str(), "jpeg");
		
	}
}