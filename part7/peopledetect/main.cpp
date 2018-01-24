#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void drawDetected(Mat &img, vector<Rect>& detected);

int main()
{
	VideoCapture cap("../data/vtest.avi");

	if (!cap.isOpened()) {
		cerr << "Video load failed!" << endl;
		return -1;
	}

	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	Mat frame;
	for (;;) {
		cap >> frame;

		if (frame.empty()) {
			break;
		}

		// Run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		double start = double(getTickCount());
		vector<Rect> detected;
		hog.detectMultiScale(frame, detected, 0, Size(8, 8), Size(32, 32), 1.05, 2);
		double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
		cout << ": It took " << duration_ms << " ms." << endl;

		//drawDetected(frame, detected);

		for (Rect r : detected) {
			rectangle(frame, r, cv::Scalar(rand() % 256, rand() % 256, rand() % 256), 3);
		}

		imshow("frame", frame);

		if (waitKey(10) == 27)
			break;
	}

	return 0;
}

void drawDetected(Mat &img, vector<Rect>& detected)
{
	vector<Rect> filtered;

	for (size_t i = 0; i < detected.size(); i++)
	{
		Rect r = detected[i];

		size_t j;
		// Do not add small detections inside a bigger detection.
		for (j = 0; j < detected.size(); j++)
			if (j != i && (r & detected[j]) == r)
				break;

		if (j == detected.size())
			filtered.push_back(r);
	}

	for (size_t i = 0; i < filtered.size(); i++)
	{
		Rect r = filtered[i];

		// The HOG detector returns slightly larger rectangles than the real objects,
		// so we slightly shrink the rectangles to get a nicer output.
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
	}
}
