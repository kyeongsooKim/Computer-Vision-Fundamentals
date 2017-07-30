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

	int hist[256] = { 0, }; // create histogram

	for (int j = 0; j < src.rows; j++) { // fill it up
		for (int i = 0; i < src.cols; i++) {
			hist[src.at<uchar>(j, i)]++;
		}
	} 

	int histMax = 0;
	for (int i = 0; i < 256; i++) {
		if (hist[i] > histMax) histMax = hist[i];
	}

	Mat imgHist(100, 256, CV_8U, Scalar(255));
	for (int i = 0; i < 256; i++) {
		line(imgHist, Point(i, 100),
			Point(i, 100 - cvRound(hist[i] * 100 / histMax)), Scalar(0));
		//cvRound() : built-in round off function in openCV
	}

	imshow("src", src);
	imshow("histogram", imgHist);

	waitKey(0);
	return 0;
}
