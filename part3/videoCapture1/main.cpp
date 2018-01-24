#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	namedWindow("camera");
	namedWindow("edge");

	Mat frame, edges;
	for (;;) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		Canny(frame, edges, 50, 150);
		imshow("camera", frame);
		imshow("edge", edges);

		if (waitKey(10) >= 0)
			break;
	}

	return 0;
}