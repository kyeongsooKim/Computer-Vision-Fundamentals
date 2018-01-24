#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3) {
		cerr << "Usage: stitching.exe <image_file1> <image_file2> [<image_file3> ...]" << endl;
		return -1;
	}

	vector<Mat> imgs;

	for (int i = 1; i < argc; i++) {
		Mat img = imread(argv[i]);
		if (img.empty()) {
			cerr << "Image load failed!" << endl;
			return -1;
		}

		imgs.push_back(img);
	}

	Mat dst;
	Ptr<Stitcher> stitcher = Stitcher::create();
//	Ptr<Stitcher> stitcher = createStitcher();

	cout << "Start stitching!" << endl;
	double start = double(getTickCount());

	Stitcher::Status status = stitcher->stitch(imgs, dst);

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	if (status == Stitcher::Status::OK) {
		imwrite("result.jpg", dst);
		cout << "result.jpg file is generated!" << endl;

		namedWindow("dst");
		imshow("dst", dst);
		waitKey(0);
	} else {
		cerr << "Error on stitching!" << endl;
	}

	return 0;
}
