/**************************************************************************

file name	: rgb2gray
date		: 2017. 8. 19
description	: implement color inversion function from rgb to grey color

*****************************************************************************/

#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define RGB2GRAY(r, g, b) ((4899*(r) + 9617*(g) + 1868*(b)) >> 14)
#define BGR2GRAY(b, g, r) ((4899*(r) + 9617*(g) + 1868*(b)) >> 14)

int main()
{
	Mat src = imread("ksbmp.bmp", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst(src.rows, src.cols, CV_8U);

#if 1
	cvtColor(src, dst, COLOR_BGR2GRAY); //using openCV function
#else
	for (int j = 0; j < src.rows; j++) {
		for (int i = 0; i < src.cols; i++) {
			uchar b = src.at<Vec3b>(j, i)[0];
			uchar g = src.at<Vec3b>(j, i)[1];
			uchar r = src.at<Vec3b>(j, i)[2];

			// uchar y = (uchar)(0.299*r + 0.587*g + 0.114*b + 0.5);
			// dst.at<uchar>(j, i) = y;
			dst.at<uchar>(j, i) = RGB2GRAY(r, g, b);
		}
	}
#endif

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}