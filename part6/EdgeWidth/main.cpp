#pragma warning(disable: 4819)

#include "opencv2\opencv.hpp"
#include "opencv2\core\ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void findEdgeWidth(const Mat& img, vector<Point2f>& left, vector<Point2f>& right);
void drawCross(Mat& img, Point pt, Scalar color = Scalar(0, 0, 255));

int main(void)
{
	ocl::setUseOpenCL(true);

	Mat src = imread("ss01.png", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	vector<Rect> roi;
	roi.push_back(Rect(70, 400, 100, 100));
	roi.push_back(Rect(740, 400, 100, 100));

	Mat dst = src.clone();

	for (Rect rect : roi) {
		rectangle(dst, rect, Scalar(0, 255, 255), 1);

		vector<Point2f> left;
		vector<Point2f> right;
		findEdgeWidth(src_gray(rect), left, right);

		for (int k = 0; k < left.size(); k++) {
			float edge_width = right[k].x - left[k].x;
			cout << "left: " << left[k] << ", right: " << right[k];
			cout << ", edge width: " << edge_width << "px." << endl;

			Point pt1(cvRound(left[k].x), cvRound(left[k].y));
			Point pt2(cvRound(right[k].x), cvRound(right[k].y));
			drawCross(dst, Point(rect.x, rect.y) + pt1);
			drawCross(dst, Point(rect.x, rect.y) + pt2);

			String str = format("%.3f", edge_width);
			putText(dst, str, Point(rect.x + 110, rect.y + left[k].y + 5),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1, LINE_AA);
		}
	}

	imshow("dst", dst);

	waitKey(0);
	return 0;
}

void findEdgeWidth(const Mat& img, vector<Point2f>& left, vector<Point2f>& right)
{
	left.clear();
	right.clear();

	// 가우시안, 1차 미분 등의 연산은 실수형으로 수행함
	Mat img_float, blured, dx;
	img.convertTo(img_float, CV_32F);
	GaussianBlur(img_float, blured, Size(0, 0), 1.);
	Sobel(blured, dx, CV_32F, 1, 0);

	double minv, maxv;
	Point minloc, maxloc;
	float i1, i2, i3, denominator;
	float x_left, x_right;

	// img 영상이 100x100인 경우, y 좌표가 20, 40, 60, 80인 경우에만
	// edge width를 구함
	for (int y = 20; y < 100; y += 20) {
		Mat roi = dx.row(y);
		minMaxLoc(roi, &minv, &maxv, &minloc, &maxloc);

#if 0
		left.push_back(Point2f(maxloc.x, y));
		right.push_back(Point2f(minloc.x, y));
#else
		// left edge
		i1 = roi.at<float>(0, maxloc.x - 1);
		i2 = roi.at<float>(0, maxloc.x);
		i3 = roi.at<float>(0, maxloc.x + 1);
		denominator = 2 * i1 - 4 * i2 + 2 * i3;
		if (denominator != 0.f)
			x_left = maxloc.x + (i1 - i3) / denominator;
		else
			x_left = maxloc.x;
		left.push_back(Point2f(x_left, y));

		// right edge
		i1 = roi.at<float>(0, minloc.x - 1);
		i2 = roi.at<float>(0, minloc.x);
		i3 = roi.at<float>(0, minloc.x + 1);
		denominator = 2 * i1 - 4 * i2 + 2 * i3;
		if (denominator != 0.f)
			x_right = minloc.x + (i1 - i3) / denominator;
		else
			x_right = minloc.x;
		right.push_back(Point2f(x_right, y));
#endif
	}
}

void drawCross(Mat& img, Point pt, Scalar color)
{
	int span = 3;
	line(img, pt + Point(-span, -span), pt + Point(span, span), color, 1, LINE_AA);
	line(img, pt + Point(-span, span), pt + Point(span, -span), color, 1, LINE_AA);
}
