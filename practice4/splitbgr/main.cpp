#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("candies.png", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	imshow("src", src);
	imshow("B plane", bgr_planes[0]);
	imshow("G plane", bgr_planes[1]);
	imshow("R plane", bgr_planes[2]);

	waitKey(0);
	return 0;
}