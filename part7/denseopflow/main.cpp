#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// calcOpticalFlowFarneback() 함수 대신 FarnebackOpticalFlow 클래스를 사용하려면
// 아래 주석을 제거하세요.
//#define USE_CLASS

void drawOpticalFlow(const Mat& flow, Mat& flowmap, int step = 16, double scale = 1.0);

int main(int argc, char* argv[])
{
	VideoCapture cap;
	
	if (argc < 2)
		cap.open("../data/vtest.avi");
	else
		cap.open(argv[1]);

	if (!cap.isOpened()) {
		cerr << "Video load failed!" << endl;
		return -1;
	}

	Mat frame1, frame2, gray1, gray2;
	Mat flow, flowmap;

	cap >> frame1;
	cvtColor(frame1, gray1, COLOR_BGR2GRAY);
	
#ifdef USE_CLASS
	Ptr<DenseOpticalFlow> optflow = FarnebackOpticalFlow::create();
#endif

	for (;;) {
		cap >> frame2;

		if (frame2.empty()) {
			break;
		}

		cvtColor(frame2, gray2, COLOR_BGR2GRAY);

		int64 t1 = getTickCount();

#ifdef USE_CLASS
		optflow->calc(gray1, gray2, flow);
#else
		calcOpticalFlowFarneback(gray1, gray2, flow, 0.5, 5, 13, 10, 5, 1.1, 0);
#endif
		
		int64 t2 = getTickCount();
		cout << "It took " << (t2 - t1) * 1000 / getTickFrequency() << "ms." << endl;

		cvtColor(gray2, flowmap, COLOR_GRAY2BGR);
		drawOpticalFlow(flow, flowmap);

		imshow("frame", frame2);
		imshow("flowmap", flowmap);

		if (waitKey(5) == 27)
			break;

		cv::swap(gray1, gray2);
	}

	return 0;
}

void drawOpticalFlow(const Mat& flow, Mat& flowmap, int step, double scale)
{
	Scalar cline(0, 255, 255), cpoint(0, 128, 255);

	for (int j = step / 2; j < flowmap.rows; j += step) {
		const Point2f* ptr = flow.ptr<Point2f>(j);
		for (int i = step / 2; i < flowmap.cols; i += step) {
			int dx = cvRound(ptr[i].x * scale);
			int dy = cvRound(ptr[i].y * scale);
			line(flowmap, Point(i, j), Point(i + dx, j + dy), cline);
			circle(flowmap, Point(i, j), 1, cpoint, -1);
		}
	}
}