#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	
	resize(src, src, Size(), 10, 10);
	cout << "Image size: " << src.size() << endl;

	const int TIMES = 100;
	int64 t1, t2;
	double ms;

	// 1. Mat operator overloading
	t1 = getTickCount();

	for (int i = 0; i < TIMES; i++) {
		src = 255 - src;
	}

	t2 = getTickCount();
	ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "1. Mat operator overloading: " << ms << " ms." << endl;

	// 2. Pixel access by ptr()
	t1 = getTickCount();

	for (int i = 0; i < TIMES; i++) {
		for (int j = 0; j < src.rows; j++) {
			uchar* p = src.ptr<uchar>(j);
			for (int i = 0; i < src.cols; i++) {
				p[i] = 255 - p[i];
			}
		}
	}

	t2 = getTickCount();
	ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "2. Pixel access by ptr(): " << ms << " ms." << endl;

	// 3. Mat forEach() method
	t1 = getTickCount();

	for (int i = 0; i < TIMES; i++) {
		src.forEach<uchar>([](uchar &p, const int * position) -> void {
			p = 255 - p;
		});
	}

	t2 = getTickCount();
	ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "3. Mat forEach() method: " << ms << " ms." << endl;

	return 0;
}
