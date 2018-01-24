#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("capture01.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

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

			if (approx2.size() == 4) {
				drawContours(dst, contours, idx, Scalar(0, 255, 0), 2, LINE_8, hierarchy);
			}
		}
	}

	imshow("src", src);
	imshow("src_bin", src_bin);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}