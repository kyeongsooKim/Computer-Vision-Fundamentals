#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	cv::ocl::setUseOpenCL(false);

	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst = src.clone();

	double start = double(getTickCount());

#if 1
	blur(src, dst, Size(3, 3));
#else
	for (int j = 1; j < src.rows - 1; j++) {
		for (int i = 1; i < src.cols - 1; i++) {
			int sum = src.at<uchar>(j - 1, i - 1) + src.at<uchar>(j - 1, i) + src.at<uchar>(j - 1, i + 1) +
				src.at<uchar>(j, i - 1) + src.at<uchar>(j, i) + src.at<uchar>(j, i + 1) +
				src.at<uchar>(j + 1, i - 1) + src.at<uchar>(j + 1, i) + src.at<uchar>(j + 1, i + 1);
			dst.at<uchar>(j, i) = cvRound(sum / 9.f);
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