#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("mandrill.bmp", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst(src.rows, src.cols, src.type());

#if 0
	dst = Scalar(255, 255, 255) - src;
#else
	for (int j = 0; j < src.rows; j++) {
		Vec3b* pSrc = src.ptr<Vec3b>(j);
		Vec3b* pDst = dst.ptr<Vec3b>(j);
		for (int i = 0; i < src.cols; i++) {
			pDst[i][0] = 255 - pSrc[i][0]; // Blue
			pDst[i][1] = 255 - pSrc[i][1]; // Green
			pDst[i][2] = 255 - pSrc[i][2]; // Red
		}
	}
#endif

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}