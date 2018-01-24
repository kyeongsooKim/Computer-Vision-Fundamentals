#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void setLabel(Mat& img, const string label, vector<Point>& contour)
{
	polylines(img, contour, true, Scalar(0, 255, 255), 2);

	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(img, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	putText(img, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, LINE_8);
}

int main(int argc, char* argv[])
{
	Mat src = imread("polygon.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow("src");
	imshow("src", src);

	Mat src_bin;
	threshold(src, src_bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	erode(src_bin, src_bin, Mat());
	dilate(src_bin, src_bin, Mat());

	namedWindow("src_bin");
	imshow("src_bin", src_bin);

	vector<vector<Point> > contours;
	findContours(src_bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (unsigned i = 0; i < contours.size(); i++) {
		vector<Point> approx;
		approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);

		if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
			continue;

		int vtc = int(approx.size());

		if (vtc == 3) {
			setLabel(dst, "TRI", contours[i]);
		} else if (vtc == 4) {
			setLabel(dst, "RECT", contours[i]);
		} else if (vtc == 5) {
			setLabel(dst, "PENTA", contours[i]);
		} else if (vtc == 6) {
			setLabel(dst, "HEXA", contours[i]);
		} else {
			// ¿ø °ËÃâ
			double area = contourArea(contours[i]);
			double len = arcLength(contours[i], true);

			if ((4. * CV_PI * area / (len * len)) > 0.9) {
				setLabel(dst, "CIR", contours[i]);
			}
		}
	}

	namedWindow("dst");
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
