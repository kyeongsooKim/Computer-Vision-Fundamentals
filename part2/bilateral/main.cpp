#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	cv::ocl::setUseOpenCL(false);

	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst;

	double start = double(getTickCount());
	
	bilateralFilter(src, dst, -1, 10, 5);
	
	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}