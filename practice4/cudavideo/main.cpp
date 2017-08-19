#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/cudaimgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::cuda;

int main(void)
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

	cap.set(CAP_PROP_POS_FRAMES, frame_count / 2);

	Mat frame, gray, edges;
	GpuMat d_frame, d_gray, d_blured, d_edges;

	Ptr<CannyEdgeDetector> canny = createCannyEdgeDetector(100, 200, 3);

	for (;;) {
		int64 t1 = getTickCount();

		cap >> frame;
		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		cv::cvtColor(frame, gray, COLOR_BGR2GRAY);

		d_gray.upload(gray);
		cuda::bilateralFilter(d_gray, d_blured, -1, 10, 3);

		canny->detect(d_blured, d_edges);

		d_edges.download(edges);

		int64 t2 = getTickCount();
		double ms = (t2 - t1) * 1000 / getTickFrequency();
		cout << "It took " << ms << " ms." << endl;

		imshow("src", frame);
		imshow("dst", edges);

		if (waitKey(1000 / fps) == 27)
			break;
	}

	return 0;
}


