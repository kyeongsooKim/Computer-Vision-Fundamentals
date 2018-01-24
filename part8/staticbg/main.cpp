#pragma warning(disable: 4819)

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap("../data/PETS2000.avi");

	if (!cap.isOpened()) {
		cerr << "Video load failed!" << endl;
		return -1;
	}

	int frame_width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
	int frame_count = (int)cap.get(CAP_PROP_FRAME_COUNT);
	int fps = (int)cap.get(CAP_PROP_FPS);
	
	cout << "> Frame width: " << frame_width << endl;
	cout << "> Frame height: " << frame_height << endl;
	cout << "> Frame count: " << frame_count << endl;
	cout << "> FPS: " << fps << endl;

	Mat back;
	cap >> back;

	if (back.empty()) {
		cerr << "Background image registration failed!" << endl;
		return -1;
	}

	Mat frame, diff, diff_th;
	for (;;) {
		cap >> frame;

		if (frame.empty()) {
			cout << "frame is empty!" << endl;
			break;
		}

		absdiff(frame, back, diff);

		cvtColor(diff, diff, CV_BGR2GRAY);
		threshold(diff, diff_th, 30, 255, THRESH_BINARY);

#if 0
		erode(diff_th, diff_th, Mat());
		dilate(diff_th, diff_th, Mat());
#endif

		imshow("diff", diff_th);
		imshow("frame", frame);

		int key = waitKey(1000 / fps);
		if (key == 'p') {
			waitKey(0);
		} else if (key == 27) {
			break;
		}
	}

	return 0;
}