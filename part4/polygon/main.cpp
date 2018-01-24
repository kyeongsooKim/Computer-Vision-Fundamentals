#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int gThreshold = 128;
Mat src, dst, src_th;

void on_threshold(int, void*)
{
	threshold(src, src_th, gThreshold, 255, THRESH_BINARY_INV);
	erode(src_th, src_th, Mat());
	dilate(src_th, src_th, Mat());
	cvtColor(src, dst, COLOR_GRAY2BGR);

	Mat labels, stats, centroids;
	int num_labels = connectedComponentsWithStats(src_th, labels, stats, centroids);

	for (int i = 1; i < num_labels; i++) {
		int* pStats = stats.ptr<int>(i);

		if (pStats[CC_STAT_AREA] >(256 * 256) ||
			pStats[CC_STAT_AREA] < 20)
			continue;

		Rect rect(pStats[CC_STAT_LEFT], pStats[CC_STAT_TOP], pStats[CC_STAT_WIDTH], pStats[CC_STAT_HEIGHT]);
		Point center(centroids.at<double>(i, 0), centroids.at<double>(i, 1));

		rectangle(dst, rect, Scalar(0, 255, 255), 2);
		circle(dst, center, 2, Scalar(0, 0, 255), 2);
	}

	imshow("src_th", src_th);
	imshow("dst", dst);
}

int main()
{
	src = imread("polygon.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow("src");
	imshow("src", src);

	namedWindow("src_th");
	namedWindow("dst");

	createTrackbar("Threshold", "src_th", &gThreshold, 255, on_threshold);
	on_threshold(0, 0);

	waitKey(0);
	return 0;
}