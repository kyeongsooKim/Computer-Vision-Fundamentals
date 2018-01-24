#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void rotate90n(const Mat& src, Mat& dst, int n)
{
	CV_Assert(n >= 0 && n <= 3);

	switch (n) {
	case 0:
		dst = src.clone();
		break;
	case 1: // 90 degree
		flip(src.t(), dst, 1);
		break;
	case 2: // 180 degree
		flip(src, dst, -1);
		break;
	case 3: // 270 degree
		flip(src.t(), dst, 0);
		break;
	default:
		break;
	}
}

int main()
{
	Mat src = imread("jungdongjin.jpg");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow("src");
	imshow("src", src);

	namedWindow("dst");
	for (int n = 0; n <= 3; n++) {
		Mat dst;
		rotate90n(src, dst, n);

		cout << (n * 90) << " degree rotated!" << endl;
		imshow("dst", dst);
		waitKey(0);
	}

	return 0;
}