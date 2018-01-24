#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst(src.rows, src.cols * 1.5, src.type(), Scalar(0));

#if 0
	float data[] = { 1.f, 0.5f, 0, 0, 1.f, 0 };
	Mat sm(2, 3, CV_32F, data);
	warpAffine(src, dst, sm, Size(src.cols * 3 / 2, src.rows));
#else
	int nx, ny;
	double m = 0.5;
	for (int j = 0; j < src.rows; j++) {
		for (int i = 0; i < src.cols; i++) {
			nx = i + m * j;
			ny = j;
			dst.at<uchar>(ny, nx) = src.at<uchar>(j, i);
		}
	}
#endif

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}