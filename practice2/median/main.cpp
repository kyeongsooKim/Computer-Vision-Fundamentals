#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	cv::ocl::setUseOpenCL(false);

	Mat src = imread("noise.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst(src.rows, src.cols, src.type());

	double start = double(getTickCount());

#if 1
	medianBlur(src, dst, 3);
#else
	uchar m[9];
	for (int j = 1; j < src.rows - 1; j++) {
		for (int i = 1; i < src.cols - 1; i++) {
			m[0] = src.at<uchar>(j - 1, i - 1);
			m[1] = src.at<uchar>(j - 1, i);
			m[2] = src.at<uchar>(j - 1, i + 1);
			m[3] = src.at<uchar>(j, i - 1);
			m[4] = src.at<uchar>(j, i);
			m[5] = src.at<uchar>(j, i + 1);
			m[6] = src.at<uchar>(j + 1, i - 1);
			m[7] = src.at<uchar>(j + 1, i);
			m[8] = src.at<uchar>(j + 1, i + 1);

			std::sort(m, m + 9);

			dst.at<uchar>(j, i) = m[4];
		}
	}
#endif

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}