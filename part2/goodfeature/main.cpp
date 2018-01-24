#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	ocl::setUseOpenCL(false);

	Mat src = imread("building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int maxCorners = 40;
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;

	int64 t1 = getTickCount();

	goodFeaturesToTrack(src,
		corners,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		useHarrisDetector,
		k);

	int64 t2 = getTickCount();
	cout << "It took " << (t2 - t1) * 1000 / getTickFrequency() << endl;

	Mat dst = src.clone();
	cvtColor(dst, dst, CV_GRAY2BGR);

	for (size_t i = 0; i < corners.size(); i++) {
		circle(dst, corners[i], 5, Scalar(0, 0, 255), 2);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}