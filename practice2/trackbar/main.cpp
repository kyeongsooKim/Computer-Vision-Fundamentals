#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int gBrightness = 100;
Mat src, dst;

void on_brightness(int pos, void* data)
{


	dst = src + (gBrightness - 100);
	cout << gBrightness << " (using global variable gBrightness)\n" << pos
		<< " (using local variable pos)\n" << endl;
	//imshow("dst", dst);
}

int main()
{
	src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	//need this line when we put imshow("dst",dst); (line19) before line 35
	//dst.create(src.rows, src.cols, src.type());


	namedWindow("dst"); // need this line when imshow() comes later than createTrack()
	createTrackbar("Brightness", "dst", &gBrightness, 200, on_brightness, NULL);
	on_brightness(0, 0);

	imshow("src", src);


	waitKey(0);
	return 0;
}