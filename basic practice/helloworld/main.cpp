#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	cout << "Hello OpenCV " << CV_VERSION << endl;

	Mat img; //make variable for saving the image
	img = imread("lenna.bmp", IMREAD_GRAYSCALE); // read the target image in gray scale

	if (img.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow("image"); //make new window named "image"
	imshow("image", img); //show the image in "image" window
	waitKey(0); // and wait till any keyboard input
	return 0;
}