#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void setLabel(Mat& img, int idx, vector<Point>& contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	String label = format("idx: %d", idx);

	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(img, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	putText(img, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, LINE_8);
}

int main()
{
	Mat src = imread("digits.bmp", IMREAD_GRAYSCALE);
	Mat two = imread("two.bmp", IMREAD_GRAYSCALE);

	if (src.empty() || two.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	// two.bmp 영상에서 외곽선 정보 추출

	Mat two_bin;
	vector<vector<Point> > contours_two;
	threshold(two, two_bin, 128, 255, THRESH_BINARY);
	findContours(two_bin.clone(), contours_two, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	vector<Point> two_contour = contours_two[0];

	// digits.bmp 영상에서 외곽선 정보 추출

	Mat src_bin;
	vector<vector<Point> > contours;
	threshold(src, src_bin, 128, 255, THRESH_BINARY);
	findContours(src_bin.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// matchShape 실행 & dst 영상 생성

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	double dist;
	for (unsigned idx = 0; idx < contours.size(); idx++) {
		if (contourArea(contours[idx]) < 10)
			continue;

		drawContours(dst, contours, idx, Scalar(0, 255, 255), 2);

		dist = matchShapes(contours[idx], two_contour, CV_CONTOURS_MATCH_I1, 0);
		cout << idx << ": " << dist << endl;

		if (dist < 0.7) {
			drawContours(dst, contours, idx, Scalar(0, 0, 255), 2);
		}

		setLabel(dst, idx, contours[idx]);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}