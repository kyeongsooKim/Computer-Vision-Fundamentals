#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void calcLBP(const Mat& src, Mat& dst)
{
	if (src.empty())
		return;

	dst = Mat::zeros(src.rows - 2, src.cols - 2, CV_8U);

	for (int j = 1; j < src.rows - 1; j++) {
		for (int i = 1; i < src.cols - 1; i++) {
			uchar value = 0;
			uchar c = src.at<uchar>(j, i);

			value |= (src.at<uchar>(j - 1, i - 1) >= c) << 7;
			value |= (src.at<uchar>(j - 1, i) >= c) << 6;
			value |= (src.at<uchar>(j - 1, i + 1) >= c) << 5;
			value |= (src.at<uchar>(j, i + 1) >= c) << 4;
			value |= (src.at<uchar>(j + 1, i + 1) >= c) << 3;
			value |= (src.at<uchar>(j + 1, i) >= c) << 2;
			value |= (src.at<uchar>(j + 1, i - 1) >= c) << 1;
			value |= (src.at<uchar>(j, i - 1) >= c);

			dst.at<uchar>(j - 1, i - 1) = value;
		}
	}
}

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

int main(int argc, char* argv[])
{
	Mat src1; 
	
	if (argc < 2 )
		src1 = imread("pebbles-1.jpg", IMREAD_COLOR);
	else
		src1 = imread(argv[1], IMREAD_COLOR);

	if (src1.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat gray1, lbp1, hist1;
	cvtColor(src1, gray1, COLOR_BGR2GRAY);
	calcLBP(gray1, lbp1);
	hist1 = calcGrayHist(lbp1);

	imshow("src1", src1);
//	imshow("lbp1", lbp1);
	imshow("hist1", histImage(hist1));

	vector<String> files = { "board-1.jpg", "board-2.jpg", "board-3.jpg", "board-4.jpg",
		"leaf-1.jpg", "leaf-2.jpg",
		"pebbles-1.jpg", "pebbles-2.jpg", "pebbles-3.jpg", "pebbles-4.jpg" };

	for (String filename : files) {
		//cout << "Loading " << filename << "..." << endl;

		Mat src2 = imread(filename, IMREAD_COLOR);
		if (src2.empty()) {
			cerr << "Image load failed!" << endl;
			return -1;
		}

		Mat gray2, lbp2, hist2;
		cvtColor(src2, gray2, COLOR_BGR2GRAY);
		calcLBP(gray2, lbp2);
		hist2 = calcGrayHist(lbp2);

		double dist = compareHist(hist1, hist2, HISTCMP_CHISQR_ALT);
		cout << "Histogram distance with " << filename << ": " << dist << endl;

		imshow("src2", src2);
		imshow("hist2", histImage(hist2));

		waitKey(0);
	}

	return 0;
}

