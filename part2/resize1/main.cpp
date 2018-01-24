#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("camera.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst(src.rows * 2, src.cols * 2, src.type(), Scalar(0));

	for (int j = 0; j < src.rows; j++) {
		for (int i = 0; i < src.cols; i++) {
			dst.at<uchar>(j * 2, i * 2) = src.at<uchar>(j, i);
		}
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}