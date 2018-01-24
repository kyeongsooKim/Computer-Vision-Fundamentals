#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src1 = imread("lenna.bmp", IMREAD_GRAYSCALE);
	Mat src2 = imread("square.bmp", IMREAD_GRAYSCALE);

	if (src1.empty() || src2.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	if (src1.rows != src2.rows || src1.cols != src2.cols || src1.type() != src2.type()) {
		cerr << "The images are not the same dimention!" << endl;
		return -1;
	}

	imshow("src1", src1);
	imshow("src2", src2);

	Mat dst(src1.rows, src1.cols, src1.type());

	cout << "bitwise_and..." << endl;
	bitwise_and(src1, src2, dst); imshow("dst", dst); waitKey(0);

	cout << "bitwise_or..." << endl;
	bitwise_or(src1, src2, dst); imshow("dst", dst); waitKey(0);

	cout << "bitwise_xor..." << endl;
	bitwise_xor(src1, src2, dst); imshow("dst", dst); waitKey(0);

	cout << "bitwise_not..." << endl;
	bitwise_not(src1, dst); imshow("dst", dst); waitKey(0);

	return 0;
}