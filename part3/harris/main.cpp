#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	cv::ocl::setUseOpenCL(false);

	Mat src = imread("building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int64 t1 = getTickCount();

	Mat harris;
	cornerHarris(src, harris, 2, 3, 0.04);

	int64 t2 = getTickCount();
	cout << "It took " << (t2 - t1) * 1000 / getTickFrequency() << endl;

	Mat harris_norm;
	normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_8U, Mat());

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);
	
	for (int j = 0; j < harris.rows; j++) {
		for (int i = 0; i < harris.cols; i++) {
			if (harris_norm.at<uchar>(j, i) > 180) {
				circle(dst, Point(i, j), 5, Scalar(0, 0, 255), 2);
			}
		}
	}

//	imshow("src", src);
	imshow("harris_norm", harris_norm);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}