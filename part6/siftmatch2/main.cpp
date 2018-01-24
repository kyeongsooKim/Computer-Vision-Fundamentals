#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
	cv::ocl::setUseOpenCL(false);

	Mat img_box = imread("box.png", IMREAD_GRAYSCALE);
	Mat img_scene = imread("box_in_scene.png", IMREAD_GRAYSCALE);

	if (img_box.empty() || img_scene.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	double start = double(getTickCount());

	// SIFT 객체 생성
	Ptr<Feature2D> detector = SIFT::create();

	// 키포인트 & 기술자 생성
	vector<KeyPoint> keypoints_box, keypoints_scene;
	Mat desc_box, desc_scene;
	detector->detectAndCompute(img_box, Mat(), keypoints_box, desc_box);
	detector->detectAndCompute(img_scene, Mat(), keypoints_scene, desc_scene);

	// Flann 기반 매칭 수행
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(desc_box, desc_scene, matches);

	// 매칭 점수가 높은 것들만 필터링
	double min_dist = 100;
	for (int i = 0; i < desc_box.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
	}

	vector<DMatch> good_matches;
	for (int i = 0; i < desc_box.rows; i++) {
		if (matches[i].distance <= 3 * min_dist) {
			good_matches.push_back(matches[i]);
		}
	}

	// 매칭 점수가 높은 것들만 이용하여 결과 영상 생성
	Mat img_matches;
	drawMatches(img_box, keypoints_box, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	// 호모그래피 계산
	vector<Point2f> obj;
	vector<Point2f> scene;
	for (size_t i = 0; i < good_matches.size(); i++) {
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_box[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, RANSAC);

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	// 정확한 객체 위치 계산 & 그리기
	vector<Point2f> obj_corners(4);
	obj_corners[0] = Point2f(0, 0);
	obj_corners[1] = Point2f(img_box.cols, 0);
	obj_corners[2] = Point2f(img_box.cols, img_box.rows);
	obj_corners[3] = Point2f(0, img_box.rows);
	vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);

	line(img_matches, scene_corners[0] + Point2f(img_box.cols, 0), scene_corners[1] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_box.cols, 0), scene_corners[2] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_box.cols, 0), scene_corners[3] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_box.cols, 0), scene_corners[0] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);

//	imshow("img_box", img_box);
//	imshow("img_scene", img_scene);
	imshow("img_matches", img_matches);
	waitKey(0);
	return 0;
}