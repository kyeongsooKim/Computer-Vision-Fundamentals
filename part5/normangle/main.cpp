#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat src;
	
	if (argc < 2)
		src = imread("sample1.bmp", IMREAD_GRAYSCALE); 
	else
		src = imread(argv[1], IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Moments m = moments(src, true);
	double theta = atan2(2 * m.mu11, m.mu20 - m.mu02) / 2;
	double degree = theta * 180 / CV_PI + 90;
	cout << "degree = " << degree << endl;
	
	Point center(m.m10 / m.m00, m.m01 / m.m00);
	Mat r = getRotationMatrix2D(center, degree, 1.0);

	Mat dst;
	warpAffine(src, dst, r, Size(src.cols, src.rows));

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
