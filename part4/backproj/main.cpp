#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src, src_ycrcb, src_mask;
	src = imread("sistar1.jpg", IMREAD_COLOR);
	src_mask = imread("sistar1_mask.bmp", IMREAD_GRAYSCALE);

//	GaussianBlur(src, src, Size(), 0.5, 0.5);
	cvtColor(src, src_ycrcb, COLOR_BGR2YCrCb);

	Mat hist, hist_norm;
	int channels[] = { 1, 2 };

	int cr_bins = 128; int cb_bins = 128;
	int histSize[] = { cr_bins, cb_bins };

	float cr_range[] = { 0, 256 };
	float cb_range[] = { 0, 256 };
	const float* ranges[] = { cr_range, cb_range };

	// Get the CrCb Histogram and normalize it
	calcHist(&src_ycrcb, 1, channels, src_mask, hist, 2, histSize, ranges, true, false);
	normalize(hist, hist_norm, 0, 255, NORM_MINMAX, -1, Mat());

	// Get Backprojection
	Mat backproj;
	calcBackProject(&src_ycrcb, 1, channels, hist, backproj, ranges, 1, true);
//	threshold(backproj, backproj, 2, 255, THRESH_BINARY);

	// Draw the backproj
	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
	src.copyTo(dst, backproj);

	imshow("src", src);
	imshow("src_mask", src_mask);
	imshow("backproj", backproj);
	imshow("dst", dst);
	waitKey(0);

#if 0
	Mat src2, src2_ycrcb;
	src2 = imread("sistar2.jpg", IMREAD_COLOR);
	cvtColor(src2, src2_ycrcb, COLOR_BGR2YCrCb);

	calcBackProject(&src2_ycrcb, 1, channels, hist, backproj, ranges, 1, true);

	// Draw the backproj
	Mat dst2 = Mat::zeros(src2.rows, src2.cols, CV_8UC3);
	src2.copyTo(dst2, backproj);

	imshow("src", src2);
	imshow("src_mask", src_mask);
	imshow("backproj", backproj);
	imshow("dst", dst2);
	waitKey(0);
#endif

	return 0;
}
