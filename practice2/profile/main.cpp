#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void on_trackbar(int, void*);

Mat src, dst, profile;
int row = 0;

int main(void)
{
	src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	
	namedWindow("dst");
	namedWindow("profile");

	profile.create(256, src.cols, CV_8U);

	createTrackbar("Profile", "dst", &row, src.rows - 1, on_trackbar, NULL);
	on_trackbar(0, 0);

	waitKey(0);
	return 0;
}

void on_trackbar(int, void*)
{
	src.copyTo(dst);
	profile.setTo(255);

	uchar value;
	uchar* pSrc = (uchar*)src.ptr<uchar>(row);
	uchar* pDst = (uchar*)dst.ptr<uchar>(row);
	for (int i = 1; i < src.cols; i++) {
		line(profile, Point(i - 1, 255 - pSrc[i - 1]), Point(i, 255 - pSrc[i]), 0);
		pDst[i] = saturate_cast<uchar>(pSrc[i] + 50);
	}

	imshow("dst", dst);
	imshow("profile", profile);
}
