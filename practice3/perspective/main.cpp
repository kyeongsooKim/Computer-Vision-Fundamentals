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

	int w = src.cols;
	int h = src.rows;

	Point2f srcQuad[4], dstQuad[4];
	srcQuad[0] = Point2f(0, 0);
	srcQuad[1] = Point2f(w - 1, 0);
	srcQuad[2] = Point2f(0, h - 1);
	srcQuad[3] = Point2f(w - 1, h - 1);

	dstQuad[0] = Point2f(150, 50);
	dstQuad[1] = Point2f(350, 150);
	dstQuad[2] = Point2f(50, 450);
	dstQuad[3] = Point2f(450, 450);

	Mat per_mat = getPerspectiveTransform(srcQuad, dstQuad);

	Mat dst;
	warpPerspective(src, dst, per_mat, Size(w, h));

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}