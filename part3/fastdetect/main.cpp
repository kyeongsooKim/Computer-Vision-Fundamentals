#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	cv::ocl::setUseOpenCL(false);

	Mat src = imread("building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	int64 t1 = getTickCount();

	vector<KeyPoint> keypoints;
	FAST(src, keypoints, 60, true);
//	Ptr<FeatureDetector> detector = FastFeatureDetector::create(60);
//	detector->detect(src, keypoints);

	int64 t2 = getTickCount();
	cout << "It took " << (t2 - t1) * 1000 / getTickFrequency() << endl;

	for (KeyPoint kp : keypoints) {
		circle(dst, Point(kp.pt.x, kp.pt.y), 5, Scalar(0, 0, 255), 2);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}