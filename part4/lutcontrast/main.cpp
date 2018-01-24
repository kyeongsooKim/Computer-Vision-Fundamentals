#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	ocl::setUseOpenCL(false);

	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	
	resize(src, src, Size(), 10, 10);
	cout << "Image size: " << src.size() << endl;

	Mat dst;
	int64 t1, t2;
	double ms;

	// 1. Mat operator overloading
	t1 = getTickCount();

	dst = 2 * src - 128;

	t2 = getTickCount();
	ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "1. Operator overloading: " << ms << " ms." << endl;

	// 2. Pixel access by ptr()
	t1 = getTickCount();

	dst = Mat::zeros(src.rows, src.cols, src.type());
	for (int j = 0; j < src.rows; j++) {
		uchar* pSrc = src.ptr<uchar>(j);
		uchar* pDst = dst.ptr<uchar>(j);
		for (int i = 0; i < src.cols; i++) {
			pDst[i] = saturate_cast<uchar>(2 * pSrc[i] - 128);
		}
	}

	t2 = getTickCount();
	ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "2. Pixel access by ptr(): " << ms << " ms." << endl;

	// 3. Look-up table
	Mat lut(1, 256, CV_8U);
	uchar* p = lut.ptr(0);
	for (int i = 0; i < 256; i++) {
		p[i] = saturate_cast<uchar>(2 * i - 128);
	}

	t1 = getTickCount();

	LUT(src, lut, dst);
	
	t2 = getTickCount();
	ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "3. Lookup table: " << ms << " ms." << endl;

	// 4. forEach method
	t1 = getTickCount();

	dst = src.clone();
	dst.forEach<uchar>([](uchar &p, const int * position) -> void {
		p = saturate_cast<uchar>(2 * p - 128);
	});

	t2 = getTickCount();
	ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "4. forEach method: " << ms << " ms." << endl;

	return 0;
}
