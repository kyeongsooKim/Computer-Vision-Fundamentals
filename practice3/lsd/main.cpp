#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Ptr<LineSegmentDetector> lsd = createLineSegmentDetector();
	vector<Vec4f> lines;

	double start = double(getTickCount());

	lsd->detect(src, lines);
	
	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	Mat dst(src);
	lsd->drawSegments(dst, lines);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}