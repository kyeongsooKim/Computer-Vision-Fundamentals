#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat src;
	
	if (argc < 2)
		src = imread("digits.png", IMREAD_GRAYSCALE);
	else
		src = imread(argv[1], IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	cout << "> Image sizes: " << src.cols << " x " << src.rows << endl;

	Mat src_bin;
	threshold(src, src_bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	imshow("src_bin", src_bin);

	double t1, t2, duration_ms;

	// (1) connectedComponents
	t1 = double(getTickCount());

	Mat labels1;
	int num_labels1 = connectedComponents(src_bin, labels1);

	t2 = double(getTickCount());
	duration_ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "> connectedComponents() took " << duration_ms << " ms." << endl;

	// (2) connectedComponentsWithStats
	t1 = double(getTickCount());

	Mat labels2, stats, centroids;
	int num_labels2 = connectedComponentsWithStats(src_bin, labels2, stats, centroids);

	t2 = double(getTickCount());
	duration_ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "> connectedComponentsWithStats() took " << duration_ms << " ms." << endl;

	// (3) findContours
	t1 = double(getTickCount());

	vector<vector<Point> > contours;
	findContours(src_bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	t2 = double(getTickCount());
	duration_ms = (t2 - t1) * 1000 / getTickFrequency();
	cout << "> findContours() took " << duration_ms << " ms." << endl;

	waitKey(0);
	return 0;
}

