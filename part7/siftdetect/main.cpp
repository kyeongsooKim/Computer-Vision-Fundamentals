#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	double start = double(getTickCount());

	Ptr<Feature2D> detector = SIFT::create();

	vector<KeyPoint> keypoints;
	detector->detect(src, keypoints);

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	Mat dst;
	drawKeypoints(src, keypoints, dst, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}