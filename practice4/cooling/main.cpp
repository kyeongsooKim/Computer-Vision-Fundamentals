#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("girl.jpg", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	vector<Mat> channels(3);
	split(src, channels);

	Mat lut(1, 256, CV_8U);
	uchar* p = lut.ptr();

	// B 채널 커브 변화
	for (int i = 0; i < 100; i++)
		p[i] = cvRound(3 * i / 2.f);

	for (int i = 100; i < 256; i++)
		p[i] = cvRound((21 * i + 2550) / 31.f);

	LUT(channels[0], lut, channels[0]);

	// R 채널 커브 변화
	for (int i = 0; i < 150; i++)
		p[i] = cvRound(2 * i / 3.f);

	for (int i = 150; i < 256; i++)
		p[i] = cvRound((31 * i - 2550) / 21.f);

	LUT(channels[2], lut, channels[2]);

	Mat dst;
	merge(channels, dst);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}
