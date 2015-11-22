#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <inttypes.h>

#include "Pixes.h"
#include "AffineTransform.h"

using namespace std;
using namespace pix;

int findStart(const Matrix &img, int startCol) {
    for(int x = startCol ; x < img.nCols ; x++) {
        for(int y = 0 ; y < img.nRows ; y++) {
            if(fabs(*img(y,x)-255.0f) > 1e-4f) {
                return x;
            }
        }
    }
    return -1;
}

int findEnd(const Matrix &img, int startCol) {
    int y;
    for(int x = startCol ; x < img.nCols ; x++) {
        for(y = 0 ; y < img.nRows ; y++) {
            if(*img(y,x) < 1e-4) {
                break;
            }
        }
        if(y == img.nRows) return x;
    }
    return -1;
}

vector<unsigned long long> findSplits(const Matrix &img) {

    unsigned long long s, e;
    vector<unsigned long long> splitList;

    e = 0;
    s = findStart(img, e);
    while(s != -1) {
        e = findEnd(img, s+1);
        if(e == -1) {
            return vector<unsigned long long>();
        }
        unsigned long long curr = (s-1)<<32 | (e+1);
        splitList.push_back(curr);
        s = findStart(img, e+1);
    }

    return splitList;
}

long long MaxMin(const Matrix &img, int scol, int ecol) {

    long long min = -1, max = -1;

    for(int y = 0 ; y < img.nRows ; y++) {
        for(int x = scol ; x < ecol ; x++) {
            if(*img(y, x) < 1e-4) {
                min = (y-1);
                goto endMin;
            }
        }
    }
    endMin:

    for(int y = 0 ; y < img.nRows ; y++) {
        for(int x = scol ; x < ecol ; x++) {
            if(*img(img.nRows-1-y, x) < 1e-4) {
                max = (img.nRows-y+1);
                goto endMax;
            }
        }
    }
    endMax:

    return max << 32 | min;
}

vector<unsigned long long> verticalSplits(const Matrix &img,
            const vector<unsigned long long> &splits) {

    vector<unsigned long long> vert;
    for(int i = 0 ; i < splits.size() ; i++) {
        int s = splits[i] >> 32;
        int e = splits[i];
        long long curr = MaxMin(img, s, e);
        int t = (curr >> 32);
        int b = curr;
        if(t == -1 || b == -1)
            return vector<unsigned long long>();
        vert.push_back(curr);
    }
    return vert;
}

void subImage(const Matrix &src, Matrix &dst,
              int sx, int sy,
              int width, int height) {

    if(!(dst.nCols==width && dst.nRows==height)) {
        dst = Matrix(height, width);
    }

    for(int x = 0 ; x < width ; x++) {
        for(int y = 0 ; y < height ; y++) {
            *dst(y, x) = *src(y+sy, x+sx);
        }
    }
}

bool splitToSegments(const Matrix &img, std::vector<Matrix> &segments) {

    Matrix dst;
    threshold(img, dst, ThresholdParams(OTSU));

    vector<unsigned long long> hsplit = findSplits(dst);
    vector<unsigned long long> vsplit = verticalSplits(img, hsplit);

    if(!(hsplit.size() == 4 && vsplit.size() == 4)) {
        return false;
    }

    Matrix aft(3, 3, 0.0f);

    for(int i = 0 ; i < 4 ; i++) {
        int l = hsplit[i] >> 32;
        int r = hsplit[i];
        int t = vsplit[i];
        int b = vsplit[i] >> 32;
        int width = r-l;
        int height = b-t;
        float scaleX = width / 20.0f;
        float scaleY = height / 20.0f;

        Matrix sub(height, width);
        subImage(dst, sub, l, t, width, height);
        createScaleMatrix(scaleX, scaleY, aft);

        affineTransform(sub, segments[i], aft, NEAREST_NEIGHBOUR);
    }

    return true;
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


int main(int argc, char * argv[])
{
    if(argc < 3) {
        cerr  << argv[0] << " file[, file ...] folder" << endl;
        return 1;
    }

    vector<Matrix> seg;
    seg.push_back(Matrix(20,20));
    seg.push_back(Matrix(20,20));
    seg.push_back(Matrix(20,20));
    seg.push_back(Matrix(20,20));

    string savefolder = argv[argc-1];

    for(int i = 1 ; i < argc - 1 ; i++) {
        Matrix img;
        loadFromFile(argv[i], img);
        if(!splitToSegments(img, seg)) {
            cerr << "Split failed for: " << argv[i] << endl;
            continue;
        }

        FileName fileNm = fileName(argv[i]);
        for(int i = 0 ; i < seg.size() ; i++) {
            string currFileNm = savefolder + "/" +fileNm.name + "_" +
                string(((i == 0) ? "1" : (i==1) ? "2" : (i==2) ? "3" : "4"))
                +".png";
            saveToFile(seg[i], seg[i], seg[i], currFileNm.c_str(), "png");
            cout << "\tsave file: " <<  currFileNm << endl;
        }

    }
}
