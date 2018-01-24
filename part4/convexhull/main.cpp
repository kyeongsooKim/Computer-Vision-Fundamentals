#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("cards.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_bin;
	threshold(src, src_bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	vector<vector<Point> > contours;
	findContours(src_bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (unsigned i = 0; i < contours.size(); i++) {
		vector<Point> hull;
		convexHull(contours[i], hull);

		polylines(dst, hull, true, Scalar(0, 255, 255), 2);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
