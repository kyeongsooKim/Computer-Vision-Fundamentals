#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main(void)
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	Mat src = imread("binch.jpg", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	// SURF 객체 생성
	int th = 800;
	Ptr<Feature2D> detector = SURF::create(th);

	// 키포인트 & 기술자 생성
	vector<KeyPoint> keypoints_src, keypoints_frame;
	Mat desc_src, desc_frame;
	detector->detectAndCompute(src, Mat(), keypoints_src, desc_src);

	namedWindow("camera");

	Mat frame;
	Mat img_matches;

	for (;;) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		double start = double(getTickCount());

		detector->detectAndCompute(frame, Mat(), keypoints_frame, desc_frame);

		if (keypoints_frame.size() > 80) {

			// Flann 기반 매칭 수행
			FlannBasedMatcher matcher;
			vector<DMatch> matches;
			matcher.match(desc_src, desc_frame, matches);

			std::sort(matches.begin(), matches.end());
			vector<DMatch> good_matches(matches.begin(), matches.begin() + 40);

			// 호모그래피 계산
			vector<Point2f> obj;
			vector<Point2f> scene;
			for (size_t i = 0; i < good_matches.size(); i++) {
				//-- Get the keypoints from the good matches
				obj.push_back(keypoints_src[good_matches[i].queryIdx].pt);
				scene.push_back(keypoints_frame[good_matches[i].trainIdx].pt);
			}

			Mat H = findHomography(obj, scene, RANSAC);

			double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
			cout << "It took " << duration_ms << " ms." << endl;

			drawMatches(src, keypoints_src, frame, keypoints_frame,
				good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
				vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

			// 정확한 객체 위치 계산 & 그리기
			vector<Point2f> obj_corners(4);
			obj_corners[0] = Point2f(0, 0);
			obj_corners[1] = Point2f(src.cols, 0);
			obj_corners[2] = Point2f(src.cols, src.rows);
			obj_corners[3] = Point2f(0, src.rows);

			vector<Point2f> scene_corners(4);
			perspectiveTransform(obj_corners, scene_corners, H);

			line(img_matches, scene_corners[0] + Point2f(src.cols, 0), scene_corners[1] + Point2f(src.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[1] + Point2f(src.cols, 0), scene_corners[2] + Point2f(src.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[2] + Point2f(src.cols, 0), scene_corners[3] + Point2f(src.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[3] + Point2f(src.cols, 0), scene_corners[0] + Point2f(src.cols, 0), Scalar(0, 255, 0), 4);

			imshow("img_matches", img_matches);
		}

		if (waitKey(5) >= 0)
			break;
	}

	return 0;
}

