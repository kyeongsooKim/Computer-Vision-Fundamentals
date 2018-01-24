#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

CascadeClassifier cascade;
const char* win_name = "camera";

void detectAndDisplay(Mat frame);

int main(int argc, char* argv[])
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	char* cascade_name;
	if (argc < 2) {
		cascade_name = "cascade.xml";
	} else {
		cascade_name = argv[1];
	}

	if (!cascade.load(cascade_name)) {
		cerr << "Failed to open xml file!" << endl;
		return -1;
	}

	namedWindow(win_name);

	Mat frame;

	for (;;) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		detectAndDisplay(frame);

		if (waitKey(10) >= 0)
			break;
	}

	return 0;
}

void detectAndDisplay(Mat frame)
{
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	vector<Rect> objects;
	cascade.detectMultiScale(frame_gray, objects, 1.1, 2, 0, Size(40, 40));

	for (Rect object : objects) {
		Point center(object.x + object.width / 2, object.y + object.height / 2);
		rectangle(frame, object, Scalar(255, 0, 255), 3);
	}

	imshow(win_name, frame);
}
