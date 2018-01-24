#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
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

	// 매칭 점수가 높은 것들만 이용하여 결과 영상 생성
	Mat img_matches;
	drawMatches(img_box, keypoints_box, img_scene, keypoints_scene,
		matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

//	imshow("img_box", img_box);
//	imshow("img_scene", img_scene);
	imshow("img_matches", img_matches);
	waitKey(0);
	return 0;
}