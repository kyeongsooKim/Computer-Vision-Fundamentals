#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
#if 0
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
	Mat templ = imread("templ.bmp", IMREAD_GRAYSCALE);
#else
	Mat src = imread("wheres_wally.jpg", IMREAD_GRAYSCALE); // IMREAD_COLOR
	Mat templ = imread("wally.bmp", IMREAD_GRAYSCALE);
#endif

	if (src.empty() || templ.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

#if 0
	Mat noise(src.size(), CV_32S);
	randn(noise, 0, 10); // 10, 50, 100
	add(src, noise, src, noArray(), CV_8U);
#endif

#if 0
	GaussianBlur(src, src, Size(5, 5), 0.0);
	GaussianBlur(templ, templ, Size(5, 5), 0.0);
#endif

#if 0
	src = src + (src - 128) * 1.0;
	src = src + 50;
#endif

#if 0
	resize(src, src, Size(0, 0), 0.9, 0.9); // 0.8, 0.7
#endif

#if 0
	Mat rot = getRotationMatrix2D(Point2f(src.cols / 2.f, src.rows / 2.f), 10, 1); // 20, 30
	warpAffine(src, src, rot, src.size());
#endif

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	Mat res, res_norm;
	matchTemplate(src, templ, res, TM_CCOEFF_NORMED);
	normalize(res, res_norm, 0, 255, NORM_MINMAX, CV_8U);

	double minv, maxv;
	Point minloc, maxloc;
	minMaxLoc(res, &minv, &maxv, &minloc, &maxloc);

	cout << "minv: " << minv << ", maxv: " << maxv << endl;
	cout << "minloc: " << minloc << ", maxloc: " << maxloc << endl;

	rectangle(dst, Rect(maxloc.x, maxloc.y, templ.cols, templ.rows), Scalar(0, 255, 255), 2);

//	imshow("src", src);
	imshow("templ", templ);
	imshow("res_norm", res_norm);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}