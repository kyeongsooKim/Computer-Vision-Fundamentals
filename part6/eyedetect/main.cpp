#pragma warning(disable: 4819)

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	CascadeClassifier face_cascade("haarcascade_frontalface_default.xml");
	if (face_cascade.empty()) {
		cerr << "Failed to open (face) xml file!" << endl;
		return -1;
	}

	CascadeClassifier eyes_cascade("haarcascade_eye.xml");
	if (eyes_cascade.empty()) {
		cerr << "Failed to open (eye) xml file!" << endl;
		return -1;
	}

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	double start = double(getTickCount());

	vector<Rect> faces;
	face_cascade.detectMultiScale(src, faces, 1.1, 3, 0, Size(40, 40));
	for (size_t i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		rectangle(dst, faces[i], Scalar(255, 0, 255), 3);

		Mat faceROI = src(faces[i]);
		std::vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0, Size(30, 30));
		for (size_t j = 0; j < eyes.size(); j++) {
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(dst, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
	}

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}