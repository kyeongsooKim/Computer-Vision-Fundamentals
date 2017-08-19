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

	Mat src_ycrcb;
	cvtColor(src, src_ycrcb, COLOR_BGR2YCrCb);

	vector<Mat> ycrcb_planes;
	split(src_ycrcb, ycrcb_planes);

	imshow("src", src);
	imshow("Y plane", ycrcb_planes[0]);
	imshow("Cr plane", ycrcb_planes[1]);
	imshow("Cb plane", ycrcb_planes[2]);

	waitKey(0);
	return 0;
}