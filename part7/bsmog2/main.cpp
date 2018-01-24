#pragma warning(disable: 4819)

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	char* video_name;
	if (argc < 2)
		video_name = "../data/PETS2000.avi";
	else
		video_name = argv[1];
	
	VideoCapture cap(video_name);

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

	Ptr<BackgroundSubtractorMOG2> mog2 = createBackgroundSubtractorMOG2();
//	mog2->setHistory(100);

	Mat frame, fgmask, fgmask_th, fgmask_bgr, bgimage, dst;
	for (;;) {
		cap >> frame;

		if (frame.empty()) {
			cout << "frame is empty!" << endl;
			break;
		}

		mog2->apply(frame, fgmask);
		mog2->getBackgroundImage(bgimage);

		threshold(fgmask, fgmask_th, 100, 255, THRESH_BINARY);

		dst = frame.clone();
		vector<vector<Point> > contours;
		findContours(fgmask_th, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

		cvtColor(fgmask, fgmask_bgr, COLOR_GRAY2BGR);
		for (unsigned i = 0; i < contours.size(); i++) {
			if (contourArea(contours[i]) > 100) {
				drawContours(fgmask_bgr, contours, i, Scalar(0, 255, 128));
				Rect rect = boundingRect(contours[i]);				
				rectangle(dst, rect, Scalar(0, 0, 255), 2);
			}
		}

		imshow("frame", frame);
		imshow("fgmask", fgmask_bgr);
		imshow("bgimage", bgimage);
		imshow("dst", dst);

		int key = waitKey(1000 / fps);
		if (key == 'p') {
			waitKey(0);
		} else if (key == 's') {
			mog2->setDetectShadows(!mog2->getDetectShadows());
		} else if (key == 27) {
			break;
		}
	}

	return 0;
}