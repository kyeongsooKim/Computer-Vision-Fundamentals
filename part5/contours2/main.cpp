#pragma warning(disable: 4819)

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("milkdrop.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_bin;
	threshold(src, src_bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	vector<vector<Point> > contours;
	findContours(src_bin, contours, RETR_LIST, CHAIN_APPROX_NONE);

	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
	for (unsigned i = 0; i < contours.size(); i++) {
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, i, color, 1, LINE_8);
	}

	namedWindow("src");
	imshow("src", src);

	namedWindow("dst");
	imshow("dst", dst);

	waitKey(0);
	return 0;
}