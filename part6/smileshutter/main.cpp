#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

const int MIDDLE_SMILE = 30;
const int BIG_SMILE = 60;

int main()
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	CascadeClassifier face_cascade("haarcascade_frontalface_alt.xml");
	if (face_cascade.empty()) {
		cerr << "Failed to open (face) xml file!" << endl;
		return -1;
	}

	CascadeClassifier smile_cascade("haarcascade_smile.xml");
	if (smile_cascade.empty()) {
		cerr << "Failed to open (eye) xml file!" << endl;
		return -1;
	}

	namedWindow("camera");

	Mat frame, dst, gray;
	for (;;) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		dst = frame.clone();
		cvtColor(dst, gray, COLOR_BGR2GRAY);
		
		vector<Rect> faces;
		face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(40, 40));

		for (Rect face : faces) {
			Rect smileROI(face.x, face.y + face.height / 2, face.width, face.height / 2);
			Mat faceROI = gray(smileROI);

			std::vector<Rect> smiles;
			smile_cascade.detectMultiScale(faceROI, smiles, 1.1, 0, 0, Size(30, 30));

			int smile_cnt = (int)smiles.size();
			cout << smile_cnt << endl;

			if (smile_cnt >= BIG_SMILE) {
				rectangle(dst, face, Scalar(0, 0, 255), 3);

				static int cnt = 1;
				String filename = format("frame%d.jpg", cnt++);
				vector<int> params(IMWRITE_JPEG_QUALITY, 95);
				imwrite(filename, frame, params);
			} else if (smile_cnt < BIG_SMILE && smile_cnt >= MIDDLE_SMILE) {
				rectangle(dst, face, Scalar(0, 128, 255), 3);
			} else {
				rectangle(dst, face, Scalar(255, 255, 0), 3);
			}
		}
		
		imshow("camera", dst);

		if (waitKey(5) >= 0)
			break;
	}
	return 0;
}
