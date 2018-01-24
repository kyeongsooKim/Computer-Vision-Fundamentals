#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void drawRect(Mat& img, vector<Point>& contours);
void setSrcQuad(vector<Point>& approx, Point2f srcQuad[4]);
void setSrcQuad2(vector<Point> corners, Point2f srcQuad[4]);

int main()
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	int frame_width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
	cout << "width: " << frame_width << ", height: " << frame_height << endl;

	namedWindow("camera");
	namedWindow("namecard");

	int w = 450, h = 250;
	Point2f srcQuad[4], dstQuad[4];
	dstQuad[0] = Point2f(0, 0);
	dstQuad[1] = Point2f(0, h);
	dstQuad[2] = Point2f(w, h);
	dstQuad[3] = Point2f(w, 0);

	Mat src, src_gray, src_bin;
	Mat namecard(Size(w, h), CV_8UC3, Scalar(255, 255, 255));

	for (;;) {
		cap >> src;
		if (src.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		cvtColor(src, src_gray, COLOR_BGR2GRAY);
		threshold(src_gray, src_bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

		vector<vector<Point> > contours;
		findContours(src_bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

		for (unsigned i = 0; i < contours.size(); i++) {
			vector<Point> approx;
			approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);

			if (fabs(contourArea(contours[i])) < 1000 || !isContourConvex(approx))
				continue;

			if (approx.size() == 4) {
				drawRect(src, approx);
				setSrcQuad2(approx, srcQuad);

				Mat pers = getPerspectiveTransform(srcQuad, dstQuad);
				warpPerspective(src, namecard, pers, Size(w, h));
			}
		}

		imshow("camera", src);
		imshow("namecard", namecard);

		if (waitKey(10) >= 0)
			break;
	}

	return 0;
}

void drawRect(Mat& img, vector<Point>& contours)
{
	CV_Assert(contours.size() == 4);

	for (int i = 1; i < 4; i++) {
		line(img, Point(contours[i - 1].x, contours[i - 1].y), 
			Point(contours[i].x, contours[i].y), Scalar(0, 255, 0), 2, LINE_AA);
	}

	line(img, Point(contours[3].x, contours[3].y),
		Point(contours[0].x, contours[0].y), Scalar(0, 255, 0), 2, LINE_AA);
}

void setSrcQuad(vector<Point>& approx, Point2f srcQuad[4])
{
	CV_Assert(approx.size() == 4);

	vector<pair<int, int> > v;
	v.push_back(pair<int, int>(approx[0].x, approx[0].y));
	v.push_back(pair<int, int>(approx[1].x, approx[1].y));
	v.push_back(pair<int, int>(approx[2].x, approx[2].y));
	v.push_back(pair<int, int>(approx[3].x, approx[3].y));

	sort(v.begin(), v.end());

	if (v[0].second < v[1].second) {
		srcQuad[0] = Point2f(v[0].first, v[0].second);
		srcQuad[1] = Point2f(v[1].first, v[1].second);
	} else {
		srcQuad[1] = Point2f(v[0].first, v[0].second);
		srcQuad[0] = Point2f(v[1].first, v[1].second);
	}

	if (v[2].second < v[3].second) {
		srcQuad[3] = Point2f(v[2].first, v[2].second);
		srcQuad[2] = Point2f(v[3].first, v[3].second);
	} else {
		srcQuad[2] = Point2f(v[2].first, v[2].second);
		srcQuad[3] = Point2f(v[3].first, v[3].second);
	}
}

void setSrcQuad2(vector<Point> corners, Point2f srcQuad[4])
{
	CV_Assert(corners.size() == 4);

	sort(corners.begin(), corners.end(), [](Point u, Point v) {
		return u.x < v.x;
	});

	if (corners[0].y < corners[1].y) {
		srcQuad[0] = Point2f(corners[0].x, corners[0].y);
		srcQuad[1] = Point2f(corners[1].x, corners[1].y);
	}
	else {
		srcQuad[1] = Point2f(corners[0].x, corners[0].y);
		srcQuad[0] = Point2f(corners[1].x, corners[1].y);
	}

	if (corners[2].y < corners[3].y) {
		srcQuad[3] = Point2f(corners[2].x, corners[2].y);
		srcQuad[2] = Point2f(corners[3].x, corners[3].y);
	}
	else {
		srcQuad[2] = Point2f(corners[2].x, corners[2].y);
		srcQuad[3] = Point2f(corners[3].x, corners[3].y);
	}
}

