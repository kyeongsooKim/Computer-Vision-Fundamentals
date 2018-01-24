#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst;
	Laplacian(src, dst, -1, 1, 1.0, 128);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
