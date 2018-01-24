#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("candies.png", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_hsv;
	cvtColor(src, src_hsv, COLOR_BGR2HSV);

	vector<Mat> hsv_planes;
	split(src_hsv, hsv_planes);

	imshow("src", src);
	imshow("H plane", hsv_planes[0]);
	imshow("S plane", hsv_planes[1]);
	imshow("V plane", hsv_planes[2]);

	waitKey(0);
	return 0;
}