#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/ximgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("LinuxLogo.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	
	Mat src_bin;
	threshold(src, src_bin, 128, 255, THRESH_BINARY);

	Mat dst;
	ximgproc::thinning(src, dst);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
