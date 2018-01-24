#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

const int TEMPL_SIZE = 32;
Mat src, templ;

enum { ROTATE_0 = 0, ROTATE_90, ROTATE_180, ROTATE_270 };

double check_marker(const Mat& src, const vector<Point> contour, int& rot)
{
	Point2f srcQuad[4], dstQuad[4];
	srcQuad[0] = contour[0];
	srcQuad[1] = contour[1];
	srcQuad[2] = contour[2];
	srcQuad[3] = contour[3];
	dstQuad[0].x = 0; dstQuad[0].y = 0;
	dstQuad[1].x = TEMPL_SIZE; dstQuad[1].y = 0;
	dstQuad[2].x = TEMPL_SIZE; dstQuad[2].y = TEMPL_SIZE;
	dstQuad[3].x = 0; dstQuad[3].y = TEMPL_SIZE;

	Mat dst, res;
	Mat per_mat = getPerspectiveTransform(srcQuad, dstQuad);
	Mat rot90 = getRotationMatrix2D(Point2f(TEMPL_SIZE / 2.f, TEMPL_SIZE / 2.f), 90.f, 1.f);
	float max_ccoeff = 0.f;

	warpPerspective(src, dst, per_mat, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_0;
	}

	warpAffine(dst, dst, rot90, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_90;
	}

	warpAffine(dst, dst, rot90, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_180;
	}

	warpAffine(dst, dst, rot90, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_270;
	}

	return max_ccoeff;
}

int main()
{
	src = imread("capture02.jpg", IMREAD_GRAYSCALE);
	templ = imread("hiro.bmp", IMREAD_GRAYSCALE);

	if (src.empty() || templ.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	resize(templ, templ, Size(TEMPL_SIZE, TEMPL_SIZE));

	double start_tick = (double)getTickCount();

	Mat src_bin;
	threshold(src, src_bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
//	adaptiveThreshold(src, src_bin, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 101, 0);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src_bin.clone(), contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		// 외곽선이 너무 짧거나 또는 너무 길면 무시.
		if ((contours[idx].size() < 200) || (contours[idx].size() > 240000))
			continue;

		// 검사할 외곽선은 기본적으로 노란색으로 그림.
		drawContours(dst, contours, idx, Scalar(0, 255, 255), 1, LINE_8, hierarchy);

		// 모든 외곽선을 다각형 근사화하여 이 중 사각형 모양만 찾음.
		vector<Point> approx;
		approxPolyDP(contours[idx], approx, contours[idx].size() * 0.02, true);

		if ((approx.size() == 4) && isContourConvex(approx)) {
			// 계층적으로 차일드 외곽선(내부 외곽선)을 가지고 있지 않으면 무시.
			int inner_idx = hierarchy[idx][2];
			if (inner_idx < 0) continue;

			// 외부&내부 모두 사각형 모양인 경우, 
			vector<Point> approx2;
			approxPolyDP(contours[inner_idx], approx2, arcLength(contours[inner_idx], true) * 0.02, true);

			int rot;
			if (approx2.size() == 4) {
				if (check_marker(src, approx2, rot) > 0.5f) {
					// 내부에 hiro 마크가 있으면 빨간색 외곽선
					drawContours(dst, contours, idx, Scalar(0, 0, 255), 2, LINE_8, hierarchy);
					circle(dst, approx2[rot], 5, Scalar(0, 128, 255), 2);
				} else {
					// 내부에 hiro 마크가 아니면 녹색 외곽선
					drawContours(dst, contours, idx, Scalar(0, 255, 0), 2, LINE_8, hierarchy);
					//drawContours(dst, vector<vector<Point> >(1, approx), 0, Scalar(0, 0, 255), 2, LINE_8, Mat());
				}
			}
		}
	}

	double duration_ms = (double(getTickCount()) - start_tick) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	imshow("src", src);
	imshow("src_bin", src_bin);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}