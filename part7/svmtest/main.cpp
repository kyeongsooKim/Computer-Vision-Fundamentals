#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace cv::ml;
using namespace std;

const int WIDTH = 500;
const int HEIGHT = 500;
Mat image;
Mat train, label;
Ptr<SVM> svm;

const char* win_name = "svm";

void on_mouse(int event, int x, int y, int flags, void*);
void trainAndDisplay();

int main()
{
	image = Mat::zeros(Size(WIDTH, HEIGHT), CV_8UC3);

	train = (Mat_<float>(2, 2) << 100, 100, 400, 400);
	label = (Mat_<int>(2, 1) << 0, 1);

	svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	TermCriteria criteria(TermCriteria::MAX_ITER, 100, 1e-6);
	svm->setTermCriteria(criteria);

	trainAndDisplay();

	namedWindow(win_name);
	imshow(win_name, image);
	setMouseCallback(win_name, on_mouse);

	waitKey(0);
	return 0;
}

void on_mouse(int event, int x, int y, int flags, void*)
{
	if (event != EVENT_LBUTTONDOWN && event != EVENT_RBUTTONDOWN)
		return;

	Mat new_sample = (Mat_<float>(1, 2) << x, y);
	train.push_back(new_sample);

	Mat new_label = (Mat_<int>(1, 1) << (event == EVENT_RBUTTONDOWN));
	label.push_back(new_label);

	trainAndDisplay();
}

void trainAndDisplay()
{
	// Do SVM train!
	svm->train(train, ROW_SAMPLE, label);

//	Ptr<TrainData> trainData = TrainData::create(train, ROW_SAMPLE, label);
//	svm->trainAuto(trainData, 2);

	// Show the decision regions given by the SVM

	Vec3b green(0, 255, 0), blue(255, 0, 0);
	for (int i = 0; i < image.rows; ++i) {
		for (int j = 0; j < image.cols; ++j) {
			Mat sampleMat = (Mat_<float>(1, 2) << j, i);
			float response = svm->predict(sampleMat);

			if (response == 0)
				image.at<Vec3b>(i, j) = green;
			else if (response == 1)
				image.at<Vec3b>(i, j) = blue;
		}
	}

	// Show the training data

	int thickness = -1;
	int lineType = 8;

	for (int i = 0; i < train.rows; i++)
	{
		int x = (int)(train.at<float>(i, 0));
		int y = (int)(train.at<float>(i, 1));
		int l = label.at<int>(i, 0);

		if (l == 0)
			circle(image, Point(x, y), 5, Scalar(0, 0, 0), thickness, lineType);
		else
			circle(image, Point(x, y), 5, Scalar(0, 0, 255), thickness, lineType);
	}

	// Show support vectors

	Mat sv = svm->getUncompressedSupportVectors();
	for (int i = 0; i < sv.rows; ++i)
	{
		int x = (int)(sv.at<float>(i, 0));
		int y = (int)(sv.at<float>(i, 1));
		circle(image, Point(x, y), 6, Scalar(128, 255, 255), 2, lineType);
	}

	imshow(win_name, image);
}
