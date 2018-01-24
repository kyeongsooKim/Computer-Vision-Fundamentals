#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
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

	Ptr<Feature2D> detector = AKAZE::create();

	vector<KeyPoint> keypoints_box, keypoints_scene;
	Mat desc_box, desc_scene;
	detector->detectAndCompute(img_box, Mat(), keypoints_box, desc_box);
	detector->detectAndCompute(img_scene, Mat(), keypoints_scene, desc_scene);

	BFMatcher matcher(NORM_HAMMING);
	vector<DMatch> matches;
	matcher.match(desc_box, desc_scene, matches);

	std::sort(matches.begin(), matches.end());
	vector<DMatch> good_matches(matches.begin(), matches.begin() + 80);

	Mat img_matches;
	drawMatches(img_box, keypoints_box, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	vector<Point2f> obj;
	vector<Point2f> scene;
	for (size_t i = 0; i < good_matches.size(); i++) {
		obj.push_back(keypoints_box[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	Mat H = findHomography(obj, scene, RANSAC);

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	//-- Get the corners from the image_1 ( the object to be "detected" )
	vector<Point2f> obj_corners(4);
	obj_corners[0] = Point2f(0, 0);
	obj_corners[1] = Point2f(img_box.cols, 0);
	obj_corners[2] = Point2f(img_box.cols, img_box.rows);
	obj_corners[3] = Point2f(0, img_box.rows);
	vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_box.cols, 0), scene_corners[1] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_box.cols, 0), scene_corners[2] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_box.cols, 0), scene_corners[3] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_box.cols, 0), scene_corners[0] + Point2f(img_box.cols, 0), Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("dst", img_matches);
	waitKey(0);
	return 0;
}
