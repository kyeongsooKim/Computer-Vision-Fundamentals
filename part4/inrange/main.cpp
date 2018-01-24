#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int pos_hue1 = 10, pos_hue2 = 30, pos_sat1 = 150, pos_sat2 = 255;
Mat src, src_hsv, dst, dst_mask;

void on_hsv_changed(int, void*)
{
	Scalar lowerb(pos_hue1, pos_sat1, 0);
	Scalar upperb(pos_hue2, pos_sat2, 255);
	inRange(src_hsv, lowerb, upperb, dst_mask);

	cvtColor(src, dst, COLOR_BGR2GRAY);
	cvtColor(dst, dst, COLOR_GRAY2BGR);
	src.copyTo(dst, dst_mask);
//	imshow("dst_mask", dst_mask);
	imshow("dst", dst);
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		src = imread("flower1.jpg", IMREAD_COLOR);
	} else {
		src = imread(argv[1], IMREAD_COLOR);
	}

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	cvtColor(src, src_hsv, COLOR_BGR2HSV);

	imshow("src", src);

	namedWindow("dst");
	createTrackbar("Lower Hue", "dst", &pos_hue1, 180, on_hsv_changed);
	createTrackbar("Upper Hue", "dst", &pos_hue2, 180, on_hsv_changed);
	createTrackbar("Lower Sat", "dst", &pos_sat1, 255, on_hsv_changed);
	createTrackbar("Upper Sat", "dst", &pos_sat2, 255, on_hsv_changed);
	on_hsv_changed(0, 0);

	waitKey(0);
	return 0;
}
