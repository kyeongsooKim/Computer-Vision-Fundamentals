#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat calcGrayHist(const Mat& img)
{
	CV_Assert(img.type() == CV_8U);

	Mat hist;
	int channels[] = { 0 };
	int dims = 1;
	const int histSize[] = { 256 };
	float graylevel[] = { 0, 256 };
	const float* ranges[] = { graylevel };

	calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges, true);

	return hist;
}

Mat histImage(const Mat& hist)
{
	CV_Assert(!hist.empty());
	CV_Assert(hist.type() == CV_32F);

	double histMax = 0.;
	minMaxLoc(hist, 0, &histMax);

	Mat imgHist(100, 256, CV_8U, Scalar(255));
	for (int i = 0; i < 256; i++) {
		line(imgHist, Point(i, 100),
			Point(i, 100 - cvRound(hist.at<float>(i) * 100 / histMax)), Scalar(0));
	}

	return imgHist;
}

int main(void)
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat hist = calcGrayHist(src);
	Mat imgHist = histImage(hist);

	imshow("src", src);
	imshow("histogram", imgHist);

	waitKey(0);
	return 0;
}