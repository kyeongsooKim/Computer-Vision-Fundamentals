#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat img_digits[10];

bool load_digits();
int  find_digit(Mat& img);
void set_label(Mat& img, int idx, vector<Point>& contour);

int main()
{
	Mat src = imread("digits.png");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	if (!load_digits()) {
		cerr << "Digit image load failed!" << endl;
		return -1;
	}

	Mat src_gray, src_bin;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
//	GaussianBlur(src_gray, src_gray, Size(11, 11), 2.);
	threshold(src_gray, src_bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	vector<vector<Point> > contours;
	findContours(src_bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat dst = src.clone();

	for (unsigned i = 0; i < contours.size(); i++) {
		if (contourArea(contours[i]) < 1000) continue;

		Rect rect = boundingRect(contours[i]);
		int digit = find_digit(src_gray(rect));

		drawContours(dst, contours, i, Scalar(0, 255, 255), 1);
		set_label(dst, digit, contours[i]);
	}

//	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

bool load_digits()
{
	for (int i = 0; i < 10; i++) {
		String filename = format("./digits/digit%d.bmp", i);
		img_digits[i] = imread(filename, IMREAD_GRAYSCALE);
		if (img_digits[i].empty())
			return false;
	}

	return true;
}

int find_digit(Mat& img)
{
	int max_idx = -1;
	float max_ccoeff = -9999;

	for (int i = 0; i < 10; i++) {
		Mat input, result;
		resize(img, input, Size(100, 150));
		matchTemplate(input, img_digits[i], result, TM_CCOEFF_NORMED);

		float ccoeff = result.at<float>(0, 0);

		if (ccoeff > max_ccoeff) {
			max_idx = i;
			max_ccoeff = ccoeff;
		}
	}

	return max_idx;
}

void set_label(Mat& img, int digit, vector<Point>& contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.8;
	int thickness = 2;
	int baseline = 2;

	String label = format("%d", digit);

	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(img, pt + Point(0, 2), pt + Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	putText(img, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, LINE_8);
}