#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("rice.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst;
	double th = threshold(src, dst, 0, 255, THRESH_BINARY | THRESH_OTSU);

	cout << "Otsu threshold value is " << th << "." << endl;

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

