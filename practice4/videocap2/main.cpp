#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	cap.open("Zootopia.mp4");

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	int frame_width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
	int frame_count = (int)cap.get(CAP_PROP_FRAME_COUNT);
	int fps = (int)cap.get(CAP_PROP_FPS);
	cout << "Frame width: " << frame_width << endl;
	cout << "Frame height: " << frame_height << endl;
	cout << "Frame count: " << frame_count << endl;
	cout << "FPS: " << fps << endl;

	namedWindow("src");
	namedWindow("dst");

	cap.set(CAP_PROP_POS_FRAMES, frame_count/2);

	Mat frame, gray, blured, edge;
	for (;;) {
		int64 t1 = getTickCount();

		cap >> frame;
		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		bilateralFilter(gray, blured, -1, 10, 3);
		Canny(blured, edge, 50, 150);

		int64 t2 = getTickCount();
		double ms = (t2 - t1) * 1000 / getTickFrequency();
		cout << "It took " << ms << " ms." << endl;

		imshow("src", frame);
		imshow("dst", edge);

		if (waitKey(1000 / fps) >= 0)
			break;
	}

	return 0;
}