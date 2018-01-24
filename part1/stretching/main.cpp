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

	double gmin, gmax;
	minMaxLoc(src, &gmin, &gmax);

	Mat dst(src.rows, src.cols, src.type());

	for (int j = 0; j < src.rows; j++) {
		uchar* pSrc = src.ptr<uchar>(j);
		uchar* pDst = dst.ptr<uchar>(j);
		for (int i = 0; i < src.cols; i++) {
			pDst[i] = saturate_cast<uchar>((pSrc[i] - gmin) * 255 / (gmax - gmin));
		}
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}