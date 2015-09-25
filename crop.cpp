#include <cstdio>
#include <cstdlib>
#include <string>

#include <Matrix.h>
#include <ImageIO.h>
#include <PerspectiveTransform.h>

using namespace pix;
using namespace std;

bool cropImage(const Matrix &src, Matrix &cropped,
			   int sx, int sy, int ex, int ey
			   )
{
	int w = ex-sx;
	int h = ey-sy;
	if(w < 0 || h < 0 ||
	   abs(w-cropped.nCols) > 1 || 
	   abs(h-cropped.nRows) > 1
	   )
	{
		return false;
	}
    for(int x = 0 ; x < w ; x++) {
	 	for(int y = 0 ; y < h ; y++) {
	 		*cropped(y, x) = *src(sy+y, sx+x);
	 	}
    }
    return true;
}

int main(int argc, char * argv[]) {
		
	if(argc != 1+1+8+1+1) {
		fprintf(stderr, "arc != %d", 1+1+8+1+1);
	}
	
	Matrix img;
	printf("LoadImage %s\n", argv[1]);
	loadFromFile(argv[1], img);
	
	printf("Unwrap Image using UL(%s, %s) DL(%s, %s) DR(%s, %s) UR(%s, %s) - DS(%s)\n", 
					argv[2], argv[3], argv[4], argv[5], 
					argv[6], argv[7], argv[8], argv[9], argv[10]);
	
	printf("\t create transform\n");
	PerspectiveTransform t(Vec2(atoi(argv[2]), atoi(argv[3])), 
						   Vec2(atoi(argv[4]), atoi(argv[5])), 
						   Vec2(atoi(argv[6]), atoi(argv[7])),
						   Vec2(atoi(argv[8]), atoi(argv[9])),
						   Vec2(0, 0), 
						   Vec2(0, atoi(argv[10])), 
						   Vec2(atoi(argv[10]),atoi(argv[10])), 
						   Vec2(atoi(argv[10]),0));
	
	int size = atoi(argv[10]);
	
	printf("\t transform image\n");
	Matrix unwr(size, size);
	t.unwrapImage(img, unwr);
	
	float incrStep = (float)size/9;
	float fraction = incrStep-((int)incrStep);
	if(fraction != 0.0f) {
		fprintf(stderr, "SIZE/9=%f", incrStep);
		return 1;
	}
	printf("Start slice image 9x9 with SIZE(%f)\n", incrStep);
	
	string folder = argv[11];
	
	if(folder[folder.size()-1] != '/' &&
	   folder[folder.size()-1] != '\\'
	   )
	{
		folder+="/";
	}
	
	string filename = argv[1];
	
	int start;
	
	// c(/) c(\) false -> false, false   V
	// c(/) !c(\) false -> false, true   V
	// !c(/) c(\) false -> true, false V
	// !c(/) !c(\) true -> true, true X
	
	if((start = filename.find_last_of("/")) == string::npos &&
	   (start = filename.find_last_of("\\") == string::npos)
	   )
    {
		start = 0;
    } else start+=1;
	
	int end = filename.find_last_of(".");
	if(end == string::npos) {
		end = filename.size();
	}
	
	filename = filename.substr(start, end-start);
	printf("\tFilename prefix %s\n", filename.c_str());
	printf("\tDestination folder %s\n", folder.c_str());
	Matrix subImage(incrStep, incrStep);
	char buff[500];
	for(int x = 0 ; x < 9 ; x++) {
		for(int y = 0 ; y < 9 ; y++) {
			
			int sx = x*incrStep;
			int sy = y*incrStep;
			int ex = (x+1)*incrStep;
			int ey = (y+1)*incrStep;
			
			if(!cropImage(unwr, subImage, sx, sy, ex, ey)) {
				fprintf(stderr, "Failed to crop slice %d %d %d %d\n", sx, sy, ex ,ey);
			}
			
			sprintf(buff, "_%d_%d.jpeg", x, y);
			string save = folder + filename + string(buff);
			
			printf("\tSave file %s\n", save.c_str());
			saveToFile(subImage, subImage, subImage, save.c_str(), "jpeg");
		}
	}
	
}