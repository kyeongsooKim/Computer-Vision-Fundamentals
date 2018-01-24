#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat src;
	
	if (argc < 2) {
		src = imread("milkdrop.bmp", IMREAD_GRAYSCALE);
	} else {
		src = imread(argv[1], IMREAD_GRAYSCALE);
	}

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_bin;
	double otsu_th = threshold(src, src_bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
	cout << "Otsu threshold: " << otsu_th << endl;

	Mat dst_erode, dst_dilate;
	erode(src_bin, dst_erode, Mat());
	dilate(src_bin, dst_dilate, Mat());
	
	imshow("src", src);
	imshow("src_bin", src_bin);
	imshow("erosion", dst_erode);
	imshow("dilation", dst_dilate);

	waitKey(0);
	return 0;
}