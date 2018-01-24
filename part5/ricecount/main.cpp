#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("rice.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst, dst_open;
	int th = threshold(src, dst, 0, 255, THRESH_BINARY | THRESH_OTSU);

	Mat labels;
	int num_objects;
	
	num_objects = connectedComponents(dst, labels, 4);
	cout << "# of objects in dst: " << num_objects << endl;

	morphologyEx(dst, dst_open, MORPH_OPEN, Mat());
	num_objects = connectedComponents(dst_open, labels, 4);
	cout << "# of objects in dst_open: " << num_objects << endl;

	imshow("src", src);
	imshow("dst", dst);
	imshow("dst_open", dst_open);

	waitKey(0);
	return 0;
}

