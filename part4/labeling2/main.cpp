#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	uchar data[] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 1, 1, 1, 0, 1, 0,
		0, 1, 1, 1, 1, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};
	Mat src(8, 8, CV_8U, data);

	Mat labels, stats, centroids;
	int num_labels = connectedComponentsWithStats(src, labels, stats, centroids);

	cout << "centroids:" << endl;
	for (int i = 0; i < num_labels; i++) {
		cout << "label #" << i << ": " << 
			centroids.at<double>(i, 0) << ", " << 
			centroids.at<double>(i, 1) << endl;
	}

	return 0;
}