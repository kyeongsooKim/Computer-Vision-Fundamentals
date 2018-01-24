#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("stop.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst1, dst2, dst3;
	resize(src, dst1, Size(), 4, 4, INTER_NEAREST);
	resize(src, dst2, Size(1024, 1024));
	resize(src, dst3, Size(1024, 1024), 0, 0, INTER_CUBIC);

	imshow("src", src);
	imshow("nearest", dst1(Rect(340, 640, 256, 256)));
	imshow("bilinear", dst2(Rect(340, 640, 256, 256)));
	imshow("bicubic", dst3(Rect(340, 640, 256, 256)));

	waitKey(0);
	return 0;
}