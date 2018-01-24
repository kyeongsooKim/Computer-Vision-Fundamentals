/**************************************************************************

file name	: videorecord
date		: 2017. 8. 19
description	: open the webcam. read from webcam and write it to the file frame by frame
			  then make the result.mp3 file. 

*****************************************************************************/

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

//	int  fourcc = -1; // Codec selection dialog
	int  fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');
	double fps = 15;
	Size size = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CAP_PROP_FRAME_HEIGHT));

	cout << "FPS = " << fps << endl;
	cout << "Size = " << size << endl;

	VideoWriter outputVideo("output.avi", fourcc, fps, size);
	if (!outputVideo.isOpened()) {
		cout << "Can not open outputVideo !!!" << endl;
		return 0;
	}

	int delay = cvRound(1000 / fps);
	Mat frame;
	for (;;) {
		cap >> frame;
		if (frame.empty())
			break;

		outputVideo << frame;
		imshow("frame", frame);

		if (waitKey(delay) == 27)
			break;
	}

	cout << "output.avi file is created!!!" << endl;
	return 0;
}
