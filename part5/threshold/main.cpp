#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int t_value = 128;
void on_trackbar_threshold(int, void*);
Mat src, dst;

int main(int argc, char* argv[])
{
	if (argc == 1) {
		src = imread("lenna.bmp", IMREAD_GRAYSCALE);
	} else if (argc == 2) {
		src = imread(argv[1], IMREAD_GRAYSCALE);
	}

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow("src");
	imshow("src", src);

	namedWindow("dst");
	createTrackbar("Threshold", "dst", &t_value, 255, on_trackbar_threshold);
	on_trackbar_threshold(0, 0); // Call the function to initialize

	waitKey(0);
	return 0;
}

void myThreshold(const Mat& src, Mat& dst, int th)
{
	CV_Assert(!src.empty());

	if (dst.empty() ||
		(src.rows != dst.rows || src.cols != dst.cols || dst.type() != CV_8U))
		dst = Mat::zeros(src.rows, src.cols, CV_8U);

	for (int j = 0; j < src.rows; j++) {
		for (int i = 0; i < src.cols; i++) {
			if (src.at<uchar>(j, i) <= th)
				dst.at<uchar>(j, i) = 0;
			else
				dst.at<uchar>(j, i) = 255;
		}
	}
}

void on_trackbar_threshold(int, void*)
{
#if 0
	threshold(src, dst, t_value, 255, THRESH_BINARY);
#else
	myThreshold(src, dst, t_value);
#endif

	imshow("dst", dst);
}
