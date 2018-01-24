#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat src;

	if (argc < 2) {
		src = imread("lenna.bmp", IMREAD_GRAYSCALE);
	} else {
		src = imread(argv[1], IMREAD_GRAYSCALE);
	}

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst;
	Mat dst2;
#if 1
	dst = src * 2;
	dst2 = src + (src - 128) * 1.5; //improved approach


#else
	dst = Mat::zeros(src.rows, src.cols, src.type());

	for (int j = 0; j < src.rows; j++) {
		uchar* pSrc = src.ptr<uchar>(j);
		uchar* pDst = dst.ptr<uchar>(j);
		for (int i = 0; i < src.cols; i++) {
			pDst[i] = saturate_cast<uchar>(pSrc[i] + (pSrc[i] - 128) * 1.5);
		}
	}
#endif

	imshow("src", src);
	imshow("dst", dst);
	imshow("dst2", dst2);

	waitKey(0);
	return 0;
}
