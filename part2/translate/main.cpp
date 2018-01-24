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

	Mat dst(src.rows, src.cols, src.type(), Scalar(0));

#if 1
	for (int j = 0; j < src.rows; j++) {
		for (int i = 0; i < src.cols; i++) {
			int x = i + 100;
			int y = j + 100;
			if (x >= dst.cols) continue;
			if (y >= dst.rows) continue;
			dst.at<uchar>(y, x) = src.at<uchar>(j, i);
		}
	}
#else
	src(Rect(0, 0, 412, 412)).copyTo(dst(Rect(100, 100, 412, 412)));
#endif

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}