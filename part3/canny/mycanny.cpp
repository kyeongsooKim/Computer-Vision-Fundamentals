#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "mycanny.h"
#include <iostream>

using namespace cv;
using namespace std;

void myCanny(const Mat& src, Mat& dst, double threshold1, double threshold2)
{
	// 1. Gaussian filter
	// not necessarily needed, since it might delete some noises but it can blur the edges
	// at the same time.
	Mat gauss;
	GaussianBlur(src, gauss, Size(), 0.5);

	// 2. Gradient by Sobel operator
	Mat dx, dy;
	Sobel(gauss, dx, CV_32F, 1, 0);
	Sobel(gauss, dy, CV_32F, 0, 1);

	Mat mag(src.rows, src.cols, CV_32F);
	Mat ang(src.rows, src.cols, CV_32F);
	
	for (int j = 0; j < src.rows; j++) {
		float* pDx = dx.ptr<float>(j);
		float* pDy = dy.ptr<float>(j);
		float* pMag = mag.ptr<float>(j);
		float* pAng = ang.ptr<float>(j);

		for (int i = 0; i < src.cols; i++) {
			pMag[i] = sqrt(pDx[i] * pDx[i] + pDy[i] * pDy[i]);

			if (pDx[i] == 0)
				pAng[i] = 90.f;
			else
				pAng[i] = float(atan(pDy[i] / pDx[i]) * 180 / CV_PI);
		}
	}

	// 3. Non-maximum suppression
	enum DISTRICT { AREA0 = 0, AREA45, AREA90, AREA135, NOAREA };

	const uchar STRONG_EDGE = 255;
	const uchar WEAK_EDGE = 128;

	vector<Point2i> strong_edges;

	dst = Mat::zeros(src.rows, src.cols, CV_8U);

	int mag_value;
	bool local_max;
	int ang_array[] = { AREA0, AREA45, AREA45, AREA90, AREA90, AREA135, AREA135, AREA0 };
	for (int j = 1; j < src.rows - 1; j++) {
		for (int i = 1; i < src.cols - 1; i++) {

			// �׷����Ʈ ũ�Ⱑ th_low���� ū �ȼ��� ���ؼ��� ������ �ִ� �˻�.
			// ������ �ִ��� �ȼ��� ���ؼ��� ���� ���� �Ǵ� ���� ������ ����.
			mag_value = cvRound(mag.at<float>(j, i));
			if (mag_value > threshold1) {

				// �׷����Ʈ ���� ��� (4�� ����)
				int ang_idx = cvRound(ang.at<float>(j, i) / 22.5f);
				if (ang_idx > 8) ang_idx -= ang_idx;

				// ������ �ִ� �˻�
				local_max = false;
				switch (ang_array[ang_idx]) {
				case AREA0:
					if ((mag_value >= mag.at<float>(j, i - 1)) && (mag_value > mag.at<float>(j, i + 1)))
						local_max = true;
					break;
				case AREA45:
					if ((mag_value >= mag.at<float>(j - 1, i - 1)) && (mag_value > mag.at<float>(j + 1, i + 1)))
						local_max = true;
					break;
				case AREA90:
					if ((mag_value >= mag.at<float>(j - 1, i)) && (mag_value > mag.at<float>(j + 1, i)))
						local_max = true;
					break;
				case AREA135:
				default:
					if ((mag_value >= mag.at<float>(j - 1, i + 1)) && (mag_value > mag.at<float>(j + 1, i - 1)))
						local_max = true;
					break;
				}

				// ���� ������ ���� ���� ����.
				if (local_max) {
					if (mag_value > threshold2) {
						dst.at<uchar>(j, i) = STRONG_EDGE;
						strong_edges.push_back(Point2i(i, j));
					} else
						dst.at<uchar>(j, i) = WEAK_EDGE;
				}
			}
		}
	}


#define CHECK_WEAK_EDGE(x, y) \
	if (dst.at<uchar>(y, x) == WEAK_EDGE) { \
		dst.at<uchar>(y, x) = STRONG_EDGE; \
		strong_edges.push_back(Point2i(x, y)); \
	}

	// 4. Hysterisis edge tracking
	while (!strong_edges.empty()) {
		Point2i p = strong_edges.back();
		strong_edges.pop_back();

		// ���� ���� �ֺ��� ���� ������ ���� ����(���� ����)�� ����
		CHECK_WEAK_EDGE(p.x + 1, p.y)
		CHECK_WEAK_EDGE(p.x + 1, p.y + 1)
		CHECK_WEAK_EDGE(p.x, p.y + 1)
		CHECK_WEAK_EDGE(p.x - 1, p.y + 1)
		CHECK_WEAK_EDGE(p.x - 1, p.y)
		CHECK_WEAK_EDGE(p.x - 1, p.y - 1)
		CHECK_WEAK_EDGE(p.x, p.y - 1)
		CHECK_WEAK_EDGE(p.x + 1, p.y - 1)
	}

	// ������ ���� ������ �����ִ� �ȼ��� ��� ������ �ƴ� ������ �Ǵ�.
	for (int j = 0; j < src.rows; j++) {
		for (int i = 0; i < src.cols; i++) {
			if (dst.at<uchar>(j, i) == WEAK_EDGE)
				dst.at<uchar>(j, i) = 0;
		}
	}
}
