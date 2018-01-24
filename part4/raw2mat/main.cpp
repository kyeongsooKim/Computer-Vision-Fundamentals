/**************************************************************************

file name	: raw2mat
date		: 2017. 8. 19
description	: create Mat from memory buffer

*****************************************************************************/


#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	// open Raw file
	FILE* fp = fopen("elaine.raw", "rb");
	if (fp == NULL) {
		cout << "File load error!" << endl;
		return -1;
	}

	// dynamic allocation & read file
	int width = 512;
	int height = 512;
	int area = width * height;
	int step = 512;

	uchar* pixels = new uchar[area];
	int ret = (int)fread(pixels, sizeof(uchar), area, fp);
	if (ret != area) {
		cout << "fread failed!" << endl;
		delete [] pixels;
		return -1;
	}

	fclose(fp);

	// create Mat object which refer to memory buffer already created.
	Mat src(height, width, CV_8UC1, pixels, step);

	// OpenCV Operations
	Mat dst;
	bilateralFilter(src, dst, -1, 20, 5);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);

	delete [] pixels;

	return 0;
}
