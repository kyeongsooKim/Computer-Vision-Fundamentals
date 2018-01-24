#pragma warning(disable: 4819)

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap("../data/camshift.avi");

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
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

	Mat src, roi, roi_hsv, hist;
	Rect track_rect(135, 220, 100, 100);

	cap >> src;				// 첫 번째 프레임
	roi = src(track_rect);	// 첫 번째 프레임에서 추적할 영역 부분 영상
	cvtColor(roi, roi_hsv, COLOR_BGR2HSV);

	// 히스토그램 계산
	int hbins = 45; int sbins = 64;
	int histSize[] = { hbins, sbins };
	float hrange[] = { 0, 180 }, srange[] = { 0, 256 };
	const float* ranges[] = { hrange, srange };
	int channels[] = { 0, 1 };

	calcHist(&roi_hsv, 1, channels, Mat(), hist, 2, histSize, ranges);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	Mat frame, frame_hsv, backproj, backproj_norm;
	RotatedRect rot_rect;
	TermCriteria criteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1);

	while (true) {
		cap >> frame;
		if (frame.empty())
			break;

		cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
		calcBackProject(&frame_hsv, 1, channels, hist, backproj, ranges);
		normalize(backproj, backproj_norm, 0, 255, NORM_MINMAX, -1, Mat());

		rot_rect = CamShift(backproj, track_rect, criteria);
		rectangle(frame, track_rect, Scalar(0, 0, 255), 2);
		ellipse(frame, rot_rect, Scalar(0, 255, 0), 2);

		imshow("frame", frame);
		imshow("backproj", backproj);

		if (waitKey(1000 / fps) == 27)
			break;
	}

	return 0;
}