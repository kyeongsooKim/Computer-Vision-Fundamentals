#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("coins.png", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	imshow("src", src);

	Mat dst = src.clone();
	cvtColor(dst, dst, COLOR_GRAY2BGR);

	vector<Vec3f> circles;
	blur(src, src, Size(3, 3));
	HoughCircles(src, circles, HOUGH_GRADIENT, 1, 10, 150, 30, 10, 50);

	for (size_t i = 0; i < circles.size(); i++) {
		Vec3i c = circles[i];
		circle(dst, Point(c[0], c[1]), c[2], Scalar(0, 0, 255), 2, LINE_AA);
	}

	imshow("dst", dst);

	waitKey(0);
	return 0;
}