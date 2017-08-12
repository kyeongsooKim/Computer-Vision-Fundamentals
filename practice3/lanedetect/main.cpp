#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("lane01.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_edge;
	Canny(src, src_edge, 50, 150);
	
	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(src_edge, lines, 1, CV_PI / 180, 50, 20, 100);

	for (Vec4i line : lines) {
		int dx = line[2] - line[0];
		int dy = line[3] - line[1];
		float angle = atan2f(dy, dx) * 180 / CV_PI;

		if (fabs(angle) <= 10)
			continue;

		if (angle > 0)
			cv::line(dst, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(0, 0, 255), 1);
		else
			cv::line(dst, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(0, 255, 0), 1);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
