#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat src;
	
	if (argc < 2) {
		src = imread("lenna.bmp", IMREAD_GRAYSCALE);
	} else {
		src = imread(argv[1], IMREAD_GRAYSCALE);
	}

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	CascadeClassifier cascade("haarcascade_frontalface_default.xml");
	if (cascade.empty()) {
		cerr << "Failed to open xml file!" << endl;
		return -1;
	}

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	double start = double(getTickCount());

	vector<Rect> faces;
	cascade.detectMultiScale(src, faces, 1.1, 3, 0, Size(40, 40));
	for (size_t i = 0; i < faces.size(); i++) {
		rectangle(dst, faces[i], Scalar(255, 0, 255), 3);
	}

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}