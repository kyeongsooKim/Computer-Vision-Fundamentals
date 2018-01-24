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

	// Unsharp mask filtering by Laplacian
	float data[] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
	Mat kernel(3, 3, CV_32F, data);

	Mat dst(src.rows, src.cols, src.type());
	filter2D(src, dst, -1, kernel);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
