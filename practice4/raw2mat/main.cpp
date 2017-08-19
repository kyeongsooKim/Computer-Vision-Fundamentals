#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	// Raw 파일 열기
	FILE* fp = fopen("elaine.raw", "rb");
	if (fp == NULL) {
		cout << "File load error!" << endl;
		return -1;
	}

	// 연속된 메모리 동적 할당 & 파일 읽기
	int width = 512;
	int height = 512;
	int area = width * height;
	int step = 512;

	uchar* pixels = new uchar[area];
	int ret = (int)fread(pixels, sizeof(uchar), area, fp);
	if (ret != area) {
		cout << "fread failed!" << endl;
		delete [] pixels;
		return -1;
	}

	fclose(fp);

	// 이미 존재하는 메모리 버퍼를 참조하는 Mat 객체 생성
	Mat src(height, width, CV_8UC1, pixels, step);

	// OpenCV Operations
	Mat dst;
	bilateralFilter(src, dst, -1, 20, 5);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);

	delete [] pixels;

	return 0;
}
