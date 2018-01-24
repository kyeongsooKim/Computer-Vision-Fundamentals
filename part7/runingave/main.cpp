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

	double alpha = 0.01;
	Mat ave_back;
	back.convertTo(ave_back, CV_32FC3);

	Mat frame, res, dst, diff, diff_th, diff_th_cpy;
	for (;;) {
		cap >> frame;
	
		if (frame.empty()) {
			cout << "frame is empty!" << endl;
			break;
		}

		accumulateWeighted(frame, ave_back, alpha);
		ave_back.convertTo(back, CV_8UC3);

		absdiff(frame, back, diff);
		cvtColor(diff, diff_th, CV_BGR2GRAY);
		threshold(diff_th, diff_th, 30, 255, THRESH_BINARY);

#if 1
		erode(diff_th, diff_th, Mat());
		dilate(diff_th, diff_th, Mat());
#endif

		dst = frame.clone();
		diff_th_cpy = diff_th.clone();
		vector<vector<Point> > contours;
		findContours(diff_th_cpy, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

		for (unsigned i = 0; i < contours.size(); i++) {
			if (contourArea(contours[i]) > 80) {
				Rect rect = boundingRect(contours[i]);
				rectangle(dst, rect, Scalar(0, 0, 255), 2);
			}
		}

		imshow("frame", frame);
		imshow("diff_th", diff_th);
		imshow("back", back);
		imshow("dst", dst);

		int key = waitKey(1000 / fps);
		if (key == 'p') {
			waitKey(0);
		} else if (key == 27) {
			break;
		}
	}

	return 0;
}