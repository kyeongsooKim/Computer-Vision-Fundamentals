#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int ksize = 101;
int pos_sigma = 10;
int pos_theta = 0;
int pos_lambd = 10;

Mat kernel;
Mat src, dst;
const char* win_name = "Gabor";

void on_trackbar_gabor(int, void*)
{
	double sigma = double(pos_sigma);
	double theta = pos_theta * CV_PI / 180.;
	double lambd = double(pos_lambd);

	double gamma = 1.;
	double psi = 0.;

	kernel = getGaborKernel(Size(ksize, ksize), sigma, theta, lambd, gamma, psi);

	filter2D(src, dst, -1, kernel);
	imshow("dst", dst);

	Mat kernel_norm, kernel_img;
	kernel_norm = (kernel + 1.0) * 128.;
	kernel_norm.convertTo(kernel_img, CV_8U, 1, 0);
	resize(kernel_img, kernel_img, Size(512, 512));
	imshow("Gabor", kernel_img);
}

int main()
{
	src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	namedWindow(win_name, WINDOW_NORMAL);
	resizeWindow(win_name, 640, 800);
	createTrackbar("Sigma", win_name, &pos_sigma, ksize / 2, on_trackbar_gabor);
	createTrackbar("Theta", win_name, &pos_theta, 180, on_trackbar_gabor);
	createTrackbar("Lambda", win_name, &pos_lambd, 100, on_trackbar_gabor);
	on_trackbar_gabor(0, 0);

	namedWindow("dst");

	waitKey(0);
	return 0;
}
