#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cerr << "Camera open failed!" << endl;
		return -1;
	}

	cout << "\nHot keys: \n"
		"\tESC - quit the program\n"
		"\tr - auto-initialize tracking\n"
		"\tc - delete all the points\n"
		"\tn - switch the \"night\" mode on/off" << endl;

	const int MAX_COUNT = 50;
	bool needToInit = false;
	bool nightMode = false;

	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);

	Mat frame, image, gray, prev;
	vector<Point2f> ptSrc, ptDst;
	Scalar color[8] = {
		Scalar(255, 0, 0), Scalar(0, 255, 0),
		Scalar(0, 0, 255), Scalar(255, 255, 0),
		Scalar(0, 255, 255), Scalar(255, 0, 255),
		Scalar(128, 255, 0), Scalar(0, 128, 128)
	};

	namedWindow("camera");

	for (;;) {
		cap >> frame;
		if (frame.empty())
			break;

		frame.copyTo(image);
		cvtColor(image, gray, COLOR_BGR2GRAY);

		if (nightMode)
			image = Scalar::all(0);

		if (needToInit) {
			// automatic initialization
			goodFeaturesToTrack(gray, ptDst, MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
			cornerSubPix(gray, ptDst, subPixWinSize, Size(-1, -1), termcrit);
			needToInit = false;
		} else if (!ptSrc.empty()) {
			if (prev.empty())
				gray.copyTo(prev);

			vector<uchar> status;
			vector<float> err;
			calcOpticalFlowPyrLK(prev, gray, ptSrc, ptDst, status, err, winSize,
				3, termcrit, 0, 0.001);

			size_t k = 0;
			for (size_t i = 0; i < ptDst.size(); i++) {

				if (!status[i])
					continue;

				ptDst[k++] = ptDst[i];
				circle(image, ptDst[i], 5, color[i % 8], 3, 8);
			}
			ptDst.resize(k);
		}

		imshow("camera", image);

		char c = (char)waitKey(10);
		if (c == 27)
			break;
		else if (c == 'r')
			needToInit = true;
		else if (c == 'n')
			nightMode = !nightMode;
		else if (c == 'c') {
			ptSrc.clear();
			ptDst.clear();
		}

		std::swap(ptDst, ptSrc);
		cv::swap(prev, gray);
	}

	return 0;
}