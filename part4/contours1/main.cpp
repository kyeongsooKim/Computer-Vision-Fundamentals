#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("contours.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src.clone(), contours, hierarchy,
		RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, idx, color, -1, LINE_8, hierarchy);
	}

	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
