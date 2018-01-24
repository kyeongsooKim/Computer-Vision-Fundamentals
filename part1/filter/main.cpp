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
	
	Mat dst = src.clone();
	//Mat dst(src.rows, src.cols, src.type(), Scalar(0));

	for (int j = 1; j < src.rows - 2; j++) {
		for (int i = 1; i < src.cols - 2; i++) {
			int val = src.at<uchar>(j - 1, i - 1) +
				src.at<uchar>(j - 1, i) +
				src.at<uchar>(j - 1, i + 1) +
				src.at<uchar>(j, i - 1) +
				src.at<uchar>(j, i) +
				src.at<uchar>(j, i + 1) +
				src.at<uchar>(j + 1, i - 1) +
				src.at<uchar>(j + 1, i) +
				src.at<uchar>(j + 1, i + 1);

			dst.at<uchar>(j, i) = cvRound(val / 9.);

				
		}
	}

	// line 40 do the same thing as line 21 ~ 33 do.
	//blur(src, dst, Size(3, 3));

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
