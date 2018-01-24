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
		// �ܰ����� �ʹ� ª�ų� �Ǵ� �ʹ� ��� ����.
		if ((contours[idx].size() < 200) || (contours[idx].size() > 240000))
			continue;

		// �˻��� �ܰ����� �⺻������ ��������� �׸�.
		drawContours(dst, contours, idx, Scalar(0, 255, 255), 1, LINE_8, hierarchy);

		// ��� �ܰ����� �ٰ��� �ٻ�ȭ�Ͽ� �� �� �簢�� ��縸 ã��.
		vector<Point> approx;
		approxPolyDP(contours[idx], approx, contours[idx].size() * 0.02, true);

		if ((approx.size() == 4) && isContourConvex(approx)) {
			// ���������� ���ϵ� �ܰ���(���� �ܰ���)�� ������ ���� ������ ����.
			int inner_idx = hierarchy[idx][2];
			if (inner_idx < 0) continue;

			// �ܺ�&���� ��� �簢�� ����� ���, 
			vector<Point> approx2;
			approxPolyDP(contours[inner_idx], approx2, arcLength(contours[inner_idx], true) * 0.02, true);

			int rot;
			if (approx2.size() == 4) {
				if (check_marker(src, approx2, rot) > 0.5f) {
					// ���ο� hiro ��ũ�� ������ ������ �ܰ���
					drawContours(dst, contours, idx, Scalar(0, 0, 255), 2, LINE_8, hierarchy);
					circle(dst, approx2[rot], 5, Scalar(0, 128, 255), 2);
				} else {
					// ���ο� hiro ��ũ�� �ƴϸ� ��� �ܰ���
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