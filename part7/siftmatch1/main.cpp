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

	// SIFT ��ü ����
	Ptr<Feature2D> detector = SIFT::create();

	// Ű����Ʈ & ����� ����
	vector<KeyPoint> keypoints_box, keypoints_scene;
	Mat desc_box, desc_scene;
	detector->detectAndCompute(img_box, Mat(), keypoints_box, desc_box);
	detector->detectAndCompute(img_scene, Mat(), keypoints_scene, desc_scene);

	// Flann ��� ��Ī ����
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(desc_box, desc_scene, matches);

	// ��Ī ������ ���� �͵鸸 �̿��Ͽ� ��� ���� ����
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