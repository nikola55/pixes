#include <string>
#include <vector>
#include <iostream>

#include <cstdlib>
#include <cstdio>

#include <fcntl.h>
#include <errno.h>

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

void writeMat(const char * fnm, const double *mat, size_t sz);
void normalize(vector<double> &vals, int rows, int cols);

void uErr(const char *msg) {
	cerr << msg << endl;
	exit(1);
}

void uWarn(const char *msg) {
	cerr << msg << endl;
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
	
	//tlebd OUT_DOUBLES, OUT_LEBELS File[, File[, File ...]]
	
	string prgNm = argv[0];
	
	if(argc < 4) {
		uErr((prgNm+" File[, File[, File ...]]").c_str());
	}
	
	int numRows = -1,
		numCols = -1;
		
	std::vector<double> vals;
	std::vector<int> lebels;
	int maxLebel = 0;
	
	cout << "Start load" << endl;
	
	for(int i = 1 ; i < argc ; i++) {
	
		string path = argv[i];
		cout << "\tFile: " << path << " ";
		FileName fn = fileName(path);
		
		string lebel = fn.path;
		if(lebel.length() > 1 && 
			lebel[lebel.length()-1]=='/'
			) {
			lebel = lebel.substr(0, lebel.length()-1);
		}
		int start = 0 ;
		if((start = lebel.find_last_of("/")) != string::npos ||
			(start = lebel.find_last_of("\\")) != string::npos
			) {
			start += 1;
		}
		lebel = lebel.substr(start, lebel.length());
		
		int intLeb = atoi(lebel.c_str());
		
		if(intLeb == 0) {
			uWarn(("Cannot convert "+lebel+" to int").c_str());
			continue;
		}
		
		cout << "Lebel :" << intLeb << endl;
		
		if(intLeb > maxLebel) {
			maxLebel = intLeb;
		}
		lebels.push_back(intLeb);
		
		Matrix image;
		loadFromFile(path.c_str(), image);
		
		if(numRows == -1 ||
			numCols == -1
			) {
			numRows = image.nRows;
			numCols = image.nCols;
		}
		
		vals.push_back(1.0);
		vals.insert(vals.end(), image(0,0), image(image.nRows-1, image.nCols-1)+1);
	}
	
	cout << "End load." << endl
		<< "Num(Loaded)=" << vals.size() 
		<< " Num(Lebels)=" << lebels.size() 
		<< " Max(Lebel)=" << maxLebel 
		<< " Dim(img)=(" << numRows << ", " << numCols << ")" 
		<< endl;
	
	cout << "Create lebel matrix" << endl;
	
	int NRows = lebels.size();
	int lebMatNCols = maxLebel;
	
	Matrix lebMat(NRows, lebMatNCols, 0.0f);
	
	for(int i = 0 ; i < NRows ; i++) {
		*lebMat(i, lebels[i]-1)=1.0f;
	}
	
	vector<double> lebData(lebMat(0,0), lebMat(lebMat.nRows, lebMat.nCols)+1);
	
	int featMatNCols = 1+numCols*numRows;
	
	normalize(vals, lebMat.nRows, featMatNCols);
	
	//DUMP_MATRIX((&vals[0]), NRows, featMatNCols);
	char tmp[50];
	string outMat = itoa(NRows,tmp,10);
	outMat += "x";
	outMat += itoa(featMatNCols,tmp,10);
	outMat += ".double";
	
	string outLeb = itoa(NRows,tmp,10);
	outLeb += "x";
	outLeb += itoa(lebMatNCols,tmp,10);
	outLeb += ".double";
	
	writeMat(outMat.c_str(), &vals[0], vals.size()*sizeof(double));
	writeMat(outLeb.c_str(), &lebData[0], lebData.size()*sizeof(double));
	
	//cout << vals.size() << " " << lebels.size() << " " << maxLebel << endl;
	return 0;
}

void calcMean(const vector<double> &vals, 
		int rows,
		int cols,
		vector<double> &means
		)
{
	means = vector<double>(cols-1, 0.0);
	
	for(int i = 0 ; i < rows ; i++) {
		int rowOff = i*cols;
		for(int j = 1 ; j < cols ; j++) {
			means[j-1] += vals[rowOff+j];
		}
	}
	
	double div = 1.0/rows; 
	for(int i = 0 ; i < means.size() ; i++) {
		means[i]*=div;
	}
}

void calcStd(const vector<double> &vals, 
		int rows,
		int cols,
		const vector<double> &means,
		vector<double> &std
		)
{
	std = vector<double>(cols-1, 0.0);
	
	for(int i = 0 ; i < rows ; i++) {
		int rowOff = i*cols;
		for(int j = 1 ; j < cols ; j++) {
			double val = vals[rowOff+j]-means[j-1];
			std[j-1]=val*val;
		}
	}
	
	double div = 1.0/rows; 
	for(int i = 0 ; i < means.size() ; i++) {
		std[i]=sqrt(std[i]*div);
	}
	
}

void normalize(vector<double> &vals, int rows, int cols) {
	
	vector<double> mean;
	vector<double> stdd;
	calcMean(vals, rows, cols, mean);
	calcStd(vals, rows, cols, mean, stdd);
	
	for(int i = 0 ; i < rows ; i++) {
		int rowOff = i*cols;
		for(int j = 1 ; j < cols ; j++) {
			double &v = vals[rowOff+j];
			v = (v - mean[j-1]) / stdd[j-1];
		}
	}
	
}

void writeMat(const char * fnm, 
				const double *mat, 
				size_t sz
				) {
	
	cout << "Write to file " << fnm << "->" << sz << " bytes" << endl;
	
	int fd, flags;
	
	flags = O_WRONLY | O_CREAT;
#ifdef __MINGW32__
    flags |= _O_BINARY;
#endif // __MINGW32__
	
	fd=open(fnm, flags, 0755);
	
	if(fd == -1) {
		perror("Write:Open.");
		return;
	}
	
	if(write(fd, mat, sz) != sz) {
		perror("Write:Write.");
		return;
	}
	
	if(close(fd)==-1) {
		perror("Write:Close.");
		return;
	}
}