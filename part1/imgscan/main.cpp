#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void inverse1(Mat& img);
void inverse2(Mat& img);
void inverse3(Mat& img);

int main(int argc, char* argv[])
{
	Mat src;

	if (argc == 2 && (!strcmp(argv[1], "G")))
		src = imread("lenna.bmp", IMREAD_GRAYSCALE);
	else
		src = imread("lenna.bmp");

	if (src.empty()) {
		cerr << "Can't load lenna.bmp file!" << endl;
		return -1;
	}

	cout << "> Width: " << src.cols << ", Height: " << src.rows;
	cout << ", Channels: " << src.channels() << endl;

	namedWindow("src");
	imshow("src", src);
	waitKey(0);

	const int TIMES = 100;
	int64 t1, t2;
	double ms;

	// inverse1
	t1 = getTickCount();
	for (int i = 0; i < TIMES; i++) 
		inverse1(src);
	t2 = getTickCount();
	ms = 1000 * (t2 - t1) / getTickFrequency();
	ms /= TIMES;

	cout << "1. 행 단위 포인터 접근(ptr) 방법:\t" << ms << " ms." << endl;

	waitKey(0);

	// inverse2
	t1 = getTickCount();
	for (int i = 0; i < TIMES; i++)
		inverse2(src);
	t2 = getTickCount();
	ms = 1000 * (t2 - t1) / getTickFrequency();
	ms /= TIMES;

	cout << "2. 반복자(iterator) 사용 방법:\t\t" << ms << " ms." << endl;

	waitKey(0);

	// inverse3
	t1 = getTickCount();
	for (int i = 0; i < TIMES; i++)
		inverse3(src);
	t2 = getTickCount();
	ms = 1000 * (t2 - t1) / getTickFrequency();
	ms /= TIMES;

	cout << "3. 임의 접근 연산자(at) 사용 방법:\t" << ms << " ms." << endl;

	waitKey(0);

	return 0;
}

void inverse1(Mat& img)
{
	if (img.channels() == 1) {
		for (int j = 0; j < img.rows; j++) {
			uchar* p = img.ptr<uchar>(j);
			for (int i = 0; i < img.cols; i++) {
				p[i] = 255 - p[i];
			}
		}
	} else if (img.channels() == 3) {
		for (int j = 0; j < img.rows; j++) {
			Vec3b* p = img.ptr<Vec3b>(j);
			for (int i = 0; i < img.cols; i++) {
				p[i][0] = 255 - p[i][0];
				p[i][1] = 255 - p[i][1];
				p[i][2] = 255 - p[i][2];
			}
		}
	}
}

void inverse2(Mat& img)
{
	if (img.channels() == 1) {
		MatIterator_<uchar> it, end;
		for (it = img.begin<uchar>(), end = img.end<uchar>(); it != end; it++) {
			*it = 255 - *it;
		}
	} else if (img.channels() == 3) {
		MatIterator_<Vec3b> it, end;
		for (it = img.begin<Vec3b>(), end = img.end<Vec3b>(); it != end; it++) {
			(*it)[0] = 255 - (*it)[0];
			(*it)[1] = 255 - (*it)[1];
			(*it)[2] = 255 - (*it)[2];
		}
	}
}

void inverse3(Mat& img)
{
	if (img.channels() == 1) {
		for (int j = 0; j < img.rows; j++) {
			for (int i = 0; i < img.cols; i++) {
				img.at<uchar>(j, i) = 255 - img.at<uchar>(j, i);
			}
		}
	} else if (img.channels() == 3) {
		for (int j = 0; j < img.rows; j++) {
			for (int i = 0; i < img.cols; i++) {
				img.at<Vec3b>(j, i)[0] = 255 - img.at<Vec3b>(j, i)[0];
				img.at<Vec3b>(j, i)[1] = 255 - img.at<Vec3b>(j, i)[1];
				img.at<Vec3b>(j, i)[2] = 255 - img.at<Vec3b>(j, i)[2];
			}
		}
	}
}
