#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

const int TEMPL_SIZE = 32;
Mat frame, gray, templ, lenna;
Size sizeVideo;

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

void insertLenna(Mat& dst, vector<Point> corners, int rot)
{
	vector<Point2f> srcQuad(4), dstQuad(4);
	srcQuad[0] = Point2f(0, 0);
	srcQuad[1] = Point2f(lenna.cols - 1, 0);
	srcQuad[2] = Point2f(lenna.cols - 1, lenna.rows - 1);
	srcQuad[3] = Point2f(0, lenna.rows - 1);

	dstQuad[0].x = corners[(0 + rot) % 4].x; dstQuad[0].y = corners[(0 + rot) % 4].y;
	dstQuad[1].x = corners[(1 + rot) % 4].x; dstQuad[1].y = corners[(1 + rot) % 4].y;
	dstQuad[2].x = corners[(2 + rot) % 4].x; dstQuad[2].y = corners[(2 + rot) % 4].y;
	dstQuad[3].x = corners[(3 + rot) % 4].x; dstQuad[3].y = corners[(3 + rot) % 4].y;

#if 1
	Size subPixWinSize(5, 5), winSize(11, 11);
	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
	cornerSubPix(gray, dstQuad, subPixWinSize, Size(-1, -1), termcrit);
#endif

	Mat lenna_warp;
	Mat p = getPerspectiveTransform(srcQuad, dstQuad);
	warpPerspective(lenna, lenna_warp, p, sizeVideo);

	Mat gray;
	cvtColor(lenna_warp, gray, COLOR_BGR2GRAY);

	Mat mask = gray > 0;
	lenna_warp.copyTo(dst, mask);
}

int main()
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Camera open failed!" << endl;
		return -1;
	}

	templ = imread("hiro.bmp", IMREAD_GRAYSCALE);
	lenna = imread("lenna.bmp", IMREAD_COLOR);

	if (lenna.empty() || templ.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	sizeVideo.width = int(cap.get(CAP_PROP_FRAME_WIDTH));
	sizeVideo.height = int(cap.get(CAP_PROP_FRAME_HEIGHT));
	cout << "Video Size: " << sizeVideo << endl;

	resize(templ, templ, Size(TEMPL_SIZE, TEMPL_SIZE));

	Mat bin;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	while (1) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Frame is empty!" << endl;
			break;
		}

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

		findContours(bin, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

		if (contours.size() > 0) {
			for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
				// 외곽선이 너무 짧거나 또는 너무 길면 무시.
				if ((contours[idx].size() < 200) || (contours[idx].size() > 240000))
					continue;

				// 모든 외곽선을 다각형 근사화하여 이 중 사각형 모양만 찾음.
				vector<Point> approx;
				approxPolyDP(contours[idx], approx, contours[idx].size() * 0.02, true);

				if (approx.size() == 4 && isContourConvex(approx)) {
					// 계층적으로 차일드 외곽선(내부 외곽선)을 가지고 있지 않으면 무시.
					int inner_idx = hierarchy[idx][2];
					if (inner_idx < 0) continue;

					// 외부&내부 모두 사각형 모양인 경우, 
					vector<Point> approx2;
					approxPolyDP(contours[inner_idx], approx2, contours[inner_idx].size() * 0.02, true);

					int rot;
					if (approx2.size() == 4) {
						if (check_marker(gray, approx2, rot) > 0.4f) {
							insertLenna(frame, approx2, rot);
							//circle(frame, approx2[rot], 5, Scalar(0, 128, 255), 2);
						}
					}
				}
			}
		}

		imshow("frame", frame);

		if (waitKey(5) == 27)
			break;
	}
	
	return 0;
}