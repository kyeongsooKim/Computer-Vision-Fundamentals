#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

double getAngle(Point p1, Point p2, Point p3);

int main(int argc, char* argv[])
{
	Mat src;

	if (argc < 2) {
		src = imread("hand01.jpg", IMREAD_COLOR);
	} else {
		src = imread(argv[1], IMREAD_COLOR);
	}

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_hsv;
	cvtColor(src, src_hsv, COLOR_BGR2HSV);

	Mat skin;
	Scalar lowerb(0, 100, 0);
	Scalar upperb(30, 255, 255);
	inRange(src_hsv, lowerb, upperb, skin);

	erode(skin, skin, Mat());
	dilate(skin, skin, Mat());

	imshow("src", src);
	imshow("skin area", skin);

	vector<vector<Point> > contours;
	findContours(skin, contours, noArray(), RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	if (contours.size() < 1) {
		cout << "Hand is not found!" << endl;
		return 0;
	}

	int max_idx = 0;
	double max_area = contourArea(contours[0]);
	for (unsigned i = 1; i < contours.size(); i++) {
		if (contourArea(contours[i]) > max_area) {
			max_area = contourArea(contours[i]);
			max_idx = i;
		}
	}

	Mat dst = src.clone();
	vector<Point> hand_contour = contours[max_idx];
	vector<int> hull;
	vector<Vec4i> defects;

	convexHull(hand_contour, hull);
	convexityDefects(hand_contour, hull, defects);

#if 0
	vector<Point> hull_pts;
	for (unsigned i = 0; i < hull.size(); i++) {
		hull_pts.push_back(hand_contour[hull[i]]);
		//circle(dst, hand_contour[hull[i]], 5, Scalar(0, 255, 255), 1);
	}
	drawContours(dst, vector<vector<Point>>(1, hull_pts), 0, Scalar(255, 0, 0), 2);
#endif

	for (Vec4i v : defects) {
		Point ptStart = hand_contour[v[0]];
		Point ptEnd = hand_contour[v[1]];
		Point ptFar = hand_contour[v[2]];
		float depth = v[3] / 256.f;

		if (getAngle(ptStart, ptFar, ptEnd) > 90) continue;

		if (depth > 50) {
			line(dst, ptStart, ptFar, Scalar(0, 255, 0), 2);
			line(dst, ptEnd, ptFar, Scalar(0, 255, 0), 2);
			circle(dst, ptStart, 5, Scalar(0, 0, 255), 2);
			circle(dst, ptEnd, 5, Scalar(0, 0, 255), 2);
			circle(dst, ptFar, 5, Scalar(255, 0, 255), 2);
		}
	}

	imshow("dst", dst);

	waitKey(0);
	return 0;
}

double getAngle(Point p1, Point p2, Point p3)
{
	Point a = p1 - p2;
	Point b = p3 - p2;

	double up = a.x * b.x + a.y * b.y;
	double dn = sqrt(a.x * a.x + a.y * a.y) * sqrt(b.x * b.x + b.y * b.y);

	return (acos(up / dn) * 180 / CV_PI);
}
