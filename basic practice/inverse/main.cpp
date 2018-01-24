//#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	

#if 0 //using function in openCV
	Mat dst;
	dst = 255 - src;

#else  //change pixel by pixel, just for checking how to access each pixel, using .at()
	
	//Mat dst(src.rows, src.cols, src.type());

	//line 29+30 = line 26
	Mat dst;
	dst = Mat::zeros(src.rows, src.cols, src.type());

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);
		}
	}
#endif

	namedWindow("src");
	imshow("src", src);

	namedWindow("dst");
	imshow("dst", dst);

	waitKey(0);
	return 0;
}